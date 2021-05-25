#ifndef CONFIG_H
#define CONFIG_H

#define PIN_SLIDE_STEP        2
#define PIN_SLIDE_DIR         5
#define PIN_ROTATE_STEP       3
#define PIN_ROTATE_DIR        6
#define PIN_MOTOR_EN          8

#define PIN_LCD_RS            8
#define PIN_LCD_EN            9
#define PIN_LCD_D4            4
#define PIN_LCD_D5            5
#define PIN_LCD_D6            6
#define PIN_LCD_D7            7
#define PIN_LCD_BL            10

// Maximum slider steps to sweep maximum coil length.
#define maxSliderSteps        (3600ul)

// Maximum coil length in um
#define maxCoilLength         (88000ul)

// Number of steps to make for one coil turn
#define rotaryMotorStepsPerTurn (200ul)    

#endif
