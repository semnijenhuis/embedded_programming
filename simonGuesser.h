#ifndef SIMON_GUESSER_H
#define SIMON_GUESSER_H

void setup_simon_guesser();
void playGame_simon_guesser();
int getSimonNumberForStep(int step);
bool displayLEDPattern(bool showedPattern);
void startNewRound();

#endif // SIMON_GUESSER_H
