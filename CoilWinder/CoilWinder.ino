
#include <stdint.h>

#define M_ROTATE    1
#define M_SLIDE     0
#define FWD         LOW
#define BACK        HIGH

const int stepSlidePin = 2;
const int dirSlidePin  = 5;
const int stepRotatePin = 3;
const int dirRotatePin  = 6;
const int enPin = 8;

const uint32_t maxSliderSteps = 22000;      // Maximum slider steps to sweep maximum coil length
const uint32_t maxCoilLength = 100000;      // Maximum coil length in um
const int rotaryMotorStepsPerTurn = 200;  // Number of steps to make for one coil turn

uint32_t sliderStepsPerCoilLength = 100000; // Number of steps the slider should make to sweep the entire coil length.
float rotationSlideRatio = 1;  
uint32_t currentSLiderSteps = 0;
uint32_t currentRotarySteps = 0;
uint32_t targetSliderSteps = 0;
uint32_t targetRotarySteps = 0;
bool pauseFlag = false;

// The following variables will be adjusted via LCD menu
uint32_t wireDiameter = 500;                // Wire diameter in um
uint32_t coilLength = 5000;                // Coil length in um
uint32_t wireTurnCount = 1000;              // Number of wire turns to wind on the coil

/* Calculates number of slider steps per wire turn based on preset wire diameter and number of slider steps to sweep the coil length.
 * This should be run every time the parameters are changed.
 */
void calibrateMotors(){
    uint32_t wireTurnsPerCoilLength = coilLength/wireDiameter;
    uint32_t rotaryMotorStepsPerCoilLength = wireTurnsPerCoilLength * rotaryMotorStepsPerTurn;
    sliderStepsPerCoilLength = (maxSliderSteps * coilLength) / maxCoilLength;
    rotationSlideRatio = (float)rotaryMotorStepsPerCoilLength / (float)sliderStepsPerCoilLength;

    targetRotarySteps = wireTurnCount * rotaryMotorStepsPerTurn;

    Serial.print("sliderStepsPerCoilLength:");
    Serial.println(sliderStepsPerCoilLength);
}

void updateMotors(){
  if(pauseFlag){
    digitalWrite(enPin,HIGH);   // Disable steppers
  }else{
    digitalWrite(enPin,LOW);    // Enable motors

    // update rotation
    digitalWrite(dirRotatePin, FWD);
    
    digitalWrite(stepRotatePin,HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepRotatePin,LOW);
    delayMicroseconds(1000);

    currentRotarySteps++;

    // Update slider
    targetSliderSteps = (float)currentRotarySteps / rotationSlideRatio;
    
    while(currentSLiderSteps < targetSliderSteps){
      uint32_t relativeSliderPosition = currentSLiderSteps % (sliderStepsPerCoilLength * 2);
      bool slideForward = relativeSliderPosition < sliderStepsPerCoilLength;

      Serial.print("currentSLiderSteps:");
      Serial.println(currentSLiderSteps);

      if(slideForward){
        digitalWrite(dirSlidePin, FWD);
        Serial.println("FWD");

      }else{
        digitalWrite(dirSlidePin, BACK);
        Serial.println("BACK");
      }

      digitalWrite(stepSlidePin,HIGH);
      delayMicroseconds(1000);
      digitalWrite(stepSlidePin,LOW);
      delayMicroseconds(1000);

      currentSLiderSteps++;

      
    }
  }
}


void setup() {
  pinMode(stepSlidePin,OUTPUT);
  pinMode(dirSlidePin,OUTPUT);

  pinMode(stepRotatePin,OUTPUT);
  pinMode(dirRotatePin,OUTPUT);

  pinMode(enPin,OUTPUT);
  digitalWrite(enPin,HIGH);

  Serial.begin(115200); // opens serial port, sets data rate to 115200 bps
  Serial.println("\n\nStarting coil winder\n");
  calibrateMotors();
}

void loop() {
  updateMotors();  
}
