markdown
# Personen-Zählsystem mit Arduino und LCD

**Autor:** Amir Mobasheraghdam  
**Datum:** 2026  
**Version:** 1.0  
**Lizenz:** MIT

---

## 📋 Inhaltsverzeichnis

1. [Projektbeschreibung](#projektbeschreibung)
2. [Benötigte Komponenten](#benötigte-komponenten)
3. [Schaltplan](#schaltplan)
4. [Pin-Belegung](#pin-belegung)
5. [Code-Erklärung](#code-erklärung)
6. [Installation](#installation)
7. [Bedienungsanleitung](#bedienungsanleitung)
8. [Fehlerbehebung](#fehlerbehebung)
9. [Technische Daten](#technische-daten)

---

## Projektbeschreibung

Dieses Projekt implementiert ein automatisches Personen-Zählsystem mit zwei IR-Sensoren und einem I2C-LCD-Display. Das System erkennt Personen, die einen Raum durch einen Eingang oder Ausgang passieren, und zeigt die aktuelle Anzahl der Personen im Raum an.

**Funktionsweise:**
- **Eingang**: Person löst zuerst Sensor 1, dann Sensor 2 aus
- **Ausgang**: Person löst zuerst Sensor 2, dann Sensor 1 aus
- Das Display zeigt: Anzahl der Eintritte, Austritte und aktuell anwesende Personen

---

## Benötigte Komponenten

| Komponente | Anzahl | Beschreibung |
|------------|--------|--------------|
| Arduino Board (Uno/Nano/Mega) | 1 | Mikrocontroller |
| IR-Sensor (z.B. FC-51) | 2 | Hinderniserkennung |
| I2C LCD Display (16x2) | 1 | Anzeige der Werte |
| Breadboard | 1 | Prototyping-Plattform |
| Jumper-Kabel (M/M) | 10+ | Verbindungskabel |
| USB-Kabel | 1 | Stromversorgung & Programmierung |

---

## Schaltplan

```
Arduino                    IR-Sensor 1 (Eingang)
--------                   ---------------------
Pin 2   -----------------> OUT (Signal)
5V      -----------------> VCC
GND     -----------------> GND

Arduino                    IR-Sensor 2 (Ausgang)
--------                   ---------------------
Pin 3   -----------------> OUT (Signal)
5V      -----------------> VCC
GND     -----------------> GND

Arduino                    I2C LCD (16x2)
--------                   -------------
A4 (SDA) -----------------> SDA
A5 (SCL) -----------------> SCL
5V      -----------------> VCC
GND     -----------------> GND
```

**Hinweis:** Bei Arduino Mega verwenden Sie Pin 20 (SDA) und Pin 21 (SCL).

---

## Pin-Belegung

| Komponente | Arduino Pin | Funktion |
|------------|-------------|----------|
| IR-Sensor 1 (Eingang) | D2 | Signal-Eingang |
| IR-Sensor 2 (Ausgang) | D3 | Signal-Eingang |
| I2C LCD SDA | A4 | Datenleitung |
| I2C LCD SCL | A5 | Clock-Leitung |

---

## Code-Erklärung

### Bibliotheken
```cpp
#include <LiquidCrystal_I2C.h>  // I2C LCD Steuerung
```

### Variablen
- `in_count`: Anzahl der eingetretenen Personen
- `out_count`: Anzahl der ausgetretenen Personen  
- `current_count`: Aktuelle Personen im Raum
- `timeout`: Entprellzeit (500ms)

### Hauptlogik
1. **setup()** - Initialisiert LCD, Sensoren und serielle Kommunikation
2. **loop()** - Überwacht kontinuierlich beide Sensoren
3. **updateDisplay()** - Aktualisiert die LCD-Anzeige

### Wichtige Funktionen
- **Entprellung**: Verhindert mehrfache Zählung durch Prell-Effekte
- **Plausibilitätsprüfung**: Verhindert negative Personenanzahlen
- **Debug-Ausgabe**: Serielle Monitor Ausgabe für Fehlersuche

---

## Installation

### 1. Arduino IDE einrichten
- Laden Sie die [Arduino IDE](https://www.arduino.cc/en/software) herunter
- Installieren Sie die IDE auf Ihrem Computer

### 2. Benötigte Bibliothek installieren
- Öffnen Sie: `Sketch` → `Bibliothek einbinden` → `Bibliotheken verwalten...`
- Suchen Sie nach: `LiquidCrystal I2C`
- Installieren Sie die Bibliothek von **Frank de Brabander**

### 3. Code hochladen
```bash
1. Schließen Sie das Arduino an Ihren Computer an
2. Öffnen Sie die Arduino IDE
3. Kopieren Sie den kompletten Code in ein neues Sketch
4. Wählen Sie den richtigen Board-Typ: Werkzeuge → Board → Arduino Uno
5. Wählen Sie den richtigen COM-Port
6. Klicken Sie auf "Hochladen" (→ Pfeil-Symbol)
```

### 4. Seriellen Monitor öffnen
- Klicken Sie auf das Symbol rechts oben (Lupe)
- Stellen Sie die Baudrate auf **9600** ein

---

## Bedienungsanleitung

### Normaler Betrieb
1. **Stromversorgung** anschließen (USB oder extern)
2. Das LCD zeigt: `IN:0   OUT:0` und `Aktuell:0`
3. Wenn eine Person den **Eingang** passiert:
   - IN erhöht sich um 1
   - AKTUELL erhöht sich um 1
4. Wenn eine Person den **Ausgang** passiert:
   - OUT erhöht sich um 1
   - AKTUELL verringert sich um 1

### Sensor-Positionierung
- **Eingangssensor** (Pin 2) - Am Eingang montieren
- **Ausgangssensor** (Pin 3) - Am Ausgang montieren
- Abstand zwischen den Sensoren: 10-30 cm
- Montagehöhe: ca. 1 Meter (Körpermitte)

---

## Fehlerbehebung

| Problem | Mögliche Ursache | Lösung |
|---------|-----------------|--------|
| LCD bleibt dunkel | Falsche I2C-Adresse | I2C-Scanner Sketch verwenden |
| | Kontrast zu niedrig | Potentiometer auf LCD-Rückseite drehen |
| | Kabel falsch verbunden | Schaltplan prüfen |
| Keine Zählung | Sensoren falsch ausgerichtet | Ausrichtung prüfen |
| | Entfernung zu groß | Sensoren näher anbringen |
| | Stromversorgung zu schwach | Externe Stromquelle verwenden |
| Mehrfachzählung | Entprellzeit zu kurz | timeout auf 500ms erhöhen |
| | Reflexionen | Sensoren schräg montieren |
| Negative Anzeige | Fehlerhafte Sensorlogik | Sensoren vertauscht? |

### I2C-Adresse finden
Laden Sie folgenden Sketch hoch:
```cpp
#include <Wire.h>
void setup() {
  Wire.begin();
  Serial.begin(9600);
  for(byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if(Wire.endTransmission() == 0) {
      Serial.print("Gefunden: 0x");
      Serial.println(addr, HEX);
    }
  }
}
void loop() {}
```

---

## Technische Daten

| Parameter | Wert |
|-----------|------|
| Betriebsspannung | 5V DC |
| Stromverbrauch | ~50mA |
| Sensor-Reichweite | 2-30 cm (einstellbar) |
| Entprellzeit | 500 ms |
| Maximale Zählrate | ~2 Personen/Sekunde |
| LCD-Größe | 16x2 Zeichen |
| I2C-Adresse (Standard) | 0x27 |
| Programmgröße | ~4 KB |

---

## Mögliche Erweiterungen

- **Datenspeicherung** mit EEPROM (bei Stromausfall)
- **Maximum-Warnung** mit LED oder Summer
- **Datenlogger** mit SD-Karten-Modul
- **Fernüberwachung** via Bluetooth/WLAN
- **Türsteuerung** mit Servomotor

---

## Danksagung

- **Autor**: Amir Mobasheraghdam
- **Erstellungsdatum**: April 2026
- **Version**: 1.0
- **Kontakt**: Bei Fragen zum Code bitte Issue auf GitHub erstellen

---

## Lizenz

MIT License - Freie Verwendung, Modifikation und Verbreitung erlaubt.

---

**Viel Erfolg mit Ihrem Personen-Zählsystem!** 🚪📊
```

Diese README-Datei enthält:
- ✅ Ihren Namen **Amir Mobasheraghdam** als Autor im Metadata-Bereich
- ✅ Vollständige deutsche Beschreibung
- ✅ Schaltplan und Pin-Belegung
- ✅ Installationsanleitung
- ✅ Fehlerbehebung
- ✅ Technische Daten
- ✅ Erweiterungsmöglichkeiten

Speichern Sie diese Datei als `README.md` im selben Ordner wie Ihren Arduino-Code.
