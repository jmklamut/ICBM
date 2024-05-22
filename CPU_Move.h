#ifndef CPU_Move
#define CPU_Move
#include "string.h"
#include <Arduino.h>

bool setBlack(bool v); // returns if the player is black or not => switch pieces

bool getCheckMate(); //returns whether the move was checkmate

bool getCheck(); // returns whether the move was a check

bool getValid(); // returns whether the move sent was valid(PVP)

bool getMoveValid(); // returns true when the move is XXXX

String getCPUMove(); // returns the move made by cpu

bool getCPUPromote(); // if cpu makes promotion

char getPromote(); // what cpu selects for promotion

void setCPUPromote(bool c); //sets the promotion variable

void setPromote(char c); //sets the promotion char

void setPawn(String x); //sets pawn promotion for sending to chess cpu

void setTimeout(String x); //sends to the cpu if a timeout or surrender happens

bool getCast2King(); // get white king castle

bool getCast2Queen(); //get white queen castle

bool getCast1King(); // get black king castle

bool getCast1Queen(); // get black queen castle

bool recieveCPU(); //waits until it receives a variable from the CPU, returns true with invalid move made or move made passed all checks, false otherwise

bool getSend(); //returns true when it recieves something from serial

void setSend(bool f); //sets send variable

String chessToCor(String convert); //converts a string into the corresponding corridinates of a chess board

String corToChess(String convert); //converts a string into the corresponding corridinates of a chess board

void transmitCPU(String send); // sends a move to the chess computer with proper packet structure


#endif