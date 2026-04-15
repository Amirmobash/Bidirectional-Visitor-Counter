#include <LiquidCrystal_I2C.h>  
// Initialize the LCD with I2C address 0x27  
LiquidCrystal_I2C lcd(0x27, 16, 2);  
const int irPin1 = 2; // IR sensor 1(Entrance Detector) connected to digital pin 2  
const int irPin2 = 3; // IR sensor 2(Exit Detector) connected to digital pin 3  
int in_count = 0;        // Variable to store the number of people went inside  
int out_count = 0;       // Variable to store the number of people went outside  
int current_count = 0;   // Variable to store the current number of people present inside  
const unsigned long timeout = 50; // Object detection Timeout period in milliseconds  
void setup() {  
	 /* Initialise LCD Display */  
	 lcd.init();  
	 lcd.backlight();  
	 lcd.setCursor(0, 0);  
	 lcd.print("IN: 0   OUT: 0");  
	 lcd.setCursor(0, 1);  
	 lcd.print("Current: 0");  
	  
	 /* Pull up the input pins */  
	 pinMode(irPin1, INPUT_PULLUP);  
	 pinMode(irPin2, INPUT_PULLUP);  
}  
void loop() {  
	 /*  Check if the first sensor is triggered */  
	 if (digitalRead(irPin1) == LOW) {  
	   unsigned long startTime = millis();  
	   while ((millis() - startTime) < timeout) {  
	     if(digitalRead(irPin2) == LOW){  
	        ++in_count;  
	        updateDisplay();  
	        break;  
	     }  
	   }  
	   //wait until both sensors return to a normal state  
	   while(!digitalRead(irPin1) || !digitalRead(irPin2));  
	 }  
	   
	 /* Check if the second sensor is triggered */  
	 else if (digitalRead(irPin2) == LOW) {  
	   unsigned long startTime = millis();  
	   while ((millis() - startTime) < timeout) {  
	     if(digitalRead(irPin1) == LOW){  
	       if(out_count < in_count){  
	          ++out_count;  
	          updateDisplay();  
	          break;  
	       }  
	     }  
	   }  
	   //wait until both sensors return to a normal state  
	   while(!digitalRead(irPin1) || !digitalRead(irPin2));  
	 }  
	  
}  
void updateDisplay() {  
	 lcd.setCursor(4, 0);  
	 lcd.print("   ");  
	 lcd.setCursor(4, 0);  
	 lcd.print(in_count);  
	 lcd.setCursor(13, 0);  
	 lcd.print("   ");  
	 lcd.setCursor(13, 0);  
	 lcd.print(out_count);  
	 current_count = in_count - out_count;  
	 lcd.setCursor(9, 1);  
	 lcd.print("   ");  
	 lcd.setCursor(9, 1);  
	 lcd.print(current_count);  
}
