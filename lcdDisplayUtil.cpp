#include "lcdDisplayUtil.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>


// Include the library for I2C LCD to acces the LCD
LiquidCrystal_I2C mylcd(0x27, 16, 2);


void lcdScreenSetup() {
  Serial.begin(115200);

  mylcd.init();
  mylcd.backlight();
  mylcd.clear();

  mylcd.clear();
  mylcd.setCursor(4, 0);
  mylcd.print("Welcome!");
}

// Global variable to display text on the screen
void displayText(String lineOne, String lineTwo) {
  int maxLength = 16;  // LCD screen width, assuming 16x2 LCD

  // Clear the screen initially
  mylcd.clear();

  // First, display both lines completely without scrolling (for the initial display)
  mylcd.setCursor(0, 0);
  mylcd.print(lineOne.substring(0, maxLength));
  mylcd.setCursor(0, 1);
  mylcd.print(lineTwo.substring(0, maxLength));
  delay(200);  // Display for 5 seconds

  // Scroll the first line if its too long
  if (lineOne.length() > maxLength) {
    // Loop for scrolling the first line
    for (int i = 0; i <= lineOne.length() - maxLength; i++) {
      mylcd.setCursor(0, 0);
      // Display part of the string
      mylcd.print(lineOne.substring(i, i + maxLength));
      // Delay to control scrolling speed
      delay(300);
    }
  }

  // Scroll the second line if it's too long
  if (lineTwo.length() > maxLength) {
    // Loop for scrolling the second line
    for (int i = 0; i <= lineTwo.length() - maxLength; i++) {
      mylcd.setCursor(0, 1);
      // Display part of the string
      mylcd.print(lineTwo.substring(i, i + maxLength));
       // Delay to control scrolling speed
      delay(300);
    }
  }

  // Once scrolling is done, just displays the full lines (no scrolling) after delay
  delay(200);
  mylcd.clear();
  mylcd.setCursor(0, 0);
  mylcd.print(lineOne.substring(0, maxLength));
  mylcd.setCursor(0, 1);
  mylcd.print(lineTwo.substring(0, maxLength));
}


void displayProximityBar(int distance) {
  // Max distance for the bar
  int maxDistance = 512;

  // Map distance so block count (0 to 16)
  int blocks = map(distance, 0, maxDistance, 16, 0);
  blocks = constrain(blocks, 0, 16);

  String bar = "";
  for (int i = 0; i < blocks; i++) {
    bar += (char)0xFF;
  }

  // Fill rest with spaces
  while (bar.length() < 16) {
    bar += " ";
  }

  // Print on bottom row
  mylcd.setCursor(0, 1);
  mylcd.print(bar);
}
