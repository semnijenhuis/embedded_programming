#include "safeSimulator.h"
#include "LedKeysUtil.h"
#include "lcdDisplayUtil.h"
#include <TM1638plus.h>
#include "gameHandler.h"

extern TM1638plus tm;

const int potMaxValue = 1023;
const int ledCount = 8;
const int rangePerLED = (potMaxValue + 1) / ledCount;

const int potTolerance = 15;
const int blinkInterval = 500;
const int totalSteps = 3;

int correctPotValues[totalSteps];
int currentStep = 0;


unsigned long lastBlinkTime = 0;
unsigned long correctHoldStart = 0;
bool blinkState = false;
bool stepCompleted[totalSteps] = { false, false, false };

// setup the game values for the potentiometer
void setup_safe_simulator(TM1638plus &tm) {
  randomSeed(analogRead(A0));

  // Starts with out-of-range values
  int previousValues[totalSteps] = { -999, -999, -999 };

  for (int i = 0; i < totalSteps; i++) {
    int newValue;
    int newBlock;
    bool valid;

    do {
      valid = true;
      newValue = random(128, potMaxValue - 128);
      newBlock = newValue / rangePerLED;

      // Check against all previous values
      for (int j = 0; j < i; j++) {
        int prevValue = previousValues[j];
        int prevBlock = prevValue / rangePerLED;

        if (abs(newValue - prevValue) < 60 || newBlock == prevBlock) {
          valid = false;
          break;
        }
      }

    } while (!valid);

    correctPotValues[i] = newValue;
    previousValues[i] = newValue;
    stepCompleted[i] = false;

    Serial.print("Correct Value ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(newValue);
  }

  displayText("Safe Cracker", "Let's begin!");
}

// Runs the safe simulator game
void playGame_safe_simulator(TM1638plus &tm) {
  int potValue = analogRead(A0);
  Serial.print("Pot Value: ");
  Serial.println(potValue);
  Serial.print("Correct Value ");
  Serial.print(": ");
  Serial.println(correctPotValues[0]);

  int selectedLED = potValue / rangePerLED;

  // Reset global LED list for this loop
  int dynamicLEDs[totalSteps + 1];
  int ledCountInArray = 0;

  // Adds leds for completed steps
  for (int i = 0; i < currentStep; i++) {
    int lockedLED = correctPotValues[i] / rangePerLED;
    dynamicLEDs[ledCountInArray++] = lockedLED + 1;
  }

  int correctPotValue = correctPotValues[currentStep];
  int targetLED = correctPotValue / rangePerLED;
  int distance = abs(potValue - correctPotValue);
  displayProximityBar(distance);

  // Check if player is in correct range
  if (distance <= potTolerance) {
    unsigned long now = millis();

    if (stepCompleted[currentStep]) {
      dynamicLEDs[ledCountInArray++] = targetLED + 1;
    } else {
      // Blinking if he found it
      if (now - lastBlinkTime >= blinkInterval) {
        lastBlinkTime = now;
        blinkState = !blinkState;
      }

      if (blinkState) {
        dynamicLEDs[ledCountInArray++] = targetLED + 1;
      }

      // Hold timer, long enough on the step to complete it
      if (correctHoldStart == 0) {
        correctHoldStart = now;
      } else if (now - correctHoldStart >= 3000) {
        stepCompleted[currentStep] = true;
        displayText("Step Completed!", "Nice!");

        delay(1000);
        currentStep++;
        correctHoldStart = 0;
        blinkState = false;

        if (currentStep >= totalSteps) {
          displayText("SAFE OPENED!", "YOU WIN!");
          tm.displayText("WIN!");
          delay(2000);
          currentStep = 0;

          // Finish the game
          finishCurrentGame(true, false);
          return;
        }
      }
    }

  } else {
    // if its out of range
    dynamicLEDs[ledCountInArray++] = selectedLED + 1;
    correctHoldStart = 0;
  }

  // applying the new led state
  setupLEDsFromArray(ledCountInArray, dynamicLEDs);

  delay(100);
}
