# Troubleshooting Guide

Common issues and solutions for ESP Chas TV project.

## Upload Issues

### Problem: "Failed to connect to ESP32"

**Possible Causes & Solutions**:

1. **Wrong COM Port Selected**
   - **Solution**: 
     - Windows: Check Device Manager → Ports (COM & LPT)
     - Mac/Linux: `ls /dev/tty*` or `ls /dev/cu*`
     - PlatformIO: Will auto-detect usually

2. **Driver Not Installed**
   - **Solution**:
     - CP2102: Download from Silicon Labs
     - CH340: Download CH340 driver
     - Restart computer after installation

3. **USB Cable is Charge-Only**
   - **Solution**: Try a different USB cable (data cable required)

4. **Board in Wrong Mode**
   - **Solution**: 
     - Hold BOOT button while clicking Upload
     - Release BOOT after upload starts
     - Some boards: Hold BOOT, press RESET, release both

5. **Another Program Using Serial Port**
   - **Solution**: 
     - Close Serial Monitor
     - Close other Arduino IDE/PlatformIO instances
     - Restart IDE

### Problem: "Timed out waiting for packet header"

**Solutions**:
1. Reduce upload speed in `platformio.ini`:
   ```ini
   upload_speed = 115200
   ```
2. Try pressing BOOT button during upload
3. Check USB cable quality
4. Try different USB port

## WiFi Issues

### Problem: "ESP_CHAS_TV network not showing up"

**Solutions**:

1. **ESP32 not booted yet**
   - Wait 30 seconds after upload
   - Check serial monitor for boot messages

2. **Code not uploaded successfully**
   - Re-upload the code
   - Verify upload completed without errors

