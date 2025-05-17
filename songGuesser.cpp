#include "songGuesser.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TM1638plus.h>
#include "gameHandler.h"
#include "lcdDisplayUtil.h"
#include "LedKeysUtil.h"


#define BUZZER_PIN A1

// Note frequencies
#define NOTE_E7 2637
#define NOTE_E7_ 2489
#define NOTE_C7 2093
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_B6 1976
#define NOTE_D7 2349
#define NOTE_F7 2794
#define NOTE_A5 880
#define NOTE_E6 1319
#define NOTE_FS6 1479
#define NOTE_F6 1397
#define NOTE_C6 1047
#define NOTE_D6 1174
#define NOTE_CS7 2217
#define NOTE_DS7 2489
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_A7 3520
#define NOTE_GS6 1661


#define BUZZER_PIN A1
extern TM1638plus tm;
LiquidCrystal_I2C lcd(0x27, 16, 2);


int currentSong = 0;
int totalSongs = 8;
bool gameRunning = true;
int rightSongGuessed = 0;
int ledsOn[8];
int ledsOnCount = 0;

enum SongGuesserState {
  SONG_INTRO,
  SONG_PLAYING,
  SONG_WAIT_FOR_INPUT,
  SONG_FEEDBACK,
  SONG_DONE
};

SongGuesserState songState = SONG_INTRO;
unsigned long songStateStartTime = 0;

const char* songNames[] = {
  "Star Wars",
  "Harry Potter",
  "Mission Imp.",
  "Simpsons",
  "Mario",
  "Donkey Kong",
  "Pirates",
  "Jingle Bells"
};

const char* optionA[8] = {
  "Star Wars",
  "Harry Potter",
  "Mission Imp.",
  "Simpsons",
  "Mario",
  "Donkey Kong",
  "Pirates",
  "Jingle Bells"
};

const char* optionB[8] = {
  "Pirates",
  "Jingle Bells",
  "Harry Potter",
  "Mission Imp.",
  "Donkey Kong",
  "Mario",
  "Jingle Bells",
  "Star Wars"
};

// Function to get the correct answer for a song
char getCorrectAnswer(int songIndex) {
  if (strcmp(optionA[songIndex], songNames[songIndex]) == 0) {
    return 'A';
  } else {
    return 'B';
  }
}
void setup_song_guesser() {

}

// Function to play the game
void playGame_song_guesser() {
  switch (songState) {

    case SONG_INTRO:
      displayText("Song Guesser", "S1=A S2=B");
      delay(1000);
      songState = SONG_PLAYING;
      break;

    case SONG_PLAYING:
      if (currentSong >= totalSongs) {

        // Checks if the game was a fail or succes
        bool wasSongsAWin = rightSongGuessed >= 4;
        finishCurrentGame(wasSongsAWin, false);
        songState = SONG_DONE;
        break;
      }

      playSong(currentSong);
      displayOptions(currentSong);
      songState = SONG_WAIT_FOR_INPUT;
      break;

    case SONG_WAIT_FOR_INPUT:
      {
        uint8_t buttons = tm.readButtons();

        if (buttons & 0b00000001) {
          // S1 Pressed (Option A)
          processAnswer('A', currentSong);
          songStateStartTime = millis();
          songState = SONG_FEEDBACK;
        }
        else if (buttons & 0b00000010) {
          // S2 Pressed (Option B)
          processAnswer('B', currentSong);
          songStateStartTime = millis();
          songState = SONG_FEEDBACK;
        }
      }
      break;

    case SONG_FEEDBACK:
      if (millis() - songStateStartTime > 2000) {
        currentSong++;
        songState = SONG_PLAYING;
      }
      break;

    case SONG_DONE:
      break;
  }
}


// Function to play a specific song
void playSong(int songIndex) {
  switch (songIndex) {
    case 0:
      playStarWarsTheme();
      break;
    case 1:
      playHarryPotterTheme();
      break;
    case 2:
      playMissionImpossibleTheme();
      break;
    case 3:
      playSimpsonsTheme();
      break;
    case 4:
      playMarioTheme();
      break;
    case 5:
      playDonkeyKongTheme();
      break;
    case 6:
      playPiratesTheme();
      break;
    case 7:
      playJingleBells();
      break;
  }
}

