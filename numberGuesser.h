#ifndef NUMBER_GUESSER_H
#define NUMBER_GUESSER_H

#include <TM1638plus.h>

void setup_number_guesser(TM1638plus &tm);
void playGame_number_guesser(TM1638plus &tm);
void generateNumberQuestion();

#endif
