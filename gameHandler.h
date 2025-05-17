#ifndef GAMEHANDLER_H
#define GAMEHANDLER_H

#include <Arduino.h>

extern int successfulGames;

void setupGameHandler();
void playNextGame();
void finishCurrentGame(bool success, bool lastGameComplete);
void displayEndCredits();

#endif  //GAMEHANDLER_H
