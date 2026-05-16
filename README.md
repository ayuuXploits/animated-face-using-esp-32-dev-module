# 🤖 OLED Anime Face

An animated anime-style face for a 128x64 OLED display, built for Arduino/ESP-compatible boards. Features smooth eye movement, blinking, winking, and a button-triggered reaction — all rendered at ~33 FPS.

> Made by [ayuuXploits](https://github.com/ayuuXploits)

---

## Preview

```
 ┌─────────────────────────────┐
 │   ██████       ██████       │
 │   ██  ██       ██  ██       │   ← Anime eyes (white filled rects)
 │   ██████       ██████       │     with moving black dot pupils
 │                             │
 │          /\                 │   ← Happy mouth
 └─────────────────────────────┘
       128x64 OLED Display
```

---

## Features

- **Anime-style eyes** — filled rectangular eyes with moving black dot pupils/reflections
- **Smooth pupil tracking** — pupils glide to random positions with eased interpolation
- **Synchronized eye movement** — both eyes always look in the same direction
- **Auto blinking** — blinks randomly every 1.5–4 seconds
- **Winking** — occasional wink (one eye closes, other stays open)
- **Button reaction** — press the button to make the face look upward suddenly
- **~33 FPS rendering** — smooth 30ms frame loop via non-blocking `millis()` timing

---

## Hardware Requirements

| Component | Details |
|-----------|---------|
| Microcontroller | Arduino, ESP32, ESP8266, or compatible |
| Display | SSD1306 128x64 OLED (I2C) |
| Button | Momentary push button |
| Resistor | Not required (uses internal pull-up) |

---

## Wiring

```
OLED Display         Microcontroller
────────────         ───────────────
VCC          →       3.3V / 5V
GND          →       GND
SDA          →       SDA (e.g. GPIO 21 on ESP32)
SCL          →       SCL (e.g. GPIO 22 on ESP32)

Button               Microcontroller
──────               ───────────────
One leg      →       GPIO 15  (BTN_PIN)
Other leg    →       GND
```

> The button uses the internal pull-up resistor — no external resistor needed.

---

## Dependencies

Install these libraries via the **Arduino Library Manager** (Sketch → Include Library → Manage Libraries):

| Library | Author |
|---------|--------|
| `Adafruit SSD1306` | Adafruit |
| `Adafruit GFX Library` | Adafruit |

---

## Setup & Upload

1. Clone or download this repository.
2. Open the `.ino` file in the Arduino IDE.
3. Install the required libraries (see above).
4. **Check your OLED I2C address** — most modules use `0x3C`, but some use `0x3D`. Change `SCREEN_ADDRESS` in the code if needed:
   ```cpp
   #define SCREEN_ADDRESS 0x3C  // change to 0x3D if display doesn't work
   ```
5. Select your board and port under **Tools**.
6. Upload and enjoy!

---

## Configuration

All visual parameters are defined at the top of the sketch and easy to tweak:

```cpp
// Eye dimensions
const int EYE_WIDTH  = 25;
const int EYE_HEIGHT = 35;

// Eye positions
const int LEFT_EYE_X_BASE  = 15;
const int RIGHT_EYE_X_BASE = 88;
const int EYE_Y_BASE       = 15;

// Pupil dot size
const int REFLECTION_SIZE = 2;

// Button pin
#define BTN_PIN 15

// Blink interval (ms)
random(1500, 4000)  // change values in scheduleNextAction()
```

---

## How It Works

| State | Trigger | Behavior |
|-------|---------|---------|
| `NORMAL` | Default | Eyes open, pupils roam smoothly |
| `BLINKING` | Auto (~85% chance) | Both eyes draw a horizontal line |
| `WINKING` | Auto (~15% chance) | Left eye open, right eye closed |
| `REACTING` | Button press | Pupils snap upward, then return to normal |

Pupil movement uses simple linear interpolation each frame:
```cpp
pupilOffsetX += (targetOffsetX - pupilOffsetX) / 3;
```
This gives a natural easing effect without any extra libraries.

---

## License

© 2025 ayuuXploits. All rights reserved.

No part of this project may be copied, modified, distributed, or used without explicit permission from the author.

---

*Built with ❤️ and pixels by [ayuuXploits](https://github.com/ayuuXploits)*
