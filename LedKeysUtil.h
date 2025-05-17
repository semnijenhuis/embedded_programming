#ifndef LEDKEYSUTIL_H
#define LEDKEYSUTIL_H

#include <Arduino.h>

extern int countdown;

void setupLEDsFromArray(int count, int leds[]);
void setupLedKeysUtil();
int getPressedButton();
void displayTimer();
void addTime(int seconds);
void subtractTime(int seconds);
void updateOpenDisplay(bool success);

#endif  //LEDKEYSUTIL_H
