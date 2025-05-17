#include <LiquidCrystal_I2C.h>
#include <TM1638plus.h>
#include "LedKeysUtil.h"
#include "lcdDisplayUtil.h"
#include "gameHandler.h"  // Handles game selection + setup
#include "numberGuesser.h"
#include "wordGuesser.h"
#include "safeSimulator.h"
#include "songGuesser.h"
#include "simonGuesser.h"

// Define TM1638 pins
#define TM_STROBE 4
#define TM_CLOCK 6
#define TM_DIO 7

// Create TM1638 object
TM1638plus tm(TM_STROBE, TM_CLOCK, TM_DIO);

// Global variables for the game
int currentGame = 0;
bool runningTheGame = false;

void setup() {
    // Initialize serial communication to print debug messages
  Serial.begin(115200);

  // Initialize components
  tm.displayBegin();
  setupLedKeysUtil();
  lcdScreenSetup();

  // Setup game manager
  setupGameHandler();
}

void loop() {
  // starts the clock of death
  displayTimer();  // Show countdown + progress bar

  if (!runningTheGame) {
    // Starts the game
    playNextGame();
  } else {
    // Continuously runs the current game
    switch (currentGame) {
      case 0:
        playGame_number_guesser(tm);
        break;
      case 1:
        playGame_word_guesser();
        break;
      case 2:
        playGame_safe_simulator(tm);
        break;
      case 3:
        playGame_song_guesser();
        break;
      case 4:
        playGame_simon_guesser();
        break;
    }
  }

  // small debounce
  delay(100);
}
