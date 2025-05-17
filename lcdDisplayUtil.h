#ifndef LCDDISPLAYUTIL_H
#define LCDDISPLAYUTIL_H

#include <Arduino.h>

void lcdScreenSetup();
void displayText(String lineOne, String lineTwo);
void displayProximityBar(int distance);

#endif  //LCDDISPLAYUTIL_H
