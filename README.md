# animated-face-using-esp-32-dev-module
libraries needed in arduino ide :-
wire.h
adafruit_gfx.h
adafruit_ssd1306.h
# required hardware
esp 32
jumper wires
.95" oled display
breadboard
A fun expressive face rendered on a 128×64 SSD1306 OLED using Arduino. The display features two large anime-style rectangular eyes with synchronized pupils that wander smoothly using lerp-based interpolation. The face blinks randomly every 1.5–4 seconds, occasionally winks, and reacts with an upward glance when a button is pressed (pin 15, active-low with debounce). A simple happy mouth is drawn below the eyes. Runs at ~33 FPS with a non-blocking millis() loop — no delay() calls in the main logic.
