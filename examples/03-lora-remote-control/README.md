# LoRa Long-Range Remote Control

Control ESP32 devices from kilometers away using LoRa (Long Range) radio technology.

## 🌍 What is LoRa?

LoRa is a long-range, low-power wireless protocol that can transmit data up to **10-15km** in optimal conditions. Unlike WiFi or Bluetooth:
- Works without any infrastructure
- Penetrates buildings and obstacles
- Extremely low power consumption
- Perfect for outdoor and remote applications

## 📦 Hardware Required

### Option 1: Integrated Boards (Recommended)
- **TTGO LoRa32 V2.1** (~$15-20)
- **Heltec WiFi LoRa 32 V2** (~$15-20)
- **LILYGO T-Beam** (~$30-40, includes GPS)

These boards have ESP32 + LoRa module built-in!

### Option 2: Separate Modules
- ESP32 development board
- SX1276 or SX1278 LoRa module (~$5-10)
- Jumper wires
- Antenna (comes with module)

## 🔌 Wiring (for separate modules)

```
LoRa Module    ESP32
-----------    -----
VCC         -> 3.3V
GND         -> GND
SCK         -> GPIO 5
MISO        -> GPIO 19
MOSI        -> GPIO 27
NSS (CS)    -> GPIO 18
RST         -> GPIO 14
DIO0        -> GPIO 26
```

## ⚙️ Setup

### Step 1: Install Library

In PlatformIO, add to `platformio.ini`:
```ini
lib_deps = 
    sandeepmistry/LoRa@^0.8.0
```

Or in Arduino IDE:
- Sketch → Include Library → Manage Libraries
- Search for "LoRa by Sandeep Mistry"
- Install

### Step 2: Set Frequency

Edit `lora_remote.cpp`:
```cpp
#define LORA_FREQUENCY 915E6  // Change based on your region:
// 433E6 for Asia
// 866E6 for Europe  
// 915E6 for North America
```

⚠️ **Important**: Check local regulations for your frequency!

### Step 3: Configure Roles

**Transmitter (Remote Control)**:
```cpp
#define IS_TRANSMITTER true
```

**Receiver (Device being controlled)**:
```cpp
#define IS_TRANSMITTER false
```

### Step 4: Upload

Upload to both boards with appropriate settings.

## 🎮 Usage

### Transmitter Commands

Open serial monitor (115200 baud) and type:

- `start` - Start the show
- `stop` - Stop the show
- `on` - Turn LED on
- `off` - Turn LED off
- `toggle` - Toggle LED
- `scene1` through `scene9` - Change scenes
- `ping` - Test connection (receiver will respond)
- `status` - Show system information
- `help` - Show command list

### Example Session

```
Type: ping
📤 Sent: PING [0,0] #1

[Receiver responds]
📨 Message Received:
  Command: PONG
  RSSI: -89 dBm
  SNR: 7.5 dB
```

## 📊 Understanding Signal Strength

### RSSI (Received Signal Strength Indicator)
- **-50 dBm**: Excellent (close range)
- **-80 dBm**: Good (several hundred meters)
- **-100 dBm**: Fair (1-2km)
- **-120 dBm**: Poor but workable (long range)

### SNR (Signal to Noise Ratio)
- **+10 dB**: Excellent
- **0 dB**: Good
- **-10 dB**: Fair (LoRa can work here!)
- **-20 dB**: Poor (limit of LoRa)

## 🎭 TV Show Use Cases

### 1. Outdoor Festival Stages
Control equipment on stage from production tent hundreds of meters away.

### 2. Drone Camera Control
Trigger camera drones from ground control, even beyond visual range.

### 3. Remote Pyrotechnics
Safe distance triggering of pyrotechnic effects (10+ meters away).

### 4. Wilderness Productions
Film in remote locations without cellular or WiFi coverage.

### 5. Backup Communication
When WiFi fails, LoRa keeps working!

### 6. Multi-Venue Events
Coordinate events happening kilometers apart.

### 7. Mobile Stage Control
Control moving stages, parade floats, or vehicles from distance.

## 🔧 Optimization

### Maximum Range Configuration
```cpp
LoRa.setSpreadingFactor(12);     // Slowest but longest range
LoRa.setSignalBandwidth(125E3);  // Narrow bandwidth
LoRa.setCodingRate4(8);          // Maximum error correction
LoRa.setTxPower(20);             // Maximum power
```

**Range**: 10-15km (line of sight)  
**Speed**: ~250 bytes/sec

