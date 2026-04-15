#include <LiquidCrystal_I2C.h>

// Initialize the LCD with I2C address 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int irPin1 = 2; // IR sensor 1 (Entrance Detector)
const int irPin2 = 3; // IR sensor 2 (Exit Detector)

int in_count = 0;        // Number of people entered
int out_count = 0;       // Number of people exited
int current_count = 0;   // Current number of people inside

const unsigned long timeout = 500; // Debounce timeout in milliseconds
bool sensor1Triggered = false;
bool sensor2Triggered = false;

void setup() {
  Serial.begin(9600);
  
  // Initialize LCD Display
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("IN:0   OUT:0");
  lcd.setCursor(0, 1);
  lcd.print("Current:0   ");
  
  // Set input pins with pullup
  pinMode(irPin1, INPUT_PULLUP);
  pinMode(irPin2, INPUT_PULLUP);
  
  delay(1000); // Give sensors time to stabilize
}

void loop() {
  int reading1 = digitalRead(irPin1);
  int reading2 = digitalRead(irPin2);
  
  // Person entering: Sensor 1 triggered first, then Sensor 2
  if (reading1 == LOW && !sensor1Triggered) {
    sensor1Triggered = true;
    unsigned long startTime = millis();
    
    while ((millis() - startTime) < timeout) {
      if (digitalRead(irPin2) == LOW) {
        // Person entered
        in_count++;
        current_count = in_count - out_count;
        updateDisplay();
        delay(200); // Debounce delay
        break;
      }
    }
  }
  
  // Person exiting: Sensor 2 triggered first, then Sensor 1
  if (reading2 == LOW && !sensor2Triggered) {
    sensor2Triggered = true;
    unsigned long startTime = millis();
    
    while ((millis() - startTime) < timeout) {
      if (digitalRead(irPin1) == LOW) {
        // Person exited, but only if there's someone inside
        if (current_count > 0) {
          out_count++;
          current_count = in_count - out_count;
          updateDisplay();
          delay(200); // Debounce delay
        }
        break;
      }
    }
  }
  
  // Reset sensor states when they go HIGH
  if (reading1 == HIGH) {
    sensor1Triggered = false;
  }
  if (reading2 == HIGH) {
    sensor2Triggered = false;
  }
  
  delay(10); // Small delay to prevent excessive looping
}

void updateDisplay() {
  // Update IN count
  lcd.setCursor(4, 0);
  lcd.print("   "); // Clear old value
  lcd.setCursor(4, 0);
  lcd.print(in_count);
  
  // Update OUT count
  lcd.setCursor(13, 0);
  lcd.print("   "); // Clear old value
  lcd.setCursor(13, 0);
  lcd.print(out_count);
  
  // Update CURRENT count
  lcd.setCursor(9, 1);
  lcd.print("   "); // Clear old value
  lcd.setCursor(9, 1);
  lcd.print(current_count);
  
  // Print to Serial Monitor for debugging
  Serial.print("IN: ");
  Serial.print(in_count);
  Serial.print(" | OUT: ");
  Serial.print(out_count);
  Serial.print(" | CURRENT: ");
  Serial.println(current_count);
}
