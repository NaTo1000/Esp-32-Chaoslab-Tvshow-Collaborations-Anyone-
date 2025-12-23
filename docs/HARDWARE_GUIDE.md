# Hardware Guide

This guide covers the hardware aspects of the ESP Chas TV project.

## ESP32 Board Selection

### Recommended Boards

#### ESP32-DevKitC (Most Common)
- **Pros**: Widely available, well-documented, affordable
- **Features**: WiFi, Bluetooth, 30+ GPIO pins
- **Best for**: General purpose, beginners
- **Price**: ~$5-10

#### NodeMCU-32S
- **Pros**: Popular, breadboard-friendly
- **Features**: Similar to DevKitC with easier pinout
- **Best for**: Prototyping, breadboard projects
- **Price**: ~$6-12

#### ESP32-S3
- **Pros**: More powerful, USB OTG
- **Features**: Dual-core, more RAM, better WiFi
- **Best for**: Advanced projects, better performance
- **Price**: ~$8-15

#### ESP32-C3
- **Pros**: Newer, RISC-V processor
- **Features**: Single core, lower power, smaller
- **Best for**: Power-efficient projects
- **Price**: ~$5-10

## Pin Reference

### Common GPIO Pins

Most ESP32 boards have these standard pins:

#### Digital I/O Pins
- **GPIO 0-39**: General purpose I/O
- **GPIO 2**: Built-in LED (most boards)
- **GPIO 34-39**: Input only (no pull-up/down)

#### Special Purpose Pins
- **TX/RX (GPIO 1/3)**: Serial communication
- **SPI**: GPIO 18 (SCK), 19 (MISO), 23 (MOSI), 5 (SS)
- **I2C**: GPIO 21 (SDA), 22 (SCL)
- **PWM**: Most GPIO pins support PWM

#### Best Pins for TV Show Projects
- **LEDs**: GPIO 2, 4, 5, 12-19, 21-23, 25-27
- **Buttons**: GPIO 32-39 (with internal pull-up)
- **Displays**: SPI or I2C pins
- **Sensors**: Any GPIO depending on type

### Pins to Avoid
- **GPIO 6-11**: Connected to flash memory (DO NOT USE)
- **GPIO 0**: Used for boot mode (can cause issues)
- **GPIO 2**: Boot mode pin (generally safe after boot)

## Display Options

### OLED Displays (I2C)

**SSD1306 (128x64 or 128x32)**
- **Connection**: I2C (SDA=GPIO21, SCL=GPIO22)
- **Power**: 3.3V
- **Library**: Adafruit_SSD1306
- **Use**: Text, simple graphics, show info
- **Price**: ~$3-8

### TFT Displays (SPI)

**ILI9341 (2.4" - 3.2")**
- **Connection**: SPI + additional control pins
- **Power**: 3.3V or 5V (check model)
- **Library**: TFT_eSPI
- **Use**: Graphics, images, animations
- **Price**: ~$10-20

### LED Matrix

**MAX7219 (8x8 or 32x8)**
- **Connection**: SPI
- **Power**: 5V
- **Library**: MD_Parola, MD_MAX72XX
- **Use**: Scrolling text, patterns, effects
- **Price**: ~$5-15

## LED Strip Options

### NeoPixel / WS2812B

**Specifications**
- **Type**: Addressable RGB LED
- **Voltage**: 5V
- **Control**: Single data pin
- **Library**: FastLED or Adafruit_NeoPixel

**Connection**
```
ESP32 GPIO 13 → LED Strip DIN
ESP32 GND      → LED Strip GND
5V Power       → LED Strip 5V
```

**Important Notes**
- Use external 5V power supply for strips with >10 LEDs
- Add 470Ω resistor on data line
- Add 1000µF capacitor across power supply

### WS2811 / WS2813

Similar to WS2812B but with:
- Separate power and data
- Backup data line (WS2813)
- More robust for longer runs

## Sensor Integration

### Motion Sensors

**PIR Motion Sensor (HC-SR501)**
- **Connection**: Digital input (GPIO 32-39)
- **Power**: 5V
- **Use**: Detect audience movement
- **Price**: ~$2-5

### Sound Sensors

**MAX9814 / MAX4466**
- **Connection**: Analog input (GPIO 32-39)
- **Power**: 3.3V
- **Use**: React to music, applause, noise
- **Price**: ~$5-10

### Distance Sensors

**HC-SR04 Ultrasonic**
- **Connection**: 2 digital pins (Trigger/Echo)
- **Power**: 5V
- **Use**: Proximity detection, interactive props
- **Price**: ~$2-5

## Power Considerations

### Power Sources

