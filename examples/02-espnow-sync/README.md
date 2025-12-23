# ESP-NOW Synchronized Show Control

Ultra-fast synchronization between multiple ESP32 devices using ESP-NOW protocol.

## 🚀 What is ESP-NOW?

ESP-NOW is a connectionless WiFi protocol developed by Espressif that allows multiple devices to communicate without a router. It's:
- **Fast**: <20ms latency
- **Efficient**: Low power consumption
- **Simple**: No network setup needed
- **Reliable**: Direct device-to-device communication

## 📦 Hardware Required

- 2 or more ESP32 development boards
- USB cables for programming
- Built-in LED (GPIO 2)

## ⚙️ Setup

### Step 1: Configure Master Device

1. Open `espnow_sync.cpp`
2. Set `#define IS_MASTER true`
3. Upload to first ESP32

### Step 2: Configure Slave Devices

1. Open `espnow_sync.cpp`
2. Set `#define IS_MASTER false`
3. Upload to other ESP32 boards

### Step 3: Find MAC Addresses

Open serial monitor on each device (115200 baud). You'll see:
```
📱 MAC Address: XX:XX:XX:XX:XX:XX
🎭 Role: MASTER (or SLAVE)
```

## 🎮 Usage

### Master Device Commands

Open serial monitor (115200 baud) and type:

- `start` - Start the show (all devices begin blinking)
- `stop` - Stop the show (all LEDs turn off)
- `on` - Turn all LEDs on
- `off` - Turn all LEDs off
- `scene1` - Switch to Scene 1 pattern
- `scene2` - Switch to Scene 2 pattern
- `scene3` - Switch to Scene 3 pattern
- `pattern0` - Off
- `pattern1` - Slow blink (1 second)
- `pattern2` - Fast blink (200ms)
- `pattern3` - Strobe (50ms)

### What Happens

1. Type command on master's serial monitor
2. Master broadcasts command via ESP-NOW
3. **All slave devices receive and execute simultaneously**
4. Total latency: typically 10-20ms

## 🎭 TV Show Use Cases

### 1. Synchronized Stage Props
- Multiple props respond to same cue
- Perfect timing across stage
- No wiring between props

### 2. Wireless Cue System
- Stage manager sends cues from master device
- All performers/crew receive instantly
- Backup communication system

### 3. Coordinated Lighting
- All lights change together
- Scene transitions in perfect sync
- No delay between fixtures

### 4. Interactive Audience Effects
- Broadcast commands to hundreds of devices
- LED wristbands all flash at once
- Creates unified audience experience

### 5. Multi-Camera Sync
- Trigger multiple cameras simultaneously
- Synchronized video from different angles
- Perfect for bullet-time effects

## 🔧 Customization

### Change Broadcast to Specific Devices

Instead of broadcasting to all, target specific devices:

```cpp
// Get slave MAC address from serial monitor
uint8_t slaveAddress[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

// Register specific peer
esp_now_peer_info_t peerInfo;
memcpy(peerInfo.peer_addr, slaveAddress, 6);
esp_now_add_peer(&peerInfo);

// Send only to this device
esp_now_send(slaveAddress, (uint8_t *) &outgoingMsg, sizeof(outgoingMsg));
```

### Add More Commands

```cpp
} else if (strcmp(cmd, "CUSTOM_CMD") == 0) {
    // Your custom action
    Serial.println("🎪 Custom command executed!");
}
```

### Extend Message Structure

```cpp
typedef struct struct_message {
  char command[32];
  int value1;
  int value2;
  int value3;          // Add more fields
  float floatValue;    // Different data types
  bool boolValue;
} struct_message;
```

## 📊 Performance

- **Latency**: 10-20ms typical
- **Range**: Up to 250m line of sight
- **Throughput**: ~250 bytes per message
- **Max peers**: 20 encrypted, or more unencrypted

## 🐛 Troubleshooting

### "Error initializing ESP-NOW"
- ESP32 must be in WiFi Station mode
- Check that WiFi.mode(WIFI_STA) is called

### Devices not receiving
- Verify all devices on same WiFi channel
- Check MAC addresses in serial monitor
- Ensure devices are in range (start close together)

### High latency
- Reduce other WiFi traffic
- Check for interference
- Move devices closer

## 💡 Advanced Ideas

### 1. Time-Synchronized Effects
Add precise timing:
```cpp
outgoingMsg.executeAt = millis() + 1000; // Execute in 1 second
```

### 2. Acknowledgment System
Slaves confirm receipt:
```cpp
void OnDataRecv() {
  // ... receive command ...
  sendCommand("ACK"); // Send acknowledgment back
}
```

### 3. Bidirectional Communication
Both master and slaves can send:
```cpp
// Slave sends status update
sendCommand("STATUS", batteryLevel, signalStrength);
```

### 4. Group Addressing
Create device groups:
```cpp
int deviceGroup = 1; // Group A, B, C, etc.
if (incomingMsg.group == deviceGroup || incomingMsg.group == 0) {
  executeCommand(); // 0 = broadcast to all
}
```

## 🎬 Example Show Sequence

```
Type: start
[All devices] 🎬 SHOW STARTED!
[LEDs begin blinking in sync]

Type: scene1
[All devices] 🎭 Scene changed to: 1
[Pattern changes on all devices simultaneously]

Type: pattern3
[All devices] 🎨 Pattern changed to: 3
[All devices now strobing in perfect sync]

Type: stop
[All devices] ⏹️ SHOW STOPPED!
[All LEDs turn off]
```

## 🔗 Next Steps

- Add NeoPixel strips for colorful synchronized effects
- Integrate sensors (motion, sound) to trigger commands
- Create web interface for master device
- Add OLED displays to show received commands
- Implement mesh routing for extended range

## 📚 Resources

- [ESP-NOW Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_now.html)
- [Random Nerd Tutorials - ESP-NOW](https://randomnerdtutorials.com/esp-now-esp32-arduino-ide/)

---

**Experience the power of sub-20ms synchronized control!** ⚡✨
