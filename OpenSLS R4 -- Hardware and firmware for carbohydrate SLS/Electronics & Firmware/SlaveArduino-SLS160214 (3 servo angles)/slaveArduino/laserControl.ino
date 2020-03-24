//#include <pins.h>
#define LASER_EN_PIN 8
#define LASER_PWM_PIN 9

void laserOn()
{
  digitalWrite(LASER_EN_PIN,LOW);
}

void laserOff()
{
  digitalWrite(LASER_EN_PIN, HIGH);
}

void laze(int laserPWM)
{
  laserOn();
  digitalWrite(LASER_PWM_PIN, laserPWM);
}

int getPWMval(int pwmInput)
{
  int cycleHigh = pulseIn(pwmInput, HIGH);
  int cycleLow = pulseIn(pwmInput, LOW);
  return map(cycleHigh,0,cycleLow+cycleHigh,0,255);
//  Serial.print("cycleHigh = ");
//  Serial.print(cycleHigh);
//  Serial.print(", cycleLow = ");
//  Serial.print(cycleLow);
//  Serial.print(", and ratio is ");
//  int val = map(cycleHigh,0,cycleLow+cycleHigh,0,255);
//  Serial.println(val);
//  Serial.println();  
  
  
  
  
//don't need to do it by hand.  use pulseIn func.  

//  //need precision, so use microseconds.  Make sure there isn't overflow though
//  unsigned long cycleStart;
//  unsigned long cycleEnd;
//  unsigned long cycleHigh;
//  unsigned long cycleLow;
//  unsigned long maxDiff = 1000000;//check for overflow using big value (1 sec)
//
//  while(1)//keep on keeping on-- if measurement is clean, exits the func
//  {
//    Serial.println("in main while loop");
//
//    while(digitalRead(pwmInput == LOW))
//    {
//      if(digitalRead(pwmInput) == HIGH)//catch the leading edge of the PWM signal
//      {
//        cycleStart = micros();
//        while(digitalRead(pwmInput) == HIGH)
//        {
//          cycleEnd = micros(); 
//        }
//        cycleHigh = cycleEnd - cycleStart; //duration of high signal
//        
//        //should be right on the falling edge now, so restart cycle timers:
//        cycleStart = micros();
//        Serial.print("cycleHigh:  ");
//        Serial.println(cycleHigh);
//        while(digitalRead(pwmInput) == LOW)
//        {
//          cycleEnd = micros(); 
//        }
//        cycleLow = cycleEnd - cycleStart;
//        Serial.print("cycleLow:  ");
//        Serial.println(cycleLow);
//        
//        //have the up/down times now, so check to see if we encountered an overflow:
//        if(cycleHigh > 0 || cycleLow > 0)
//        {
//          Serial.println(int(100*int(cycleHigh)/int(cycleLow)));
//          Serial.println();
//
//          return int(100*cycleHigh/cycleLow);
//        }
//        break;//got the times, get outta here
//      } 
//    }//end while(digitalRead(pwmInput) == HIGH)
//
//
//  }//end while(1)

}




