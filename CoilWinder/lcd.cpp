#include <Arduino.h>
#include <LiquidCrystal.h>

const int pin_RS = 8; 
const int pin_EN = 9; 
const int pin_d4 = 4; 
const int pin_d5 = 5; 
const int pin_d6 = 6; 
const int pin_d7 = 7; 
const int pin_BL = 10; 
LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);

void LCD_setup() {
 lcd.begin(16, 2);
 lcd.setCursor(0,0);
 lcd.print("Coil winder");
 lcd.setCursor(0,1);
 lcd.print("Press Key:");
}
void LCD_process() {
 int x = analogRead(0);
 
 lcd.setCursor(10,1);
 if (x < 60) {
   lcd.print ("Right ");
 }
 else if (x < 200) {
   lcd.print ("Up    ");
 }
 else if (x < 400){
   lcd.print ("Down  ");
 }
 else if (x < 600){
   lcd.print ("Left  ");
 }
 else if (x < 800){
   lcd.print ("Select");
 }
}
