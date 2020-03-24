
/*
Ian Kinstlinger, 06/2015 for MillerLab OpenSLS
Modified from Andreas Bastian, 2013
 Arduino code for execution on a slaved Arduino Uno as a temporary
 means for controlling both the laser and the powder management
 hardware.
 
 */
 
//Define motor speeds and servo angles
int CWspeed = 250; //PWM value
int CCWspeed = 200; //PWM
int DownAngle = 150; //deg
int DownAngle2 = 47;
int UpAngle = 93; //deg
int UpAngle2 = 100;
int ExUpAngle = 10;
int ExUpAngle2 = 180;
 

//define pins for DC motor and servo motor, plus comm pins with rambo

int DC_CW = 3; //PWM pin needed
int DC_CCW = 5; //PWM pin needed

int SERVO_PIN = 10; //Reserved in servo.h
int SERVO_PIN2  = 8;
int DC_MOTOR_SIGNAL_IN = A0;
int SERVO_SIGNAL_IN = A7;


//#include <pins.h>
#include <math.h>
#include <SoftwareSerial.h>
//#include "A4988_stepper.h"
#include "Arduino.h"
#include <Servo.h>


int DCstatus = 0;
int i = 0;
int LED = 13;
int layerCount = 0;
Servo SpatulaServo;
Servo SpatulaServo2;

void setup()
{

  Serial.begin(9600);


  pinMode(LED, OUTPUT);
  pinMode(DC_MOTOR_SIGNAL_IN, INPUT);
  pinMode(SERVO_SIGNAL_IN, INPUT);
  pinMode(DC_CW, OUTPUT);
  pinMode(DC_CCW, OUTPUT);
  pinMode(SERVO_PIN, OUTPUT);
  pinMode(SERVO_PIN2, OUTPUT);
  
  
  SpatulaServo.attach(SERVO_PIN);
  SpatulaServo2.attach(SERVO_PIN2);
  
}

void loop()
{
 
  DCstatus = checkDCPin(DC_MOTOR_SIGNAL_IN); //what should the motor be doing
  if(DCstatus == 0) //Not plowing, motor off
  {

    digitalWrite(LED,LOW);
    analogWrite(DC_CW, 0);
    analogWrite(DC_CCW, 0);

  }//end if
  
  if(DCstatus == 1) //Plowing with clockwise motor
  {
    analogWrite(DC_CCW, 0);
    analogWrite(DC_CW, CWspeed);
    digitalWrite(LED,HIGH);
    
  }
  
  else if(DCstatus == 2) // Plowing w CCW motor
  {
    analogWrite(DC_CW, 0);
    analogWrite(DC_CCW, CCWspeed);
    digitalWrite(LED,HIGH);
  }
  
  int ServoStatus = checkServoPin(SERVO_SIGNAL_IN);
  if(ServoStatus == 2)
  {
    SpatulaServo.write(ExUpAngle);
    SpatulaServo2.write(ExUpAngle2);
  }
  if(ServoStatus == 1)
  { 
    SpatulaServo.write(DownAngle);
    SpatulaServo2.write(DownAngle2);
  }
  
  if(ServoStatus == 0) //Spatula in plow position
  {
    SpatulaServo.write(UpAngle);
    SpatulaServo2.write(UpAngle2);
  }
  
   


}//end loop()

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////



int checkDCPin(int DCin)
{

    int duration = pulseIn(DCin, HIGH);

  //Serial.println(duration);
  if(duration < 500)
  { 
    return 0;
  }
  
  else if(duration > 500 && duration < 1000)
  {
    return 1;
  }
  
  else if(duration > 1000)
  {
    return 2;
  }
  
}
   


int checkServoPin(int ServoIn)
{
    
  int ServoState = pulseIn(ServoIn, HIGH);
  
  int highthreshold = 1000;
  int lowthreshold = 100;
  //Serial.println(ServoState);
  
  if(ServoState < highthreshold && ServoState > lowthreshold)
  {
    return 0;
  }
  
  else if(ServoState > highthreshold)
  {
    return 1;
  }

  else if(ServoState < lowthreshold)
  {
    return 2;
  }
  
}

    
    
    
    /* //though I think is cleaner implementation, it doesn't
     //seem to catch two successive pulses and still misfires 
     //when the RAMBO resets.
     while(analogRead(layerPin) > 310)
     {
     //measure duration of high signal
     later = millis(); 
     }
     if(later-now >= 400)
     {
     return true;
     }
     */


float fabs(float val)
{
  if(val < 0)
  {
    return -1*val;
  }
  else
  {
    return val;
  }
}

/*

 
 
 
 
 
 
 
 //move a distance at a velocity
 void moveDist(int _sp, int _dp, float dist, float velocity, float _spmm)
 {
 int stepDelay = 1/(_spmm*velocity);
 int numSteps = _spmm*dist;
 setDir(, dist);
 while(numSteps != 0)
 {
 numSteps--;
 STEP();
 }
 }
 
 
 //just a simple func to set motor direction based on sign of target distance
 void setDir(int _dp, float dist)
 {
 if(dist < 0)
 {
 digitalWrite(_dp, HIGH); 
 }
 else
 {
 digitalWrite(_dp, LOW);
 }
 }
 
 //take a single step
 void STEP(int _sp, float stepDelay)
 {
 digitalWrite(_sp, HIGH);
 delayMicroseconds(stepDelay/2);
 digitalWrite(_sp, LOW);
 delayMicroseconds(stepDelay/2); 
 }
 
 
 
 */




//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////









