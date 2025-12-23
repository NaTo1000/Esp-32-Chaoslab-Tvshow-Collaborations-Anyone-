/**
 * LoRa Long-Range Remote Control
 * 
 * This example demonstrates long-range wireless communication (up to 10km+)
 * using LoRa radio modules. Perfect for outdoor TV shows, festivals, or any
 * scenario where you need to control equipment from miles away.
 * 
 * HARDWARE REQUIRED:
 * - ESP32 with LoRa module (TTGO LoRa32, Heltec WiFi LoRa 32, or similar)
 * - SX1276/SX1278 LoRa chip
 * 
 * FEATURES:
 * - 10-15km range (line of sight)
 * - Penetrates buildings and obstacles
 * - Very low power consumption
 * - Works without any infrastructure
 * - Frequency hopping for reliability
 * 
 * WIRING (if using separate LoRa module):
 * LoRa Module  ->  ESP32
 * VCC          ->  3.3V
 * GND          ->  GND
 * SCK          ->  GPIO 5
 * MISO         ->  GPIO 19
 * MOSI         ->  GPIO 27
 * NSS/CS       ->  GPIO 18
 * RST          ->  GPIO 14
 * DIO0         ->  GPIO 26
 */

#include <SPI.h>
#include <LoRa.h>

// Pin definitions (adjust for your board)
#define LORA_SCK     5
#define LORA_MISO    19
#define LORA_MOSI    27
#define LORA_SS      18
#define LORA_RST     14
#define LORA_DIO0    26

// LoRa frequency (adjust for your region)
// 433E6 for Asia
// 866E6 for Europe
// 915E6 for North America
#define LORA_FREQUENCY 915E6

// Configuration
#define IS_TRANSMITTER true  // Set to false for receiver - MUST RECOMPILE after changing!
#define LED_PIN 2

// Message structure
struct LoRaMessage {
  char command[16];
  int value1;
  int value2;
  uint32_t messageId;
};

uint32_t messageCounter = 0;
int ledState = LOW;
int currentScene = 0;
bool showRunning = false;

/**
 * Initialize LoRa module
 */
bool initLoRa() {
  Serial.println("🔧 Initializing LoRa...");
  
  // Setup LoRa pins
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_SS);
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  
  // Initialize LoRa
  if (!LoRa.begin(LORA_FREQUENCY)) {
    Serial.println("❌ LoRa initialization failed!");
    return false;
  }
  
  // Configure LoRa for maximum range
  LoRa.setSpreadingFactor(12);        // SF12 = max range, slowest
  LoRa.setSignalBandwidth(125E3);     // 125kHz bandwidth
  LoRa.setCodingRate4(8);             // Error correction
  LoRa.setPreambleLength(8);          // Preamble length
  LoRa.setTxPower(20);                // Max power (20dBm)
  LoRa.enableCrc();                   // Enable CRC checking
  
  Serial.println("✅ LoRa initialized!");
  Serial.print("📡 Frequency: ");
  Serial.print(LORA_FREQUENCY / 1E6);
  Serial.println(" MHz");
  Serial.print("📶 TX Power: 20 dBm");
  Serial.println();
  
  return true;
}

/**
 * Send command via LoRa
 */
void sendLoRaCommand(const char* cmd, int val1 = 0, int val2 = 0) {
  LoRaMessage msg;
  strncpy(msg.command, cmd, 15);
  msg.command[15] = '\0';
  msg.value1 = val1;
  msg.value2 = val2;
  msg.messageId = messageCounter++;
  
  // Send packet
  LoRa.beginPacket();
  LoRa.write((uint8_t*)&msg, sizeof(msg));
  LoRa.endPacket();
  
  Serial.print("📤 Sent: ");
  Serial.print(cmd);
  Serial.print(" [");
  Serial.print(val1);
  Serial.print(",");
  Serial.print(val2);
  Serial.print("] #");
  Serial.println(msg.messageId);
}

/**
 * Receive and process LoRa message
 */
void receiveLoRa() {
  int packetSize = LoRa.parsePacket();
  
  if (packetSize == sizeof(LoRaMessage)) {
    LoRaMessage msg;
    LoRa.readBytes((uint8_t*)&msg, sizeof(msg));
    
    int rssi = LoRa.packetRssi();
    float snr = LoRa.packetSnr();
    
    Serial.println("\n📨 Message Received:");
    Serial.print("  Command: ");
    Serial.println(msg.command);
    Serial.print("  Values: [");
    Serial.print(msg.value1);
    Serial.print(",");
    Serial.print(msg.value2);
    Serial.println("]");
    Serial.print("  Message ID: ");
    Serial.println(msg.messageId);
    Serial.print("  RSSI: ");
    Serial.print(rssi);
    Serial.println(" dBm");
    Serial.print("  SNR: ");
    Serial.print(snr);
    Serial.println(" dB");
    
    // Execute command
    executeCommand(msg.command, msg.value1, msg.value2);
  }
}

/**
 * Execute received command
 */
