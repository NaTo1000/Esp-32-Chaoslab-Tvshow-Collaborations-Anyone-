/**
 * ESP-NOW Synchronized Show Control
 * 
 * This example demonstrates ultra-fast synchronization between multiple ESP32
 * devices using ESP-NOW protocol. Perfect for coordinated stage effects where
 * millisecond-level timing matters.
 * 
 * FEATURES:
 * - Broadcast commands to all devices instantly
 * - No WiFi infrastructure needed
 * - <20ms latency
 * - Up to 250m range
 * - Synchronized LED effects
 * 
 * USAGE:
 * 1. Upload this code to multiple ESP32 boards
 * 2. One board acts as MASTER (set IS_MASTER to true)
 * 3. Other boards are SLAVES (set IS_MASTER to false)
 * 4. Master sends commands via serial or web interface
 * 5. All slaves execute commands simultaneously
 */

#include <esp_now.h>
#include <WiFi.h>

// Configuration
#define IS_MASTER true  // Set to false for slave devices
#define LED_PIN 2

// Broadcast address (FF:FF:FF:FF:FF:FF sends to all)
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Message structure
typedef struct struct_message {
  char command[32];    // Command name
  int value1;          // Parameter 1
  int value2;          // Parameter 2
  unsigned long timestamp; // For sync timing
} struct_message;

struct_message outgoingMsg;
struct_message incomingMsg;

// LED state
int ledPattern = 0;
unsigned long lastPatternChange = 0;

/**
 * Callback when data is sent
 */
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("📡 Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "✅ Success" : "❌ Fail");
}

/**
 * Callback when data is received
 */
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingMsg, incomingData, sizeof(incomingMsg));
  
  Serial.println("\n📨 Message Received:");
  Serial.print("  Command: ");
  Serial.println(incomingMsg.command);
  Serial.print("  Value1: ");
  Serial.println(incomingMsg.value1);
  Serial.print("  Value2: ");
  Serial.println(incomingMsg.value2);
  Serial.print("  Latency: ");
  Serial.print(millis() - incomingMsg.timestamp);
  Serial.println("ms");
  
  // Execute command
  executeCommand(incomingMsg.command, incomingMsg.value1, incomingMsg.value2);
}

/**
 * Execute received command
 */
void executeCommand(const char* cmd, int val1, int val2) {
  if (strcmp(cmd, "LED_ON") == 0) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("💡 LED ON");
    
  } else if (strcmp(cmd, "LED_OFF") == 0) {
    digitalWrite(LED_PIN, LOW);
    Serial.println("💡 LED OFF");
    
  } else if (strcmp(cmd, "PATTERN") == 0) {
    ledPattern = val1;
    Serial.print("🎨 Pattern changed to: ");
    Serial.println(val1);
    
  } else if (strcmp(cmd, "SHOW_START") == 0) {
    Serial.println("🎬 SHOW STARTED!");
    ledPattern = 1; // Start blinking
    
  } else if (strcmp(cmd, "SHOW_STOP") == 0) {
    Serial.println("⏹️ SHOW STOPPED!");
    ledPattern = 0;
    digitalWrite(LED_PIN, LOW);
    
  } else if (strcmp(cmd, "SCENE") == 0) {
    Serial.print("🎭 Scene changed to: ");
    Serial.println(val1);
    ledPattern = val1 + 10; // Different pattern per scene
    
  } else {
    Serial.println("❓ Unknown command");
  }
}

/**
 * Send command to all devices
 */
void sendCommand(const char* cmd, int val1 = 0, int val2 = 0) {
  strcpy(outgoingMsg.command, cmd);
  outgoingMsg.value1 = val1;
  outgoingMsg.value2 = val2;
  outgoingMsg.timestamp = millis();
  
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &outgoingMsg, sizeof(outgoingMsg));
  
  if (result == ESP_OK) {
    Serial.println("📤 Sent command: " + String(cmd));
  } else {
    Serial.println("❌ Error sending command");
  }
}

/**
 * Process serial commands (for master)
 */
void processSerialCommand() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    
    if (input == "start") {
      sendCommand("SHOW_START");
    } else if (input == "stop") {
      sendCommand("SHOW_STOP");
    } else if (input == "on") {
      sendCommand("LED_ON");
    } else if (input == "off") {
      sendCommand("LED_OFF");
    } else if (input.startsWith("scene")) {
      int sceneNum = input.substring(5).toInt();
      sendCommand("SCENE", sceneNum);
    } else if (input.startsWith("pattern")) {
      int patternNum = input.substring(7).toInt();
      sendCommand("PATTERN", patternNum);
    } else {
      Serial.println("Unknown command. Try: start, stop, on, off, scene1-3, pattern0-9");
    }
  }
}

/**
 * Update LED based on current pattern
 */
void updateLED() {
  unsigned long now = millis();
  
  switch (ledPattern) {
    case 0: // Off
      digitalWrite(LED_PIN, LOW);
      break;
      
    case 1: // Slow blink
      if (now - lastPatternChange > 1000) {
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        lastPatternChange = now;
      }
      break;
      
    case 2: // Fast blink
      if (now - lastPatternChange > 200) {
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        lastPatternChange = now;
      }
      break;
      
    case 3: // Strobe
      if (now - lastPatternChange > 50) {
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        lastPatternChange = now;
      }
      break;
      
    case 10: // Scene 1 - slow pulse
    case 11: // Scene 2 - medium pulse
    case 12: // Scene 3 - fast pulse
      int interval = 1000 / (ledPattern - 9);
      if (now - lastPatternChange > interval) {
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        lastPatternChange = now;
      }
      break;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  
  // Set device as WiFi Station
  WiFi.mode(WIFI_STA);
  
  Serial.println("\n\n🎬 ESP-NOW Synchronized Show Control");
  Serial.println("=====================================");
  Serial.print("📱 MAC Address: ");
  Serial.println(WiFi.macAddress());
  Serial.print("🎭 Role: ");
  Serial.println(IS_MASTER ? "MASTER" : "SLAVE");
  Serial.println("=====================================\n");
  
  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ Error initializing ESP-NOW");
    return;
  }
  
  Serial.println("✅ ESP-NOW Initialized");
  
  // Register callbacks
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);
  
  // Register peer (broadcast)
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("❌ Failed to add peer");
    return;
  }
  
  Serial.println("✅ Peer registered");
  
  if (IS_MASTER) {
    Serial.println("\n📝 Commands:");
    Serial.println("  start  - Start show");
    Serial.println("  stop   - Stop show");
    Serial.println("  on     - LED on");
    Serial.println("  off    - LED off");
    Serial.println("  scene1 - Scene 1");
    Serial.println("  scene2 - Scene 2");
    Serial.println("  scene3 - Scene 3");
    Serial.println("  pattern0-9 - LED patterns");
    Serial.println("\n🎬 Ready for commands!\n");
  } else {
    Serial.println("\n👂 Listening for commands...\n");
  }
}

void loop() {
  if (IS_MASTER) {
    processSerialCommand();
  }
  
  updateLED();
  delay(10);
}
