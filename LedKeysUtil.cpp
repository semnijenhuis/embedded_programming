#include "LedKeysUtil.h"
#include <TM1638plus.h>
#include "gameHandler.h"

extern TM1638plus tm;

 // Define countdown globally
int countdown = 9999;

static int gameProgress = 0;
static unsigned long previousMillis = 0;
const long countdownInterval = 30;

char openLetters[5] = { 'O', 'P', 'E', 'N', '\0' };
char gameResultDisplay[5] = { '_', '_', '_', '_', '\0' };
int openLetterIndex = 0;


void setupLedKeysUtil() {
  tm.displayBegin();
  tm.reset();
  tm.brightness(3);

  int activeLEDs[] = {};
  setupLEDsFromArray(sizeof(activeLEDs) / sizeof(activeLEDs[0]), activeLEDs);

  tm.readButtons();
  getPressedButton();
}
// De timer of death, when it reaches 0, the game is over
void displayTimer() {
  if (millis() - previousMillis >= countdownInterval) {
    previousMillis = millis();

    // Decrement the timer
    if (countdown > 0) {
      countdown--;
    }

    // Display game progress on the left and countdown on the right
    tm.DisplayDecNumNibble(gameProgress, countdown, false, TMAlignTextRight);

    if (countdown <= 0) {
      tm.displayText("YOURDEAD");
      delay(1000);
      tm.displayText("FAILFAIL");
      delay(1000);
      // finishes the game
      finishCurrentGame(false, true);
      // Reset the game timer
      countdown = 9999;
    }
  }
}

void addTime(int seconds) {
  // Increase countdown
  countdown += seconds;
  // Prevent overflow
  if (countdown > 9999) countdown = 9999;
}

void subtractTime(int seconds) {
  countdown -= seconds;
  // Prevent negative countdown
  if (countdown < 0) countdown = 0;
}

// Function to set up LEDs based on an array of LED numbers (to make it global accessible)
void setupLEDsFromArray(int count, int leds[]) {
  // Start with all LEDs OFF
  uint16_t ledStateTree = 0b0000000000000000;
  for (int i = 0; i < count; i++) {
    if (leds[i] >= 1 && leds[i] <= 8) {
      // Shift bit into correct position
      ledStateTree |= (1 << (leds[i] + 7));
    }
  }
  tm.setLEDs(ledStateTree);
}

// Tried displaying "OPEN" on the left side, if a user completed a game
void updateOpenDisplay(bool success) {
  if (success && openLetterIndex < 4) {
    gameResultDisplay[openLetterIndex] = openLetters[openLetterIndex];
    openLetterIndex++;
  }
  // Left side of the display
  tm.displayText(gameResultDisplay);
}

// Global function to get pressed button input
int getPressedButton() {
  // Store previous button state
  static uint8_t lastState = 0;
  static unsigned long lastPressTime = 0;

  // Small debounce time
  const int debounceDelay = 150;

  uint8_t buttons = tm.readButtons();

  // If button state changed and enough time has passed
  if (buttons != lastState && millis() - lastPressTime > debounceDelay) {
    // Update time
    lastPressTime = millis();
     // Update stored state
    lastState = buttons;

    for (int i = 0; i < 8; i++) {
      if (buttons & (1 << i)) {
        Serial.print("Detected Press: Button ");
        Serial.println(i + 1);
        return i + 1;
      }
    }
  }

  return -1;
}
