/*
  laser.cpp - Laser control library for Arduino using 16 bit timers- Version 1
  Copyright (c) 2013 Timothy Schmidt.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "laser.h"
#include "Configuration.h"
#include "ConfigurationStore.h"
#include "pins.h"
#include <avr/interrupt.h>
#include <Arduino.h>
#include "Marlin.h"

laser_t laser;

void timer3_init(int pin) {
	pinMode(pin, OUTPUT);
    analogWrite(pin, 1);  // let Arduino setup do it's thing to the PWM pin

    TCCR3B = 0x00;  // stop Timer4 clock for register updates
    TCCR3A = 0x82; // Clear OC3A on match, fast PWM mode, lower WGM3x=14
    ICR3 = labs(F_CPU / LASER_PWM); // clock cycles per PWM pulse
    OCR3A = labs(F_CPU / LASER_PWM) - 1; // ICR3 - 1 force immediate compare on next tick
    TCCR3B = 0x18 | 0x01; // upper WGM4x = 14, clock sel = prescaler, start running

    noInterrupts();
    TCCR3B &= 0xf8; // stop timer, OC3A may be active now
    TCNT3 = labs(F_CPU / LASER_PWM); // force immediate compare on next tick
    ICR3 = labs(F_CPU / LASER_PWM); // set new PWM period
    TCCR3B |= 0x01; // start the timer with proper prescaler value
    interrupts();
}

void timer4_init(int pin) {
	pinMode(pin, OUTPUT);
    analogWrite(pin, 1);  // let Arduino setup do it's thing to the PWM pin

    TCCR4B = 0x00;  // stop Timer4 clock for register updates
    TCCR4A = 0x82; // Clear OC4A on match, fast PWM mode, lower WGM4x=14
    ICR4 = labs(F_CPU / LASER_PWM); // clock cycles per PWM pulse
    OCR4A = labs(F_CPU / LASER_PWM) - 1; // ICR4 - 1 force immediate compare on next tick
    TCCR4B = 0x18 | 0x01; // upper WGM4x = 14, clock sel = prescaler, start running

    noInterrupts();
    TCCR4B &= 0xf8; // stop timer, OC4A may be active now
    TCNT4 = labs(F_CPU / LASER_PWM); // force immediate compare on next tick
    ICR4 = labs(F_CPU / LASER_PWM); // set new PWM period
    TCCR4B |= 0x01; // start the timer with proper prescaler value
    interrupts();
}

void laser_init()
{
  // Initialize timers for laser intensity control
  #if LASER_CONTROL == 1
    if (LASER_FIRING_PIN == 2 || LASER_FIRING_PIN == 3 || LASER_FIRING_PIN == 5) timer3_init(LASER_FIRING_PIN);
    if (LASER_FIRING_PIN == 6 || LASER_FIRING_PIN == 7 || LASER_FIRING_PIN == 8) timer4_init(LASER_FIRING_PIN);
  #endif
  #if LASER_CONTROL == 2
    if (LASER_INTENSITY_PIN == 2 || LASER_INTENSITY_PIN == 3 || LASER_INTENSITY_PIN == 5) timer3_init(LASER_INTENSITY_PIN);
    if (LASER_INTENSITY_PIN == 6 || LASER_INTENSITY_PIN == 7 || LASER_INTENSITY_PIN == 8) timer4_init(LASER_INTENSITY_PIN);
  #endif

  #ifdef LASER_PERIPHERALS
  digitalWrite(LASER_PERIPHERALS_PIN, HIGH);  // Laser peripherals are active LOW, so preset the pin
  pinMode(LASER_PERIPHERALS_PIN, OUTPUT);

  digitalWrite(LASER_PERIPHERALS_STATUS_PIN, HIGH);  // Set the peripherals status pin to pull-up.
  pinMode(LASER_PERIPHERALS_STATUS_PIN, INPUT);
  #endif // LASER_PERIPHERALS

  // initialize state to some sane defaults
  laser.intensity = 100.0;
  laser.ppm = 0.0;
  laser.duration = 0;
  laser.status = LASER_OFF;
  laser.firing = LASER_OFF;
  laser.mode = CONTINUOUS;
  laser.last_firing = 0;
  laser.diagnostics = false;
  laser.time = 0;
  #ifdef LASER_RASTER
    laser.raster_aspect_ratio = LASER_RASTER_ASPECT_RATIO;
    laser.raster_mm_per_pulse = LASER_RASTER_MM_PER_PULSE;
    laser.raster_direction = 1;
  #endif // LASER_RASTER
  #ifdef MUVE_Z_PEEL
    laser.peel_distance = 2.0;
    laser.peel_speed = 2.0;
    laser.peel_pause = 0.0;
  #endif // MUVE_Z_PEEL
  
  laser_extinguish();
}
void laser_fire(int intensity = 100.0){
	laser.firing = LASER_ON;
	laser.last_firing = micros(); // microseconds of last laser firing
	if (intensity > 100.0) intensity = 100.0; // restrict intensity between 0 and 100
	if (intensity < 0) intensity = 0;

    pinMode(LASER_FIRING_PIN, OUTPUT);
	#if LASER_CONTROL == 1
	  analogWrite(LASER_FIRING_PIN, labs((intensity / 100.0)*(F_CPU / LASER_PWM)));
    #endif
	#if LASER_CONTROL == 2
      analogWrite(LASER_INTENSITY_PIN, labs((intensity / 100.0)*(F_CPU / LASER_PWM)));
      digitalWrite(LASER_FIRING_PIN, HIGH);
    #endif

    if (laser.diagnostics) {
	  SERIAL_ECHOLN("Laser fired");
	}
}
void laser_extinguish(){
	if (laser.firing == LASER_ON) {
	  laser.firing = LASER_OFF;

	  // Engage the pullup resistor for TTL laser controllers which don't turn off entirely without it.
	  digitalWrite(LASER_FIRING_PIN, LOW);
	  laser.time += millis() - (laser.last_firing / 1000);

	  if (laser.diagnostics) {
	    SERIAL_ECHOLN("Laser extinguished");
	  }
	}
}
void laser_set_mode(int mode){
	switch(mode){
		case 0:
		  laser.mode = CONTINUOUS;
		  return;
		case 1:
		  laser.mode = PULSED;
		  return;
		case 2:
		  laser.mode = RASTER;
		  return;
	}
}
#ifdef LASER_PERIPHERALS
bool laser_peripherals_ok(){
	return !digitalRead(LASER_PERIPHERALS_STATUS_PIN);
}
void laser_peripherals_on(){
	digitalWrite(LASER_PERIPHERALS_PIN, LOW);
	if (laser.diagnostics) {
	  SERIAL_ECHO_START;
	  SERIAL_ECHOLNPGM("Laser Peripherals Enabled");
    }
}
void laser_peripherals_off(){
	if (!digitalRead(LASER_PERIPHERALS_STATUS_PIN)) {
	  digitalWrite(LASER_PERIPHERALS_PIN, HIGH);
	  if (laser.diagnostics) {
	    SERIAL_ECHO_START;
	    SERIAL_ECHOLNPGM("Laser Peripherals Disabled");
      }
    }
}
void laser_wait_for_peripherals() {
	unsigned long timeout = millis() + LASER_PERIPHERALS_TIMEOUT;
	if (laser.diagnostics) {
	  SERIAL_ECHO_START;
	  SERIAL_ECHOLNPGM("Waiting for peripheral control board signal...");
	}
	while(!laser_peripherals_ok()) {
		if (millis() > timeout) {
			if (laser.diagnostics) {
			  SERIAL_ERROR_START;
			  SERIAL_ERRORLNPGM("Peripheral control board failed to respond");
			}
			Stop();
			break;
		}
	}
}
#endif // LASER_PERIPHERALS
