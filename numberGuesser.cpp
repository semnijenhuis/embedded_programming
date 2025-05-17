#include "numberGuesser.h"
#include <TM1638plus.h>
#include "LedkeysUtil.h"
#include "lcdDisplayUtil.h"
#include "gameHandler.h"

extern TM1638plus tm;

// Global variables
int num1, num2, correctAnswer;
int numCorrectCount = 0;
int numTheLEDs[8];
uint16_t numLedState = 0b0000000000000000;
int numTimeLeft = 30;
unsigned long numPreviousMillis = 0;
const int numCountdownSpeed = 1000;
int mathRightAnswer = 0;

void setup_number_guesser(TM1638plus &tm) {
  Serial.begin(115200);

  randomSeed(analogRead(A0));
  generateNumberQuestion();
}

// Function to generate a random number question
void generateNumberQuestion() {
  num1 = random(2, 9);
  num2 = random(1, num1);
  correctAnswer = num1 - num2;

  String question = String(num1) + " - " + String(num2) + " = ?";
  displayText(question, "Press the button");
}

// Runs the guessing number game itself
void playGame_number_guesser(TM1638plus &tm) {
  int pressedNumber = getPressedButton();

  if (pressedNumber == -1) {
    // No button press detected, exit function
    return;
  }

  // Small debuging data
  Serial.print("Detected Button: ");
  Serial.println(pressedNumber);

  if (pressedNumber == correctAnswer) {
    // Correct answer
    displayText("WOW", "That is Correct!");
    mathRightAnswer++;
    Serial.print("Correct Answer Count: ");
    Serial.println(mathRightAnswer);

    // Turn on the LED for the correct answer
    if (numCorrectCount < 8) {
       // Set LED position based on numCorrectCount
      numTheLEDs[numCorrectCount] = numCorrectCount + 1;
      setupLEDsFromArray(numCorrectCount + 1, numTheLEDs);
    }

    numCorrectCount++;

    // Proceed to next question
    if (numCorrectCount < 8) {
      // Waits for a bit before the next question
      delay(2000);
      generateNumberQuestion();
    } else {
      tm.displayText("WIN!");
      delay(2000);

      // Reset game-specific stuff
      numCorrectCount = 0;
      numTimeLeft = 30;
      numLedState = 0;
      tm.setLEDs(numLedState);

      // Tell the game its completed
      bool wasMathAWin = mathRightAnswer >= 4;
      finishCurrentGame(wasMathAWin, false);
    }

  } else {
    // Incorrect answer
    displayText("Sorry", "That is Wrong");

    delay(1000);
    // Subtract 1500 seconds for a wrong answer
    displayText("you should", "know this -1500");
    subtractTime(1500);

    // Proceed to next question
    numCorrectCount++;
    delay(2000);
    generateNumberQuestion();
  }
}
