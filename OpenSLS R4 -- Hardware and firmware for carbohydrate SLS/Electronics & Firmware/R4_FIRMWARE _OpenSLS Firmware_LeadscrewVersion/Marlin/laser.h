/*
  laser.h - Laser cutter control library for Arduino using 16 bit timers- Version 1
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

#ifndef LASER_H
#define LASER_H

#include <inttypes.h>
#include "Configuration.h"

// split into planned and status
typedef struct {
  int fired; // method used to ask the laser to fire - LASER_FIRE_G1, LASER_FIRE_SPINDLE, LASER_FIRE_E, etc
  float intensity; // Laser firing instensity 0.0 - 100.0
  float ppm; // pulses per millimeter, for pulsed firing mode
  unsigned long duration; // laser firing duration in microseconds, for pulsed firing mode
  unsigned long dur; // instantaneous duration
  bool status; // LASER_ON / LASER_OFF - buffered
  bool firing; // LASER_ON / LASER_OFF - instantaneous
  uint8_t mode; // CONTINUOUS, PULSED, RASTER
  unsigned long last_firing; // microseconds since last laser firing
  bool diagnostics; // Verbose debugging output over serial
  unsigned int time; // temporary counter to limit eeprom writes
  unsigned int lifetime; // laser lifetime firing counter in minutes
  #ifdef LASER_RASTER
    char raster_data[LASER_MAX_RASTER_LINE];
    float raster_aspect_ratio;
    float raster_mm_per_pulse;
    int raster_raw_length;
    int raster_num_pixels;
    bool raster_direction;
  #endif // LASER_RASTER
  #ifdef MUVE_Z_PEEL
    float peel_distance;
    float peel_speed;
    float peel_pause;
  #endif // MUVE_Z_PEEL
} laser_t;

extern laser_t laser;

void laser_init();
void laser_fire(int intensity);
void laser_extinguish();
void laser_update_lifetime();
void laser_set_mode(int mode);
#ifdef LASER_PERIPHERALS
  bool laser_peripherals_ok();
  void laser_peripherals_on();
  void laser_peripherals_off();
  void laser_wait_for_peripherals();
#endif // LASER_PERIPHERALS

// Laser constants
#define LASER_OFF 0
#define LASER_ON 1

#define CONTINUOUS 0
#define PULSED 1
#define RASTER 2

#endif // LASER_H
