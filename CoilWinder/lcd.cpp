#include <Arduino.h>
#include <LiquidCrystal.h>
#include "config.h"

LiquidCrystal lcd( PIN_LCD_RS,  PIN_LCD_EN,  PIN_LCD_D4,  PIN_LCD_D5,  PIN_LCD_D6,  PIN_LCD_D7);

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
