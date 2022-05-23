// Simple stepper motor driver for equatorial tracker

#include <Arduino.h>
//#include "BasicStepperDriver.h"

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 2
#define REDUCTION_MOTOR 100
#define REDUCTION_MOUNT 

// Since microstepping is set externally, make sure this matches the selected mode
// If it doesn't, the motor will move at a different RPM than chosen
// 1=full step, 2=half step etc.
#define MICROSTEPS 16

// All the wires needed for full functionality
#define DIR 61 //MKS gen 1.4 Y stepper
#define STEP 60 //MKS gen 1.4 Y stepper
#define SLEEP 56 //MKS gen 1.4 Y stepper

// Micros counts up to 4,294,967,295 so we need to check for overflows for long tracking
// First iteration delay 92308 us
// Second iteration delay 98120 us
// Third iteration delay 97638 us
// Fourth iteration with a 100:1 planetary gearbos 976us per step    
// Changing this value will change the rotation speed, a bigger value means a slower speed and vice-versa
const unsigned long stepDuration = 976;
unsigned long nextStep = 0;
const unsigned long microsLimit = 4294967295- stepDuration;

void setup() {
    pinMode(DIR,OUTPUT);
    pinMode(STEP,OUTPUT);
    pinMode(SLEEP,OUTPUT);

    // Enable the stepper driver
    digitalWrite(SLEEP,0);

    // Change the rotation direction according to the pole you are in
    // North Pole 0
    // South Pole 1
    digitalWrite(DIR,1);

    // Sets steps counter
    nextStep = micros() + stepDuration;
}

void loop() {
    // Non blocking with micros() to allow adjustments    
    if (micros() > nextStep)
    {
      // Toggle step pin
      digitalWrite(STEP,!digitalRead(STEP));

      // Check and correct for overflow
      unsigned long nextMicros = micros();
      if (nextMicros >= microsLimit)
      {
        nextMicros = nextMicros - microsLimit;
      }

      // Set next step
      nextStep = nextMicros + stepDuration;
    }  
}
