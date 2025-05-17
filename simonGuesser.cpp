#include "simonGuesser.h"
#include "LedKeysUtil.h"
#include "lcdDisplayUtil.h"
#include <TM1638plus.h>
#include "gameHandler.h"

extern TM1638plus tm;


int firstNumb;
int secondNumb;
int thirdNumber;
int forthNumber;
int ledOff[] = {};


// Global variables for managing input:
const int patternLength = 4;
int userInput[patternLength] = { 0, 0, 0, 0 };
int inputIndex = 0;
int simonStep = 0;
bool waitingForInput = false;
bool showedPattern = false;

int roundCount = 0;
const int totalRounds = 3;

void setup_simon_guesser() {
  Serial.begin(115200);
  randomSeed(analogRead(A0));

  roundCount = 0;

  startNewRound();
}

void startNewRound() {
  // Generate random numbers between 1 and 8
  firstNumb = random(1, 9);
  secondNumb = random(1, 9);
  thirdNumber = random(1, 9);
  forthNumber = random(1, 9);

  // Show the pattern
  String patternStr = String(firstNumb) + " " + String(secondNumb) + " " + String(thirdNumber) + " " + String(forthNumber);
  displayText("Right answer:", patternStr);

  inputIndex = 0;
  showedPattern = false;
}

int getSimonNumberForStep(int step) {
  // Return the expected number for each step
  switch (step) {
    case 0: return firstNumb;
    case 1: return secondNumb;
    case 2: return thirdNumber;
    case 3: return forthNumber;
    default: return -1;
  }
}

// Function to play the Simon Says game
void playGame_simon_guesser() {
  static unsigned long lastActionTime = 0;

  if (!showedPattern) {
    showedPattern = displayLEDPattern(showedPattern);
  }

    // Check if the game is in progress and waiting for input for new patern idea
  if (!waitingForInput) {
    int ledPattern[patternLength];
    for (int i = 0; i < patternLength; i++) {
      ledPattern[i] = getSimonNumberForStep(i);
    }
    setupLEDsFromArray(0, ledOff);
    displayText("Simon Says", "Repeat 4 LEDs");
    waitingForInput = true;
    inputIndex = 0;
    lastActionTime = millis();
  }

  // Every loop, check for a button pressed
  int buttonPressed = getPressedButton();
  if (buttonPressed > 0) {
    userInput[inputIndex] = buttonPressed;
    inputIndex++;

    // Displayes  feedback
    String userInputStr = "";
    for (int j = 0; j < inputIndex; j++) {
      userInputStr += String(userInput[j]) + " ";
    }
    displayText("You pressed:", userInputStr);
    // Debounce delay
    delay(300);

    // Once collected 4 inputs, checking the pattern:
    if (inputIndex == patternLength) {
      bool correct = true;
      for (int i = 0; i < patternLength; i++) {
        if (userInput[i] != getSimonNumberForStep(i)) {
          correct = false;
          break;
        }
      }
      if (correct) {
        displayText("Correct!", "Well done");
        delay(1000);
        roundCount++;

        if (roundCount < totalRounds) {
          // If there are more rounds, starst a new round
          startNewRound();
          waitingForInput = false;
          showedPattern = false;
        } else {
          // Otherwise game finished after 3 rounds and also last game
          delay(2000);
          // finishes the game, but also as last game
          finishCurrentGame(true, true);
        }
        return;

      } else {
        displayText("Wrong!", "Try again");
        delay(500);

        displayText("No hard punsiment", "sick -750");
        subtractTime(750);

        delay(2000);

        // resets input
        inputIndex = 0;
        waitingForInput = true;

        displayText("Look again", "Try to follow");
        delay(500);
        showedPattern = displayLEDPattern(false);
      }
    }
  }
}

bool displayLEDPattern(bool showedPattern) {
  // second delay to show the led pattern
  const int delayTime = 1000;
  // Shorter delay for button press feedback
  const int delayTimeShort = 300;

  if (!showedPattern) {
    int displayfirstLED[] = { firstNumb };
    setupLEDsFromArray(1, displayfirstLED);

    // Show the pattern for 1 second
    delay(delayTime);
    setupLEDsFromArray(0, ledOff);
    delay(delayTimeShort);

    int displaysecondLED[] = { secondNumb };
    setupLEDsFromArray(1, displaysecondLED);
    delay(delayTime);
    setupLEDsFromArray(0, ledOff);
    delay(delayTimeShort);

    int displaythirdLED[] = { thirdNumber };
    setupLEDsFromArray(1, displaythirdLED);
    delay(delayTime);
    setupLEDsFromArray(0, ledOff);
    delay(delayTimeShort);

    int displayforthLED[] = { forthNumber };
    setupLEDsFromArray(1, displayforthLED);
    delay(delayTime);
    setupLEDsFromArray(0, ledOff);
    delay(delayTimeShort);

    return true;
  }
}
