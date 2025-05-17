#ifndef SONGGUESSER_H
#define SONGGUESSER_H

#include <Arduino.h>

void beep(int note, int duration);
void setup_song_guesser();
void playGame_song_guesser();

void playMarioTheme();
void playDonkeyKongTheme();
void playPiratesTheme();
void playJingleBells();
void playStarWarsTheme();
void playHarryPotterTheme();
void playMissionImpossibleTheme();
void playSimpsonsTheme();

void playSong(int songIndex);
void displayOptions(int songIndex);
void checkAnswer(int songIndex);
void processAnswer(char choice, int songIndex);
char getCorrectAnswer(int songIndex);


#endif