### Maximum Speed Configuration
```cpp
LoRa.setSpreadingFactor(6);      // Fastest
LoRa.setSignalBandwidth(500E3);  // Wide bandwidth
LoRa.setCodingRate4(5);          // Minimum error correction
```

**Range**: 1-2km  
**Speed**: ~5000 bytes/sec

### Balanced Configuration (Default)
```cpp
LoRa.setSpreadingFactor(7);
LoRa.setSignalBandwidth(125E3);
LoRa.setCodingRate4(5);
```

## 🐛 Troubleshooting

### "LoRa initialization failed!"

**Check**:
1. Wiring connections (especially power and SPI pins)
2. Pin definitions match your board
3. LoRa module is receiving power (3.3V, not 5V!)
4. Antenna is connected

### No messages received

**Solutions**:
1. Verify both devices use same frequency
2. Check spreading factor matches
3. Ensure antennas are connected
4. Try shorter distance first (2-3 meters)
5. Check serial monitor for errors

### Poor range

**Improve**:
1. Use external antenna (not chip antenna)
2. Increase spreading factor
3. Ensure line of sight
4. Raise antennas higher
5. Move away from metal/concrete

### Interference

**Fix**:
1. Change frequency (within legal bands)
2. Add frequency hopping
3. Enable CRC checking (default in code)
4. Use encryption for reliability

## 💡 Advanced Features

### 1. Encryption

Add encryption for security:
```cpp
#include <mbedtls/aes.h>

// Encrypt message before sending
void encryptAndSend(byte* data, int len) {
  // Implement AES encryption
  // Then send encrypted data
}
```

### 2. Acknowledgments

Receiver confirms receipt:
```cpp
void receiveLoRa() {
  // ... receive message ...
  
  // Send ACK back
  sendLoRaCommand("ACK", msg.messageId);
}
```

### 3. Retry Logic

Resend if no ACK received:
```cpp
const int MAX_RETRIES = 3;
int retryCount = 0;

while (retryCount < MAX_RETRIES && !ackReceived) {
  sendLoRaCommand(cmd);
  delay(1000);
  retryCount++;
}
```

### 4. Frequency Hopping

Avoid interference:
```cpp
const long frequencies[] = {915E6, 916E6, 917E6};
int currentFreq = 0;

void hopFrequency() {
  currentFreq = (currentFreq + 1) % 3;
  LoRa.setFrequency(frequencies[currentFreq]);
}
```

### 5. GPS Integration

Add location data (with GPS module):
```cpp
struct LoRaMessage {
  char command[16];
  float latitude;
  float longitude;
  // ... other fields
};
```

## 📏 Real-World Range Tests

| Environment | Distance Achieved | RSSI |
|-------------|-------------------|------|
| Open field (line of sight) | 10-12 km | -118 dBm |
| Urban area | 2-3 km | -95 dBm |
| Inside building to outside | 500m - 1km | -88 dBm |
| Through multiple buildings | 200-500m | -80 dBm |
| Dense forest | 1-2 km | -100 dBm |

*Results vary based on antenna, power, and spreading factor*

## 🔋 Power Consumption

- **Transmitting**: ~100-120mA
- **Receiving**: ~10-15mA
- **Sleep mode**: ~1-2mA

For battery operation:
```cpp
// Enter deep sleep between transmissions
esp_deep_sleep(60 * 1000000); // Sleep 60 seconds
```

## 🎯 Best Practices

1. **Always use antenna** - Don't operate without it (damages module)
2. **Test at close range first** - Verify communication before increasing distance
3. **Monitor RSSI** - Use signal strength to optimize position
4. **Legal compliance** - Check local frequency regulations
5. **Power supply** - Use stable 3.3V (ESP32 can provide this)

## 🌟 Cool Project Ideas

- **LoRa mesh network** - Devices relay for extended range
- **GPS tracker** - Track moving props/equipment
- **Telemetry system** - Monitor sensor data from afar
- **Emergency beacon** - Send alerts from remote locations
- **Two-way radio** - Voice over LoRa (advanced)

## 📚 Resources

- [LoRa Library Documentation](https://github.com/sandeepmistry/arduino-LoRa)
- [LoRa Calculator](https://www.rfwireless-world.com/calculators/LoRa-Data-Rate-Calculator.html)
- [Regional Frequencies](https://www.thethingsnetwork.org/docs/lorawan/frequencies-by-country.html)

---

**Control your TV show from anywhere, even miles away!** 📡🎬✨
