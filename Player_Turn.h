#ifndef Player_Turn
#define Player_Turn
#include <Arduino.h>

void setBlackPlayer(); //sets board up for black player

void set_occupancy(int preRow, int preCol, int newRow, int newCol); // sets a square on the occupancy board

bool get_promotion(); //get promotion variable

void set_promotion(bool p); //sets promotion variable

char get_promote_type(); //get promotion piece type - checks what color it is

int get_promote_row(); // gets the row of the promoted piece

int get_promote_col(); // gets the col of the promoted piece

bool getLedInGrid(int row, int col); // gets the leds currenlty set on board

void setLedGrid(int row, int col, bool val); // sets the led grid square to a value

void setLedGridZero(); //set the led screen to zero

void turnOffAll(); //turn off all the leds

void turnOnLED(int row, int col); //turn on specific led

void turnKnightLEDs(int row, int col); //turn on the leds for the knight

void turnBishopLEDs(int row, int col, double time); // turn on leds for the bishops

void turnRookLEDs(int row, int col, double time); // turn on leds for the rook

void turnQueenLEDs(int row, int col); // turn on queen leds

void turnPawnLEDs(int row, int col); // turn on pawn led

void turnKingLEDs(int row, int col); // turn on king led

void turnOnPieceLED(int row, int col); // turn on the leds based on piece on that grid

bool checkKnight(int current_row, int current_col); //check if placed knight valid

bool checkBishop(int current_row, int current_col); //check if placed bishop valid

bool checkRook(int current_row, int current_col); //check if placed rook valid

bool checkQueen(int current_row, int current_col); //check if placed queen valid

bool checkPawn(int current_row, int current_col); //check if placed pawn valid

bool checkKing(int current_row, int current_col); //check if placed king valid

bool checkPiece(int row, int col, char k); // runs check function based on piece

bool checkPieceColor(int row, int col); // check piece color

bool checkHandPiece(); // check the piece in hand color

void setPiece(int preRow, int preCol, int newRow, int newCol, char piece); // move a piece on the board

void setPlaceZero(int row, int col); // sets a square zero

void setPieceSpecific(int row, int col, char k); // sets a square to a different piece

char getPiece(int row, int col); // get a piece value on based on the grid

String sendAPiece(); // sends the preivous piece in hand to the fsm

bool moveMade(); // sends wheter a piece was picked up and put down for FSM => if yes check to see if clock is pressed

void set_moveMade(bool f); // sets the move made

void reset(); //resets the game for a new round

void revert(); //returns true until switches are detected 

void win(); // set of LEDs when someone wins

bool readSquare(int row, int col); //reads a specific square to see if it is occupied or not

void readBoardState(); //reads the board state based off switches

void compareBoards(); //lights pieces and compared boards

void printBoardState(); // prints the current board state

void setupPlayer(); //sets up the leds on the board

void test();

#endif