3. **WiFi disabled or blocked**
   - Check serial monitor for WiFi errors
   - Ensure 2.4GHz WiFi (ESP32 doesn't support 5GHz)

4. **Power supply insufficient**
   - Use better USB power source
   - Try different USB port
   - Use external 5V power supply

### Problem: "Can connect to WiFi but can't access web interface"

**Solutions**:

1. **Wrong IP address**
   - Use exactly: `http://192.168.4.1`
   - Don't use HTTPS (use HTTP)
   - Check serial monitor for actual IP

2. **Mobile data interfering (on phone)**
   - Disable mobile data
   - Ensure using WiFi only

3. **Firewall blocking connection**
   - Temporarily disable firewall
   - Add exception for port 80

4. **Browser cache**
   - Clear browser cache
   - Try incognito/private mode
   - Try different browser

## Compilation Errors

### Problem: "WiFi.h: No such file or directory"

**Solution**:
- Ensure ESP32 platform is installed
- PlatformIO: `platform = espressif32` in platformio.ini
- Arduino IDE: Install ESP32 board support

### Problem: "WebServer.h: No such file or directory"

**Solution**:
- Should be included with ESP32 Arduino core
- Update ESP32 platform: PlatformIO → Update
- Arduino IDE: Update ESP32 board support

### Problem: "Multiple definition of 'setup'"

**Solution**:
- Only one `setup()` and `loop()` allowed
- Check for duplicate main.cpp files
- Ensure example code not conflicting

### Problem: "Board 'esp32dev' not found"

**Solution**:
- Install ESP32 platform in PlatformIO
- Or change board in platformio.ini to your board
- Check board name spelling

## Runtime Issues

### Problem: "LED not working"

**Solutions**:

1. **Wrong pin number**
   - Most boards: GPIO 2
   - Some boards: GPIO 5 or GPIO 13
   - Check your board's pinout

2. **LED damaged**
   - Test with external LED
   - Use multimeter to check voltage

3. **Code issue**
   - Check serial monitor for errors
   - Verify LED pin definition
   - Test with simple blink sketch

### Problem: "Web page loads but buttons don't work"

**Solutions**:

1. **JavaScript disabled**
   - Enable JavaScript in browser
   - Buttons use simple HTTP GET, should work

2. **Network timeout**
   - WiFi signal too weak
   - Move closer to ESP32

3. **ESP32 crashed**
   - Check serial monitor
   - Press reset button
   - Re-upload code

### Problem: "Serial monitor shows garbage characters"

**Solutions**:

1. **Wrong baud rate**
   - Set to 115200
   - Check serial monitor settings

2. **Board resetting**
   - Power supply issue
   - Add capacitor across power pins

3. **Noise on TX line**
   - Keep wires short
   - Add decoupling capacitor

## Performance Issues

### Problem: "Web interface very slow"

**Solutions**:

1. **Too many connections**
   - Limit simultaneous users
   - Reduce HTML size

2. **WiFi interference**
   - Change WiFi channel (edit code)
   - Move away from interference sources

3. **ESP32 overloaded**
   - Reduce loop() frequency
   - Optimize code
   - Remove Serial.println in tight loops

### Problem: "LED blinking inconsistently"

**Solutions**:

1. **Timing issues**
   - Use millis() instead of delay()
   - Check for blocking code

2. **Power fluctuations**
   - Better power supply
   - Add capacitors

## Common Error Messages

### "Brownout detector was triggered"

**Cause**: Voltage drop below threshold

**Solutions**:
- Use better power supply
- Add bulk capacitor (100µF-1000µF)
- Reduce peak current draw
- Disable brownout detector (not recommended):
  ```ini
  build_flags = -DCONFIG_ESP32_BROWNOUT_DET=0
  ```

### "Guru Meditation Error: Core 0 panic'ed"

**Cause**: Code crash (null pointer, stack overflow, etc.)

**Solutions**:
- Check serial monitor for details
- Enable exception decoder
- Review recent code changes
- Check array bounds
- Verify pointer validity

### "Fatal exception (0): epc1=0x..."

**Cause**: Serious code error

**Solutions**:
- Use PlatformIO's exception decoder
- Check for:
  - Invalid memory access
  - Uninitialized variables
  - Stack overflow
  - Divide by zero

## Platform-Specific Issues

### Windows

**Problem**: "Access Denied" when uploading

**Solution**:
- Run IDE as administrator
- Check antivirus blocking port
- Close other programs using COM port

### macOS

**Problem**: "Permission denied" on /dev/cu.*

**Solution**:
```bash
sudo chmod 666 /dev/cu.usbserial*
# or
sudo chmod 666 /dev/cu.SLAB_USBtoUART
```

### Linux

**Problem**: "Permission denied" on /dev/ttyUSB*

**Solution**:
```bash
sudo usermod -a -G dialout $USER
# Log out and back in
```

Or for immediate access:
```bash
sudo chmod 666 /dev/ttyUSB0
```

## Hardware Issues

### Problem: "ESP32 gets very hot"

**Causes & Solutions**:
- **Short circuit**: Check wiring immediately
- **Overcurrent**: Check connected devices
- **Normal WiFi operation**: Some warmth is normal
- **Damaged board**: Replace if excessive

### Problem: "Board won't boot after connecting peripherals"

**Solutions**:
- Check wiring for shorts
- Ensure proper voltage levels (3.3V logic)
- Use level shifters for 5V devices
- Check current draw doesn't exceed ratings

## Still Having Issues?

If problems persist:

1. **Check Serial Monitor First**
   - Most errors show up here
   - Set baud rate to 115200

2. **Try Minimal Code**
   - Upload simple blink sketch
   - Verify basic functionality
   - Add features incrementally

3. **Check Hardware**
   - Test with multimeter
   - Verify connections
   - Try different components

4. **Search Issues**
   - GitHub issues for this project
   - ESP32 forums
   - Stack Overflow

5. **Ask for Help**
   - Open a GitHub issue
   - Include:
     - Board model
     - Complete error message
     - Serial monitor output
     - Code snippet
     - What you've tried

## Useful Debug Commands

### Check ESP32 Info
```cpp
Serial.println("ESP32 Chip ID: " + String((uint32_t)ESP.getEfuseMac(), HEX));
Serial.println("Flash Size: " + String(ESP.getFlashChipSize()));
Serial.println("Free Heap: " + String(ESP.getFreeHeap()));
Serial.println("CPU Freq: " + String(ESP.getCpuFreqMHz()) + " MHz");
```

### Monitor WiFi
```cpp
Serial.println("WiFi Status: " + String(WiFi.status()));
Serial.println("Connected clients: " + String(WiFi.softAPgetStationNum()));
Serial.println("IP: " + WiFi.softAPIP().toString());
```

### Test LED
```cpp
// Rapid blink to verify
for(int i=0; i<10; i++) {
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);
  delay(100);
}
```

---

**Can't find your issue here? Open a GitHub issue and we'll add it!** 🔧✨
