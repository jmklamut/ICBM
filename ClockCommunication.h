
#ifndef ClockCommunication
#define ClockCommunication
#include <Arduino.h>

void recieveEvent(int bytes); //interrrupt that will activate when something is being sent

String decoder(); // decodes the incoming messages coming from clock

void set_ret(String x); //idk

void set_Zero(); // sets x = 5

void setPinOne(uint8_t i); // sets pin 37

void setPinTwo(uint8_t i); // sets pin 38

void setPinThree(uint8_t i);// sets pin 39

void nothing(); // all zero

void check(); // when there is a check

void pawnPromotion(); // when pawn is promoted

void sendKnight(); // sends knight

void error(); // sends rook

void sendBishop(); // sends bishop

void sendQueen(); // sends queen

void winningScreen(); // win screen 

void clockSetup();

#endif
