#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 
#define SCREEN_ADDRESS 0x3C // <-- CHANGE THIS to 0x3D if needed

#define BTN_PIN 15

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Eye parameters (Anime style: filled rectangles with reflections)
const int EYE_WIDTH = 25;
const int EYE_HEIGHT = 35;
const int LEFT_EYE_X_BASE = 15;
const int RIGHT_EYE_X_BASE = 88;
const int EYE_Y_BASE = 15; 
const int REFLECTION_SIZE = 2; // Pupil/reflection size

// Pupil movement variables for smooth tracking
int pupilOffsetX, pupilOffsetY; // Unified offsets for both eyes
int targetOffsetX, targetOffsetY; // Unified targets

// Timing variables
unsigned long previousMillis = 0;
const long frameDelay = 30; // Target 33 FPS
unsigned long nextActionTime = 0;
enum FaceState { NORMAL, BLINKING, WINKING, REACTING };
FaceState currentState = NORMAL;
const int BLINK_DURATION = 100;
unsigned long actionStartTime = 0;

// Button variables
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;


void setup() {
  Wire.begin(); 
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for(;;); 
  }
  pinMode(BTN_PIN, INPUT_PULLUP);
  randomSeed(analogRead(0));
  setNewPupilTarget();
  scheduleNextAction();
}

void loop() {
  unsigned long currentMillis = millis();

  handleButton(); 
  handleActions(currentMillis); 
  updatePupilPosition(); // Smoothly move pupils

  if (currentMillis - previousMillis >= frameDelay) {
    previousMillis = currentMillis;
    display.clearDisplay(); 
    drawFace();
    display.display(); 
  }
}

// --- Function Definitions ---

void handleButton() {
  int reading = digitalRead(BTN_PIN);
  if (reading != lastButtonState) lastDebounceTime = millis();

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != lastButtonState) {
      lastButtonState = reading;
      if (lastButtonState == LOW) {
        // React to button press by looking UP suddenly
        currentState = REACTING;
        actionStartTime = millis();
        // Set unified targets for looking UP
        targetOffsetX = 0; 
        targetOffsetY = -10; // Look high up in the eye area
      }
    }
  }
}

void handleActions(unsigned long currentMillis) {
    if (currentState == NORMAL && currentMillis >= nextActionTime) {
      int action = random(0, 100); 
      actionStartTime = currentMillis;
      if (action < 85) currentState = BLINKING; 
      else currentState = WINKING;
    }

    if ((currentState == BLINKING || currentState == WINKING || currentState == REACTING) && 
        currentMillis - actionStartTime >= (currentState == WINKING ? 200 : 100)) {
      currentState = NORMAL;
      scheduleNextAction();
      setNewPupilTarget(); // Get a new random *synchronized* target location
    }
}

void scheduleNextAction() {
  nextActionTime = millis() + random(1500, 4000); 
}

void setNewPupilTarget() {
  // Define a restricted area for the reflection dots to wander within the white eye block
  // This range is now applied as a unified offset
  int xRange = (EYE_WIDTH - (REFLECTION_SIZE * 2) - 2) / 2;
  int yRange = (EYE_HEIGHT - (REFLECTION_SIZE * 2) - 2) / 2;

  targetOffsetX = random(-xRange, xRange + 1);
  targetOffsetY = random(-yRange, yRange + 1);
}

void updatePupilPosition() {
  // Smooth movement towards the unified target
  pupilOffsetX += (targetOffsetX - pupilOffsetX) / 3;
  pupilOffsetY += (targetOffsetY - pupilOffsetY) / 3;
}

void drawFace() {
    drawMouthHappy();
    
    // Calculate the actual pupil positions based on the base location + unified offset
    int leftPupilX = LEFT_EYE_X_BASE + EYE_WIDTH/2 + pupilOffsetX;
    int leftPupilY = EYE_Y_BASE + EYE_HEIGHT/2 + pupilOffsetY;
    int rightPupilX = RIGHT_EYE_X_BASE + EYE_WIDTH/2 + pupilOffsetX;
    int rightPupilY = EYE_Y_BASE + EYE_HEIGHT/2 + pupilOffsetY;


    if (currentState == BLINKING) {
        // Draw lines for closed eyes
        int blinkY = EYE_Y_BASE + (EYE_HEIGHT / 2);
        display.drawLine(LEFT_EYE_X_BASE, blinkY, LEFT_EYE_X_BASE + EYE_WIDTH, blinkY, SSD1306_WHITE);
        display.drawLine(RIGHT_EYE_X_BASE, blinkY, RIGHT_EYE_X_BASE + EYE_WIDTH, blinkY, SSD1306_WHITE);
    } else if (currentState == WINKING) {
        // Left eye open, right eye closed line
        display.fillRect(LEFT_EYE_X_BASE, EYE_Y_BASE, EYE_WIDTH, EYE_HEIGHT, SSD1306_WHITE);
        display.fillCircle(leftPupilX, leftPupilY, REFLECTION_SIZE, SSD1306_BLACK);
        int blinkY = EYE_Y_BASE + (EYE_HEIGHT / 2);
        display.drawLine(RIGHT_EYE_X_BASE, blinkY, RIGHT_EYE_X_BASE + EYE_WIDTH, blinkY, SSD1306_WHITE);
    } else {
        // Draw open eyes with synchronized moving reflection/pupil dots
        display.fillRect(LEFT_EYE_X_BASE, EYE_Y_BASE, EYE_WIDTH, EYE_HEIGHT, SSD1306_WHITE);
        display.fillCircle(leftPupilX, leftPupilY, REFLECTION_SIZE, SSD1306_BLACK);
        display.fillRect(RIGHT_EYE_X_BASE, EYE_Y_BASE, EYE_WIDTH, EYE_HEIGHT, SSD1306_WHITE);
        display.fillCircle(rightPupilX, rightPupilY, REFLECTION_SIZE, SSD1306_BLACK);
    }
}

void drawMouthHappy() {
  int mouthX = SCREEN_WIDTH / 2;
  int mouthY = EYE_Y_BASE + EYE_HEIGHT + 10;
  display.drawLine(mouthX - 5, mouthY, mouthX, mouthY + 3, SSD1306_WHITE);
  display.drawLine(mouthX, mouthY + 3, mouthX + 5, mouthY, SSD1306_WHITE);
}
