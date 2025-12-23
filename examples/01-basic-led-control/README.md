# Basic LED Control Example

This example demonstrates basic LED control patterns for TV show effects.

## Hardware Required

- ESP32 development board
- Built-in LED (GPIO 2 on most boards)
- Optional: External LED with 220Ω resistor

## Features

- Multiple LED patterns (slow blink, fast blink, pulse, strobe)
- Serial control interface
- Easy to extend with more patterns

## Usage

1. Copy `led_control.cpp` to `src/main.cpp`
2. Build and upload to ESP32
3. Open serial monitor (115200 baud)
4. Send commands to change patterns:
   - `1` - Slow blink (1 second interval)
   - `2` - Fast blink (200ms interval)
   - `3` - Pulse (smooth fade)
   - `4` - Strobe (dramatic flash)
   - `0` - Off

## Use Cases for TV Show

- **Scene transitions**: Use different patterns between scenes
- **Mood indication**: Pulse for dramatic moments, strobe for action
- **Audience cues**: Specific patterns signal when to participate
- **Status indicators**: Show technical crew what's happening

## Extending This Example

Ideas for enhancement:
- Add RGB LED support for color changes
- Sync multiple ESP32 devices
- Add web interface for remote control
- Create custom pattern sequences
- Add sound-reactive patterns

Enjoy experimenting! 🎬✨
