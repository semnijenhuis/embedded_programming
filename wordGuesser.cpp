#include "lcdDisplayUtil.h"
#include "LedKeysUtil.h"
#include <Arduino.h>
#include "wordGuesser.h"
#include "gameHandler.h"

struct WordPuzzle {
  String clueLine1;
  String clueLine2;
  String answer;
};

// All available puzzles
WordPuzzle puzzles[] = {
  { "The baker", "makes:", "BREAD" },
  { "The police", "writes:", "TICKET" },
  { "The dog", "likes to:", "RUN" },
  { "The singer", "sings:", "SONG" },
  { "The student", "learns:", "MATH" },
  { "The artist", "draws:", "SK   ETCH" },
  { "The doctor", "heals:", "WOUND" },
  { "The wizard", "casts:", "SPELL" }
};

const int totalPuzzles = sizeof(puzzles) / sizeof(puzzles[0]);
int currentPuzzle = 0;

String targetWord = "";
String enteredWord = "";
char currentChar = 'A';
int letterIndex = 0;

String clueLine1 = "";
String clueLine2 = "";

// Track correct answers
int correctAnswers[8];

int selectedPuzzles[3];
int puzzleIndex = 0;
int correctGuessedWords = 0;

// For smooth input
int smoothedValue = 0;
int lastStableValue = 0;
int lastIndex = -1;
const int hysteresis = 10;


extern int countdown;

void setup_word_guesser() {
  enteredWord = "";
  currentChar = 'A';
  letterIndex = 0;

  if (puzzleIndex == 0) {
    // Picks 3 new puzzles
    shufflePuzzles();
  }

  int selected = selectedPuzzles[puzzleIndex];
  targetWord = puzzles[selected].answer;
  clueLine1 = puzzles[selected].clueLine1;
  clueLine2 = puzzles[selected].clueLine2;

  displayText(clueLine1, clueLine2);
  delay(1000);
  updateDisplay();


  // Display the current puzzle succes in leds
  setupLEDsFromArray(correctGuessedWords, correctAnswers);
}

void shufflePuzzles() {
  int indices[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };

  for (int i = 7; i > 0; i--) {
    int j = random(i + 1);
    int temp = indices[i];
    indices[i] = indices[j];
    indices[j] = temp;
  }

  // Take the first 3 shuffled
  for (int k = 0; k < 3; k++) {
    selectedPuzzles[k] = indices[k];
  }

  puzzleIndex = 0;
  correctGuessedWords = 0;
}


// Update the display with the current word with the clue
void updateDisplay() {
  String shownWord = enteredWord;
  while (shownWord.length() < targetWord.length()) {
    if (shownWord.length() == letterIndex) {
      shownWord += currentChar;
    } else {
      shownWord += "_";
    }
  }

  String answerLine = clueLine2 + shownWord;
  displayText(clueLine1, answerLine);
}

// Function to play the word guesser game
void playGame_word_guesser() {
  int potValue = analogRead(A0);

  // Smooths out the input
  smoothedValue = (smoothedValue * 3 + potValue) / 4;

  if (abs(smoothedValue - lastStableValue) > hysteresis) {
    lastStableValue = smoothedValue;

    int index = map(smoothedValue, 0, 1023, 0, 25);
    index = constrain(index, 0, 25);

    if (index != lastIndex) {
      lastIndex = index;
      currentChar = 'A' + index;
      updateDisplay();
    }
  }

  // Checks the button choice
  int btn = getPressedButton();
  if (btn == 1) {
    enteredWord += currentChar;
    letterIndex++;

    if (enteredWord.length() >= targetWord.length()) {
      if (enteredWord == targetWord) {
        displayText("ACCESS GRANTED", enteredWord);

        if (correctGuessedWords < 3) {
          correctAnswers[correctGuessedWords] = puzzleIndex;
          correctGuessedWords++;
          setupLEDsFromArray(correctGuessedWords, correctAnswers);
        }
      } else {
        displayText("ACCESS DENIED", enteredWord);
        delay(1000);
        displayText("This was easy", "just why? -1000");
        subtractTime(1000);
      }

      delay(2000);
      puzzleIndex++;

      if (puzzleIndex >= 3) {
        // Game finished, based on how many correct guesses
        finishCurrentGame((correctGuessedWords >= 2), false);
      } else {
        setup_word_guesser();
      }
    } else {
      // updates the display with the new letter
      updateDisplay();
    }
  }
}
