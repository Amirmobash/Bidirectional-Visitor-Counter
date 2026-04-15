#include <LiquidCrystal_I2C.h>

// Initialisiere das LCD mit I2C-Adresse 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int irPin1 = 2; // IR-Sensor 1 (Eingangsdetektor) an Pin 2
const int irPin2 = 3; // IR-Sensor 2 (Ausgangsdetektor) an Pin 3

int in_count = 0;        // Anzahl der hereingegangenen Personen
int out_count = 0;       // Anzahl der hinausgegangenen Personen
int current_count = 0;   // Aktuelle Anzahl der Personen im Raum

const unsigned long timeout = 500; // Entprellzeit in Millisekunden
bool sensor1Getriggert = false;
bool sensor2Getriggert = false;

void setup() {
  Serial.begin(9600);
  
  // LCD initialisieren
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("IN:0   OUT:0");
  lcd.setCursor(0, 1);
  lcd.print("Aktuell:0   ");
  
  // Eingangspins mit Pull-up setzen
  pinMode(irPin1, INPUT_PULLUP);
  pinMode(irPin2, INPUT_PULLUP);
  
  delay(1000); // Warten bis Sensoren stabil sind
}

void loop() {
  int wert1 = digitalRead(irPin1);
  int wert2 = digitalRead(irPin2);
  
  // Person betritt: Sensor 1 zuerst, dann Sensor 2
  if (wert1 == LOW && !sensor1Getriggert) {
    sensor1Getriggert = true;
    unsigned long startZeit = millis();
    
    while ((millis() - startZeit) < timeout) {
      if (digitalRead(irPin2) == LOW) {
        // Person eingetreten
        in_count++;
        current_count = in_count - out_count;
        updateDisplay();
        delay(200); // Entprellverzögerung
        break;
      }
    }
  }
  
  // Person verlässt: Sensor 2 zuerst, dann Sensor 1
  if (wert2 == LOW && !sensor2Getriggert) {
    sensor2Getriggert = true;
    unsigned long startZeit = millis();
    
    while ((millis() - startZeit) < timeout) {
      if (digitalRead(irPin1) == LOW) {
        // Person verlässt, aber nur wenn jemand im Raum ist
        if (current_count > 0) {
          out_count++;
          current_count = in_count - out_count;
          updateDisplay();
          delay(200); // Entprellverzögerung
        }
        break;
      }
    }
  }
  
  // Sensorzustände zurücksetzen wenn sie HIGH werden
  if (wert1 == HIGH) {
    sensor1Getriggert = false;
  }
  if (wert2 == HIGH) {
    sensor2Getriggert = false;
  }
  
  delay(10); // Kleine Verzögerung zur Vermeidung von Endlosschleifen
}

void updateDisplay() {
  // IN Anzahl aktualisieren
  lcd.setCursor(4, 0);
  lcd.print("   "); // Alten Wert löschen
  lcd.setCursor(4, 0);
  lcd.print(in_count);
  
  // OUT Anzahl aktualisieren
  lcd.setCursor(13, 0);
  lcd.print("   "); // Alten Wert löschen
  lcd.setCursor(13, 0);
  lcd.print(out_count);
  
  // AKTUELLE Anzahl aktualisieren
  lcd.setCursor(9, 1);
  lcd.print("   "); // Alten Wert löschen
  lcd.setCursor(9, 1);
  lcd.print(current_count);
  
  // Für Debugging auf Seriellen Monitor ausgeben
  Serial.print("Eingang: ");
  Serial.print(in_count);
  Serial.print(" | Ausgang: ");
  Serial.print(out_count);
  Serial.print(" | Aktuell: ");
  Serial.println(current_count);
}