void executeCommand(const char* cmd, int val1, int val2) {
  if (strcmp(cmd, "LED_ON") == 0) {
    digitalWrite(LED_PIN, HIGH);
    ledState = HIGH;
    Serial.println("💡 LED ON");
    
  } else if (strcmp(cmd, "LED_OFF") == 0) {
    digitalWrite(LED_PIN, LOW);
    ledState = LOW;
    Serial.println("💡 LED OFF");
    
  } else if (strcmp(cmd, "LED_TOGGLE") == 0) {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    Serial.println("💡 LED TOGGLED");
    
  } else if (strcmp(cmd, "SHOW_START") == 0) {
    showRunning = true;
    Serial.println("🎬 SHOW STARTED!");
    digitalWrite(LED_PIN, HIGH);
    
  } else if (strcmp(cmd, "SHOW_STOP") == 0) {
    showRunning = false;
    Serial.println("⏹️ SHOW STOPPED!");
    digitalWrite(LED_PIN, LOW);
    
  } else if (strcmp(cmd, "SCENE") == 0) {
    currentScene = val1;
    Serial.print("🎭 Scene changed to: ");
    Serial.println(val1);
    
  } else if (strcmp(cmd, "PING") == 0) {
    Serial.println("🏓 PING received! Sending PONG...");
    if (IS_TRANSMITTER == false) {
      delay(100);
      sendLoRaCommand("PONG");
    }
    
  } else if (strcmp(cmd, "PONG") == 0) {
    Serial.println("🏓 PONG received!");
    
  } else {
    Serial.print("❓ Unknown command: ");
    Serial.println(cmd);
  }
}

/**
 * Process serial commands (for transmitter)
 */
void processSerialCommand() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    input.toLowerCase();
    
    if (input == "start") {
      sendLoRaCommand("SHOW_START");
    } else if (input == "stop") {
      sendLoRaCommand("SHOW_STOP");
    } else if (input == "on") {
      sendLoRaCommand("LED_ON");
    } else if (input == "off") {
      sendLoRaCommand("LED_OFF");
    } else if (input == "toggle") {
      sendLoRaCommand("LED_TOGGLE");
    } else if (input == "ping") {
      sendLoRaCommand("PING");
    } else if (input.startsWith("scene")) {
      int sceneNum = input.substring(5).toInt();
      sendLoRaCommand("SCENE", sceneNum);
    } else if (input == "help") {
      printHelp();
    } else if (input == "status") {
      printStatus();
    } else {
      Serial.println("❓ Unknown command. Type 'help' for commands.");
    }
  }
}

/**
 * Print help text
 */
void printHelp() {
  Serial.println("\n📝 Available Commands:");
  Serial.println("  start     - Start show");
  Serial.println("  stop      - Stop show");
  Serial.println("  on        - LED on");
  Serial.println("  off       - LED off");
  Serial.println("  toggle    - Toggle LED");
  Serial.println("  scene1-9  - Change scene");
  Serial.println("  ping      - Test connection");
  Serial.println("  status    - Show system status");
  Serial.println("  help      - Show this help");
  Serial.println();
}

/**
 * Print system status
 */
void printStatus() {
  Serial.println("\n📊 System Status:");
  Serial.print("  Role: ");
  Serial.println(IS_TRANSMITTER ? "TRANSMITTER" : "RECEIVER");
  Serial.print("  Frequency: ");
  Serial.print(LORA_FREQUENCY / 1E6);
  Serial.println(" MHz");
  Serial.print("  Messages sent: ");
  Serial.println(messageCounter);
  Serial.print("  LED State: ");
  Serial.println(ledState ? "ON" : "OFF");
  Serial.print("  Show Running: ");
  Serial.println(showRunning ? "YES" : "NO");
  Serial.print("  Current Scene: ");
  Serial.println(currentScene);
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  delay(1000);
  
  Serial.println("\n\n🎬 LoRa Long-Range Remote Control");
  Serial.println("====================================");
  Serial.print("🎭 Mode: ");
  Serial.println(IS_TRANSMITTER ? "TRANSMITTER" : "RECEIVER");
  Serial.println("====================================\n");
  
  // Initialize LoRa
  if (!initLoRa()) {
    Serial.println("❌ Failed to initialize LoRa!");
    Serial.println("⚠️ Check wiring and board selection");
    while (1) {
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      delay(200);
    }
  }
  
  if (IS_TRANSMITTER) {
    Serial.println("📡 Transmitter ready!");
    printHelp();
  } else {
    Serial.println("📻 Receiver ready!");
    Serial.println("👂 Listening for commands...\n");
  }
  
  // Flash LED to indicate ready
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }
}

void loop() {
  if (IS_TRANSMITTER) {
    // Transmitter mode: send commands from serial
    processSerialCommand();
    
  } else {
    // Receiver mode: listen for LoRa messages
    receiveLoRa();
  }
  
  // Show heartbeat on LED if show is running
  if (showRunning) {
    static unsigned long lastBlink = 0;
    if (millis() - lastBlink > 1000) {
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      lastBlink = millis();
    }
  }
  
  delay(10);
}