#### USB Power (5V)
- **Good for**: Development, single ESP32
- **Limitation**: Usually 500mA max
- **Use**: Testing, basic projects

#### External Power Supply
- **Voltage**: 5V or 3.3V
- **Current**: Depends on peripherals
- **Use**: LED strips, motors, multiple components

#### Battery Power
- **18650 Li-ion**: 3.7V nominal, rechargeable
- **Power bank**: 5V USB output, convenient
- **LiPo**: Various voltages, lightweight

### Power Calculation

**ESP32 alone**: ~80-160mA (WiFi active)

**With peripherals**:
- Small OLED: +15mA
- WS2812B LED: +60mA per LED (full white)
- Servo motor: +100-500mA
- TFT display: +50-100mA

**Example**: ESP32 + 10 LEDs + OLED
- ESP32: 150mA
- LEDs: 10 × 60mA = 600mA
- OLED: 15mA
- **Total**: ~765mA minimum

**Recommendation**: Use power supply rated 2× your calculated need.

## Wiring Best Practices

### General Guidelines

1. **Use appropriate wire gauge**
   - Signal: 22-24 AWG
   - Power (< 1A): 20-22 AWG
   - Power (> 1A): 18-20 AWG

2. **Keep wires short**
   - Reduces interference
   - Better signal integrity

3. **Separate power and signal**
   - Avoid noise coupling
   - Better reliability

4. **Use bypass capacitors**
   - 0.1µF near each IC
   - Reduces power noise

5. **Add pull-up/pull-down resistors**
   - For buttons: 10kΩ pull-down
   - For I2C: 4.7kΩ pull-up

### Common Wiring Diagram

```
┌─────────────────┐
│     ESP32       │
│                 │
│ GPIO 2  ────────┼───[LED]──[220Ω]─── GND
│                 │
│ GPIO 21 ────────┼───[OLED SDA]
│ GPIO 22 ────────┼───[OLED SCL]
│                 │
│ GPIO 13 ────────┼───[470Ω]──[LED Strip DIN]
│                 │
│ 3.3V    ────────┼─── OLED VCC
│ GND     ────────┼─── OLED GND, LED Strip GND
│                 │
└─────────────────┘
                  
[5V Supply] ──── LED Strip 5V
```

## Safety Notes

⚠️ **Important Safety Guidelines**:

1. **Never exceed voltage ratings**
   - ESP32 GPIO: 3.3V max
   - Logic level shifters for 5V devices

2. **Check current limits**
   - ESP32 GPIO: 40mA max per pin
   - Use transistors/MOSFETs for high current

3. **Protect against reverse polarity**
   - Use diodes on power inputs

4. **Add fuses**
   - Protect your circuit from shorts

5. **Proper grounding**
   - All grounds connected
   - Star ground for sensitive circuits

## Enclosures and Mounting

### Recommended Enclosures

- **Plastic project boxes**: Easy to modify
- **3D printed cases**: Custom fit
- **Acrylic cases**: Professional look
- **DIN rail mounts**: For permanent installations

### Mounting Options

- Standoffs for PCBs
- Velcro for temporary setups
- Hot glue for quick prototypes
- Cable ties for wire management

## Shopping List for Beginners

### Basic Starter Kit (~$30-50)
- [ ] ESP32 DevKitC board
- [ ] USB cable
- [ ] Breadboard
- [ ] Jumper wires (male-male, male-female)
- [ ] LEDs (various colors)
- [ ] Resistors (220Ω, 10kΩ)
- [ ] Push buttons

### Intermediate Kit (~$50-100)
- Everything in Basic Kit plus:
- [ ] OLED display (128x64)
- [ ] WS2812B LED strip (30 LEDs)
- [ ] PIR motion sensor
- [ ] 5V power supply (3A)
- [ ] Voltage level shifter

### Advanced Kit (~$100-200)
- Everything in Intermediate Kit plus:
- [ ] TFT touchscreen display
- [ ] Multiple ESP32 boards
- [ ] Servo motors
- [ ] Sound sensor
- [ ] Relay modules
- [ ] Battery pack
- [ ] Soldering kit

## Where to Buy

### Online Retailers
- **AliExpress**: Cheapest, longer shipping
- **Amazon**: Fast shipping, higher prices
- **Adafruit**: Quality, good support
- **SparkFun**: Quality, tutorials
- **Digi-Key/Mouser**: Professional components

### Local Options
- Electronics hobby shops
- Maker spaces
- University surplus stores

## Next Steps

1. Choose your ESP32 board
2. Select displays/LEDs for your project
3. Plan your power requirements
4. Order components
5. Start building!

---

**Need help choosing hardware? Open an issue and ask!** 🛠️✨
