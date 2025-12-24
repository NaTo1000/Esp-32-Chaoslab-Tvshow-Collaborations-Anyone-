/**
 * ESP Chas TV - Basic LED Control Example
 * 
 * This example demonstrates basic LED control for TV show effects.
 * The built-in LED creates different patterns that can be used
 * during a TV show.
 */

#include <Arduino.h>

const int LED_PIN = 2;  // Built-in LED on most ESP32 boards

// Pattern modes
enum Pattern {
  SLOW_BLINK,
  FAST_BLINK,
  PULSE,
  STROBE,
  OFF
};

Pattern currentPattern = SLOW_BLINK;

/**
 * Slow blink pattern (1 second interval)
 */
void slowBlink() {
  static unsigned long lastChange = 0;
  if (millis() - lastChange > 1000) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    lastChange = millis();
  }
}

/**
 * Fast blink pattern (200ms interval)
 */
void fastBlink() {
  static unsigned long lastChange = 0;
  if (millis() - lastChange > 200) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    lastChange = millis();
  }
}

/**
 * Pulse pattern (smooth fade in/out using PWM)
 */
void pulse() {
  static unsigned long lastUpdate = 0;
  static int brightness = 0;
  static int fadeAmount = 5;
  
  if (millis() - lastUpdate > 30) {
    analogWrite(LED_PIN, brightness);
    brightness = brightness + fadeAmount;
    
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount;
    }
    lastUpdate = millis();
  }
}

/**
 * Strobe pattern (fast on/off for dramatic effect)
 */
void strobe() {
  static unsigned long lastChange = 0;
  if (millis() - lastChange > 50) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    lastChange = millis();
  }
}

/**
 * Check for pattern change via Serial commands
 */
void checkSerial() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    
    switch (command) {
      case '1':
        currentPattern = SLOW_BLINK;
        Serial.println("Pattern: Slow Blink");
        break;
      case '2':
        currentPattern = FAST_BLINK;
        Serial.println("Pattern: Fast Blink");
        break;
      case '3':
        currentPattern = PULSE;
        Serial.println("Pattern: Pulse");
        break;
      case '4':
        currentPattern = STROBE;
        Serial.println("Pattern: Strobe");
        break;
      case '0':
        currentPattern = OFF;
        digitalWrite(LED_PIN, LOW);
        Serial.println("Pattern: Off");
        break;
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  
  Serial.println("\n🎬 ESP Chas TV - LED Control Example");
  Serial.println("=====================================");
  Serial.println("Commands:");
  Serial.println("  1 - Slow Blink");
  Serial.println("  2 - Fast Blink");
  Serial.println("  3 - Pulse (Fade)");
  Serial.println("  4 - Strobe");
  Serial.println("  0 - Off");
  Serial.println("=====================================\n");
}

void loop() {
  checkSerial();
  
  switch (currentPattern) {
    case SLOW_BLINK:
      slowBlink();
      break;
    case FAST_BLINK:
      fastBlink();
      break;
    case PULSE:
      pulse();
      break;
    case STROBE:
      strobe();
      break;
    case OFF:
      // Do nothing
      break;
  }
}
