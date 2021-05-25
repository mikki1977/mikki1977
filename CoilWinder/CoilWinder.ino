#include <stdint.h>
#include "lcd.h"
#include "config.h"

#define M_ROTATE    1
#define M_SLIDE     0
#define FWD         LOW
#define BACK        HIGH

uint32_t sliderStepsPerCoilLength = 100000;     // Number of steps the slider should make to sweep the entire coil length.
float rotationSlideRatio = 1;  
uint32_t currentSLiderSteps = 0;
uint32_t currentRotarySteps = 0;
uint32_t targetSliderSteps = 0;
uint32_t targetRotarySteps = 0;
bool pauseFlag = false;
int slideDirection = BACK;
uint8_t processLcdCounter = 0;

// The following variables will be adjusted via LCD menu
uint32_t wireDiameter = 500;                // Wire diameter in um
uint32_t coilLength = 30000;                // Coil length in um
uint32_t wireTurnCount = 1000;              // Number of wire turns to wind on the coil


/* Calculates number of slider steps per wire turn based on preset wire diameter and number of slider steps to sweep the coil length.
 * This should be run every time the parameters are changed.
 */
void calibrateMotors(){
    uint32_t wireTurnsPerCoilLength = coilLength/wireDiameter;
    uint32_t rotaryMotorStepsPerCoilLength = wireTurnsPerCoilLength * rotaryMotorStepsPerTurn * 2;  // Multiply by 2 because each pulse will toggle pin.
    sliderStepsPerCoilLength = (maxSliderSteps * coilLength * 2) / maxCoilLength;                   // Multiply by 2 because each pulse will toggle pin.
    rotationSlideRatio = (float)rotaryMotorStepsPerCoilLength / (float)sliderStepsPerCoilLength;

    targetRotarySteps = wireTurnCount * rotaryMotorStepsPerTurn;
}

//This is the interrupt request routine triggered at 1ms rate
ISR(TIMER0_COMPA_vect){    
  // update rotation
  digitalWrite(PIN_ROTATE_DIR, FWD);
  int rotateState = currentRotarySteps & 1;  
  digitalWrite(PIN_ROTATE_STEP,rotateState);
  currentRotarySteps++;
  
  // Update slider
  targetSliderSteps = (float)currentRotarySteps / rotationSlideRatio;
  if(currentSLiderSteps < targetSliderSteps){
    uint32_t relativeSliderPosition = currentSLiderSteps % (sliderStepsPerCoilLength * 2);
    bool slideForward = relativeSliderPosition < sliderStepsPerCoilLength;

    if(slideForward){
      if(slideDirection == BACK){
        Serial.println("FWD");
      }
      slideDirection = FWD; 
    }else{
      if(slideDirection == FWD){
        Serial.println("BACK");        
      }
      slideDirection = BACK;        
    }    
    
    digitalWrite(PIN_SLIDE_DIR, slideDirection);
    
    int slideState = currentSLiderSteps & 1;  
    digitalWrite(PIN_SLIDE_STEP,slideState);    

    currentSLiderSteps++;    
  } 

  processLcdCounter++;
}

void setup() {
  pinMode(PIN_SLIDE_STEP,OUTPUT);
  pinMode(PIN_SLIDE_DIR,OUTPUT);

  pinMode(PIN_ROTATE_STEP,OUTPUT);
  pinMode(PIN_ROTATE_DIR,OUTPUT);  

  pinMode(PIN_MOTOR_EN,OUTPUT);
  digitalWrite(PIN_MOTOR_EN,HIGH);

  Serial.begin(115200); // opens serial port, sets data rate to 115200 bps
  Serial.println("\n\nStarting coil winder\n");
  calibrateMotors();

  LCD_setup();

  // Set interrupt at 1ms
  TCCR0A=(1<<WGM01);    //Set the CTC mode   
  OCR0A=0xF9; //Value for ORC0A for 1ms 
  
  TIMSK0|=(1<<OCIE0A);   //Set the interrupt request  
  
  TCCR0B|=(1<<CS01);    //Set the prescale 1/64 clock
  TCCR0B|=(1<<CS00);

  sei(); //Enable interrupt  
}

void loop() {
  if(processLcdCounter > 100){
    // Process LCD every 100ms to avoid unnecessary delays
    LCD_process();
  }
}