// Function to display the two options a/b on the screen
void displayOptions(int songIndex) {
  displayText(String("A: ") + optionA[songIndex], String("B: ") + optionB[songIndex]);
}

// Function to check the users answer
void checkAnswer(int songIndex) {
  bool answered = false;

  while (!answered) {
    uint8_t buttons = tm.readButtons();

    // S1 Pressed (Option A)
    if (buttons & 0b00000001) {
      answered = true;
      processAnswer('A', songIndex);
    }

    // S2 Pressed (Option B)
    if (buttons & 0b00000010) {
      answered = true;
      processAnswer('B', songIndex);
    }
  }
}

// Function to check the users answer
void processAnswer(char choice, int songIndex) {
  if (choice == getCorrectAnswer(songIndex)) {
    displayText("Correct, it was:", songNames[songIndex]);
    rightSongGuessed++;
    Serial.print("Correct Answer Count: ");
    Serial.println(rightSongGuessed);

    // Turn on the LED for the correct answer
    ledsOn[ledsOnCount++] = songIndex + 1;
    setupLEDsFromArray(ledsOnCount, ledsOn);

  } else {
    displayText("Wrong, it was:", songNames[songIndex]);
    delay(1000);
    displayText("You had 50%", "there goes -1000");
    subtractTime(1000);
    delay(2000);
  }
}

void beep(int note, int duration) {
  // Play the tone on the buzzer pin
  tone(BUZZER_PIN, note, duration);
  // Add a small delay to separate the tones
  delay(duration * 1.3);
   // Stop the tone
  noTone(BUZZER_PIN);
}



void playMarioTheme() {
  // Part 1
  beep(NOTE_E7, 100);
  beep(NOTE_E7, 100);
  delay(100);
  beep(NOTE_E7, 100);
  delay(100);
  beep(NOTE_C7, 100);
  beep(NOTE_E7, 100);
  delay(100);
  beep(NOTE_G6, 100);
  delay(300);
  beep(NOTE_G6, 100);
  delay(300);

  // Part 2
  beep(NOTE_C7, 100);
  delay(100);
  beep(NOTE_G6, 100);
  delay(100);
  beep(NOTE_E6, 100);
  delay(100);
  beep(NOTE_A6, 100);
  beep(NOTE_B6, 100);
  beep(NOTE_A6, 100);
  beep(NOTE_GS6, 100);
  beep(NOTE_A6, 100);
  delay(100);
  beep(NOTE_E6, 100);
  delay(100);
  beep(NOTE_C7, 100);
  beep(NOTE_D7, 100);
  beep(NOTE_B6, 100);

  // Part 3
  delay(150);
  beep(NOTE_G6, 100);
  beep(NOTE_FS6, 100);
  beep(NOTE_E6, 100);
  beep(NOTE_FS6, 100);
  beep(NOTE_G6, 100);
  delay(150);
  beep(NOTE_E6, 100);
  beep(NOTE_G6, 100);
  beep(NOTE_A6, 100);
  delay(100);
  beep(NOTE_F7, 100);
  beep(NOTE_D7, 100);
  beep(NOTE_B6, 100);
  beep(NOTE_C7, 100);
}

void playDonkeyKongTheme() {
  beep(NOTE_C7, 200);
  beep(NOTE_G6, 200);
  beep(NOTE_E6, 200);
  beep(NOTE_A6, 400);
  delay(100);
  beep(NOTE_B6, 200);
  beep(NOTE_C7, 400);
  delay(200);
  beep(NOTE_E6, 200);
  beep(NOTE_G6, 200);
  beep(NOTE_A6, 200);
  beep(NOTE_F6, 400);
}

