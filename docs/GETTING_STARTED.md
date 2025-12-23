# Getting Started with ESP Chas TV

Welcome to the ESP Chas TV project! This guide will help you get up and running quickly.

## Prerequisites

### Software Requirements

1. **PlatformIO IDE** (recommended)
   - Install [VS Code](https://code.visualstudio.com/)
   - Install [PlatformIO extension](https://platformio.org/install/ide?install=vscode)

   OR

2. **Arduino IDE** (alternative)
   - Download from [arduino.cc](https://www.arduino.cc/en/software)
   - Install ESP32 board support

### Hardware Requirements

- **ESP32 Development Board** - Any of these will work:
  - ESP32-WROOM-32
  - ESP32-DevKitC
  - ESP32-S3
  - ESP32-C3
  - NodeMCU-32S
  
- **USB Cable** (usually Micro-USB or USB-C depending on your board)
- **Computer** with USB port

### Optional Hardware for Advanced Features

- LED strips (WS2812, NeoPixel)
- TFT or OLED displays
- Sensors (temperature, motion, sound)
- Buttons and switches
- Relays for controlling lights/devices

## Installation Steps

### Method 1: Using PlatformIO (Recommended)

1. **Clone the repository**
   ```bash
   git clone https://github.com/NaTo1000/Esp-32-Chaoslab-Tvshow-Collaborations-Anyone-.git
   cd Esp-32-Chaoslab-Tvshow-Collaborations-Anyone-
   ```

2. **Open in VS Code**
   - Open VS Code
   - File → Open Folder
   - Select the cloned repository folder

3. **PlatformIO will automatically**
   - Detect the project
   - Download necessary dependencies
   - Configure the build environment

4. **Build the project**
   - Click the PlatformIO icon in the left sidebar
   - Click "Build" (checkmark icon)
   - Or use terminal: `pio run`

5. **Connect your ESP32**
   - Plug ESP32 into USB port
   - Your OS should detect it

6. **Upload to ESP32**
   - Click "Upload" (arrow icon) in PlatformIO
   - Or use terminal: `pio run --target upload`

7. **Open Serial Monitor**
   - Click "Serial Monitor" (plug icon)
   - Set baud rate to 115200
   - Or use terminal: `pio device monitor`

### Method 2: Using Arduino IDE

1. **Install ESP32 Board Support**
   - File → Preferences
   - Add to Additional Board URLs:
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Tools → Board → Boards Manager
   - Search "ESP32" and install "esp32 by Espressif Systems"

2. **Clone and Open**
   ```bash
   git clone https://github.com/NaTo1000/Esp-32-Chaoslab-Tvshow-Collaborations-Anyone-.git
   ```
   - Copy `src/main.cpp` content to a new Arduino sketch
   - Rename to `.ino` extension

3. **Select Board**
   - Tools → Board → ESP32 Arduino → Select your board

4. **Upload**
   - Connect ESP32
   - Tools → Port → Select your port
   - Click Upload button

## First Run

### Connecting to ESP Chas TV

After uploading, the ESP32 will create a WiFi access point:

1. **Find the WiFi network** on your phone or computer:
   - Network name: `ESP_CHAS_TV`
   - Password: `chaoslab2024`

2. **Connect to the network**

3. **Open a web browser** and navigate to:
   - `http://192.168.4.1`

4. **You should see** the ESP Chas TV control interface! 🎬

### Using the Web Interface

- **Start Show** - Begin the show (LED starts blinking)
- **Stop Show** - End the show
- **Scene Buttons** - Change between different scenes
- **Status Display** - See current show status and viewer count

## Testing Your Setup

### Serial Monitor Test

Open the serial monitor (115200 baud). You should see:
```
🎬 ESP Chas TV Starting...
================================
📡 Starting WiFi Access Point...
✅ AP Started! IP address: 192.168.4.1
📱 Connect to WiFi: ESP_CHAS_TV
🔑 Password: chaoslab2024
🌐 Web server started!
================================
🎭 Ready for collaboration!
================================
```

### LED Test

- The built-in LED (usually on GPIO 2) should be off initially
- Click "Start Show" in the web interface
- LED should start blinking
- Click "Stop Show" to turn it off

### WiFi Test

- Use your phone/computer to scan for WiFi networks
- Find `ESP_CHAS_TV`
- Connect using password `chaoslab2024`
- Browse to `192.168.4.1`

## Troubleshooting

### Can't Upload to ESP32

- **Check USB cable** - Some cables are charging-only
- **Hold BOOT button** while clicking Upload (some boards)
- **Check driver** - Install CH340 or CP2102 driver if needed
- **Try different USB port**
- **Check board selection** in Tools → Board

### WiFi Not Showing Up

- **Wait 30 seconds** after upload for boot
- **Check serial monitor** for startup messages
- **Restart ESP32** (press EN/RST button)
- **Re-upload code** if necessary

### Can't Connect to Web Interface

- **Ensure you're connected** to ESP_CHAS_TV WiFi
- **Try** `http://192.168.4.1` exactly
- **Disable mobile data** on phone (should use WiFi)
- **Check firewall** settings on computer

### Compilation Errors

- **Update PlatformIO** - Click PlatformIO → Update All
- **Clean build** - Delete `.pio` folder and rebuild
- **Check platformio.ini** matches your board

## Next Steps

Now that you have the basic setup running:

1. **Explore the examples** in the `examples/` directory
2. **Read the project documentation** in `docs/`
3. **Check out CONTRIBUTING.md** to add your own features
4. **Join the collaboration** and share your ideas!

## Need Help?

- Open an issue on GitHub
- Check existing issues for solutions
- Join our community discussions
- Read the full documentation

---

**Happy hacking and enjoy creating TV show magic!** 🎬✨
