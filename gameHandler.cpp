#include "gameHandler.h"
#include "lcdDisplayUtil.h"
#include "LedKeysUtil.h"
#include "numberGuesser.h"
#include "wordGuesser.h"
#include "safeSimulator.h"
#include "songGuesser.h"
#include "simonGuesser.h"

extern TM1638plus tm;
extern int currentGame;
extern bool runningTheGame;
int successfulGames = 0;

void setupGameHandler() {
  currentGame = 0;
  runningTheGame = false;
  successfulGames = 0;
}

// Sets up the game, which is being played
void playNextGame() {
  switch (currentGame) {
    case 0:
      displayText("Get ready for", "Number Guesser!");
      delay(2000);
      setup_number_guesser(tm);
      break;
    case 1:
      displayText("Next up:", "Word Guesser!");
      delay(2000);
      setup_word_guesser();
      break;
    case 2:
      displayText("Crack the", "Safe Simulator!");
      delay(2000);
      setup_safe_simulator(tm);
      break;
    case 3:
      displayText("Play the", "Song Guesser!");
      delay(2000);
      setup_song_guesser();
      break;
    case 4:
      displayText("Time for", "Simon Says!");
      delay(2000);
      setup_simon_guesser();
      break;
  }

  runningTheGame = true;
}

// Finishes the current game and updates the display
void finishCurrentGame(bool success, bool lastGameComplete) {
  if (success) {
    displayText("Success!", "You passed!");
    delay(1000);
    successfulGames++;
  } else {
    displayText("Failed!", "No points!");
    delay(1000);
  }

  Serial.print("current game: ");
  Serial.println(currentGame);
  Serial.print("won games: ");
  Serial.println(successfulGames);

  updateOpenDisplay(success);

  runningTheGame = false;
  currentGame++;

  // Check if we have reached the last game
  if (lastGameComplete) {
    displayEndCredits();  // Now only show this when explicitly told it's the last game
    delay(3000);
    currentGame = 0;
    successfulGames = 0;
  } else {
    playNextGame();  // Proceed to next game normally
  }
}

// Displays the end credits based on the number of successful games
void displayEndCredits() {
  if (successfulGames == 5) {
    displayText("Amazing!", "You succeeded all!");
    delay(2000);
    displayText("Enjoy your freedom!", "You passed the escape!");
  } else if (successfulGames >= 4) {
    displayText("Well done!", "4/5 games completed!");
    delay(2000);
    displayText("You're allowed to leave!", "Enjoy your freedom!");
  } else {
    displayText("Try Again", "You failed to many games!");
  }
}