void playPiratesTheme() {
  beep(NOTE_E6, 150);
  beep(NOTE_G6, 150);
  beep(NOTE_A6, 150);
  beep(NOTE_A6, 150);
  beep(NOTE_B6, 300);
  beep(NOTE_C7, 300);
  delay(150);
  beep(NOTE_D7, 150);
  beep(NOTE_E7, 150);
  beep(NOTE_E7, 150);
  beep(NOTE_FS6, 150);
  beep(NOTE_G6, 300);
  beep(NOTE_A6, 300);
}

void playJingleBells() {
  beep(NOTE_E6, 200);
  beep(NOTE_E6, 200);
  beep(NOTE_E6, 400);
  delay(100);
  beep(NOTE_E6, 200);
  beep(NOTE_E6, 200);
  beep(NOTE_E6, 400);
  delay(100);
  beep(NOTE_E6, 200);
  beep(NOTE_G6, 200);
  beep(NOTE_C6, 200);
  beep(NOTE_D6, 200);
  beep(NOTE_E6, 600);
}

void playStarWarsTheme() {
  beep(NOTE_A6, 500);
  beep(NOTE_A6, 500);
  beep(NOTE_F6, 350);
  beep(NOTE_C7, 150);
  beep(NOTE_A6, 500);
  beep(NOTE_F6, 350);
  beep(NOTE_C7, 150);
  beep(NOTE_A6, 1000);

  beep(NOTE_E7, 500);
  beep(NOTE_E7, 500);
  beep(NOTE_E7, 500);
  beep(NOTE_F7, 350);
  beep(NOTE_C7, 150);
  beep(NOTE_GS6, 500);
  beep(NOTE_F6, 350);
  beep(NOTE_C7, 150);
  beep(NOTE_A6, 1000);
}

void playHarryPotterTheme() {
  beep(NOTE_B6, 400);
  beep(NOTE_E7, 800);
  beep(NOTE_G7, 400);
  beep(NOTE_FS7, 400);
  beep(NOTE_E7, 600);
  delay(200);
  beep(NOTE_B7, 400);
  beep(NOTE_A7, 200);
  beep(NOTE_FS7, 400);
  beep(NOTE_E7, 800);
}

void playMissionImpossibleTheme() {
  for (int i = 0; i < 2; i++) {
    beep(NOTE_E6, 150);
    delay(150);
    beep(NOTE_E6, 150);
    delay(150);
    beep(NOTE_E6, 150);
    delay(150);
  }

  beep(NOTE_C6, 150);
  delay(100);
  beep(NOTE_E6, 150);
  delay(100);
  beep(NOTE_G6, 400);
}

void playSimpsonsTheme() {
  beep(NOTE_B6, 150);
  beep(NOTE_E7, 300);
  delay(100);
  beep(NOTE_FS7, 150);
  beep(NOTE_D7, 300);
  delay(100);
  beep(NOTE_A6, 150);
  beep(NOTE_B6, 300);
  delay(200);
  beep(NOTE_E7, 150);
  beep(NOTE_C7, 300);
}

void playTetrisTheme() {
  beep(NOTE_E6, 300);
  beep(NOTE_E6, 300);
  beep(NOTE_E6, 300);
  beep(NOTE_C6, 300);
  beep(NOTE_E6, 300);
  beep(NOTE_G6, 300);
  beep(NOTE_GS6, 200);

  beep(NOTE_E6, 300);
  beep(NOTE_E6, 300);
  beep(NOTE_E6, 300);
  beep(NOTE_C6, 300);
  beep(NOTE_E6, 300);
  beep(NOTE_G6, 300);
  beep(NOTE_GS6, 200);

  beep(NOTE_E6, 300);
  beep(NOTE_E6, 300);
  beep(NOTE_E6, 300);
  beep(NOTE_C6, 300);
  beep(NOTE_E6, 300);
  beep(NOTE_G6, 300);
  beep(NOTE_A6, 200);

  beep(NOTE_F6, 300);
  beep(NOTE_E6, 200);
  beep(NOTE_C6, 300);
  beep(NOTE_E6, 200);
}
