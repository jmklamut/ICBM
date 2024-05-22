#include "Player_Turn.h"
#include "string.h"

//////WARNING!!!! BLACK IS NOW WHITE AND WHITE IS NOW BLACK!!!!!

// Define the pins for rows and columns
const int rowPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
const int colPins[] = {10, 11, 12, 13, 22, 23, 24, 25};
const int sel0= 26;
const int sel1 = 27;
const int sel2 = 28;

const int colSel[] = {29,30,31,32,33,34,35,36};

struct cor{
  int row;
  int col;
  bool pick_up;
  char type;
};

cor other_hand{0,0,false,'\0'};

cor piece_in_hand{0,0,false,'\0'};

cor send_piece{0,0,false,'\0'};

cor promote{0,0, false, '\0'};


// Define the grid representing the chessboard
const int numRows = 8;
const int numCols = 8;
const int twoWhite = 2;
const int twoBlack = 7;
const int numcolSel = 8;

bool move_made = false;
bool promotion = false;
bool cast1 = false;
bool cast2 = false;
bool cast2yesK = false;
bool cast2yesQ = false;
bool cast1yesK = false;
bool cast1yesQ = false;

int col = 1;
bool led_grid[numRows][numCols] = {
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0}
};

bool prev_occupied_squares[numRows][numCols] = {
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 1},  
  {1, 1, 1, 1, 1, 1, 1, 1}
};

bool current_occupied_squares[numRows][numCols] = {
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 1},  
  {1, 1, 1, 1, 1, 1, 1, 1}
};

char chessboard[numRows][numCols] = { // k = knight, b = bishop, q = queen, p = pawn, r = rook, c = king
    { 'R', 'K', 'B', 'Q', 'C', 'B', 'K', 'R'}, //A8 to H8
    { 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'},
    { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'},
    { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'},
    { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'},
    { 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    { 'r', 'k', 'b', 'q', 'c', 'b', 'k', 'r'} //A1 to H1
};

void setBlackPlayer(){
  char white[8] = {'R','K','B','C','Q','B','K','R'};
  char black[8] = {'r','k','b','c','q','b','k','r'};
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      if(i == 0){
        chessboard[i][j] = white[j];
        prev_occupied_squares[i][j] = 1;
        current_occupied_squares[i][j] = 1;
      }
      else if(i == 1){
        chessboard[i][j] = 'P';
        prev_occupied_squares[i][j] = 1;
        current_occupied_squares[i][j] = 1;  
      }
      else if(i == 6){
        chessboard[i][j] = 'p';
        prev_occupied_squares[i][j] = 1;
        current_occupied_squares[i][j] = 1;       
      }
      else if(i == 7){
        chessboard[i][j] = black[j];
        prev_occupied_squares[i][j] = 1;
        current_occupied_squares[i][j] = 1;         
      }
      else{
        chessboard[i][j] = '\0';
        prev_occupied_squares[i][j] = 0;
        current_occupied_squares[i][j] = 0;       
      }
    }
  }
}

void set_occupancy(int preRow, int preCol, int newRow, int newCol){
  prev_occupied_squares[preRow][preCol] = 0;
  prev_occupied_squares[newRow][newCol] = 1;
  current_occupied_squares[preRow][preCol] = 0;
  current_occupied_squares[newRow][newCol] = 1;
}

bool get_promotion(){
  return promotion;
}

void set_promotion(bool p){
  promotion = p;
}

char get_promote_type(){
  return promote.type;
}

int get_promote_row(){
  return promote.row;
}

int get_promote_col(){
  return promote.col;
}

bool getLedInGrid(int row, int col){
  if(led_grid[row][col]){
    return true;
  }
  return false;
}

void setLedGrid(int row, int col, bool val){
  led_grid[row-1][col-1] = val;
}

void setLedGridZero(){
  for(int i = 0; i < numRows; i++){
    for(int j = 0; j < numCols; j++){
      led_grid[i][j] = 0;
    }
  }
}

void turnOffAll(){
  for (int i = 0; i < numRows; i++) {
    digitalWrite(rowPins[i], LOW);
  }
  for (int j = 0; j < numCols; j++) {
    digitalWrite(colPins[j], HIGH);
  }
}

void turnOnLED(int row, int col) {
  // Turn off all LEDs
  turnOffAll();
  setLedGridZero();
  
  // Turn on the specified LED
  digitalWrite(rowPins[row-1], HIGH);
  digitalWrite(colPins[col-1], LOW);
  setLedGrid(row,col,1);
  
}

void turnKnightLEDs(int row, int col){
  bool color = checkPieceColor(row, col);
  turnOnLED(row, col);
  delay(1);
  if(row+2 <= numRows && col+1 <= numCols && (color != checkPieceColor(row+2,col+1) || chessboard[row+1][col] == '\0')){
    turnOnLED(row+2,col+1);
    delay(1);
  }
  if(row-2 > 0 && col+1 <= numCols && (color != checkPieceColor(row-2,col+1) || chessboard[row-3][col] == '\0')){
    turnOnLED(row-2,col+1);
    delay(1);
  }
  if(row+2 <= numRows && col-1 > 0 && (color != checkPieceColor(row+2,col-1) || chessboard[row+1][col-2] == '\0')){
    turnOnLED(row+2,col-1);
    delay(1);
  }
  if(row-2 > 0 && col-1 > 0 && (color != checkPieceColor(row-2,col-1) || chessboard[row-3][col-2] == '\0')){
    turnOnLED(row-2,col-1);
    delay(1);
  }
  if(row+1 <= numRows && col+2 <= numCols && (color != checkPieceColor(row+1,col+2) || chessboard[row][col+1] == '\0')){
    turnOnLED(row+1,col+2);
    delay(1);
  }
  if(row-1 > 0 && col+2 <= numCols && (color != checkPieceColor(row-1,col+2) || chessboard[row-2][col+1] == '\0')){
    turnOnLED(row-1,col+2);
    delay(1);
  }
  if(row+1 <= numRows && col-2 > 0 && (color != checkPieceColor(row+1,col-2) || chessboard[row][col-3] == '\0')){
    turnOnLED(row+1,col-2);
    delay(1);
  }
  if(row-1 > 0 && col-2 > 0 && (color != checkPieceColor(row-1,col-2) || chessboard[row-2][col-3] == '\0')){
    turnOnLED(row-1,col-2);
    delay(1);
  }
}

void turnBishopLEDs(int row, int col, double time) {
  turnOnLED(row, col);
  delay(1);
  bool color = checkPieceColor(row, col);
  // Diagonal movements
  for (int i = 1; row + i <= numRows && col + i <= numCols; i++) {
    if(chessboard[row-1+i][col-1+i] == '\0'){
      turnOnLED(row + i, col + i);
      delay(time);
    }
    else if(color != checkPieceColor(row+i,col+i)){
      turnOnLED(row + i, col + i);
      delay(time);
      break;
    }
    else{
      break;
    }
  }
  for (int i = 1; row - i > 0 && col - i > 0; i++) {
    if(chessboard[row-1-i][col-1-i] == '\0'){
      turnOnLED(row - i, col - i);
      delay(time);
    }
    else if(color != checkPieceColor(row-i,col-i)){
      turnOnLED(row - i, col - i);
      delay(time);
      break;
    }
    else{
      break;
    }
  }
  for (int i = 1; row + i <= numRows && col - i > 0; i++) {
    if(chessboard[row-1+i][col-1-i] == '\0'){
      turnOnLED(row + i, col - i);
      delay(time);
    }
    else if(color != checkPieceColor(row+i,col-i)){
      turnOnLED(row + i, col - i);
      delay(time);
      break;
    }
    else{
      break;
    }
  }
  for (int i = 1; row - i > 0 && col + i <= numCols; i++) {
    if(chessboard[row-1-i][col-1+i] == '\0'){
      turnOnLED(row - i, col + i);
      delay(time);
    }
    else if(color != checkPieceColor(row-i,col+i)){
      turnOnLED(row - i, col + i);
      delay(time);
      break;
    }
    else{
      break;
    }
  }
}

void turnRookLEDs(int row, int col, double time) {
  turnOnLED(row, col);
  delay(1);
  bool color = checkPieceColor(row, col);
  // Horizontal movements (left)
  for (int i = col - 1; i >= 0; i--){
    if(i+1 == col){
      //skip
    }
    else if(chessboard[row - 1][i] == '\0') {
      turnOnLED(row, i + 1);
      delay(time);
    } else if(color != checkPieceColor(row, i+1)){
      turnOnLED(row, i + 1);
      delay(time);
      break; // Stop if encountering an occupied block
    }
    else{
      break;
    }
  }

  // Horizontal movements (right)
  for (int i = col; i <= numCols; i++) {
    if(i == col){
      //skip
    }
    else if (chessboard[row - 1][i-1] == '\0') {
      turnOnLED(row, i);
      delay(time);
    } else if(color != checkPieceColor(row, i)){
      turnOnLED(row, i);
      delay(time);
      break; // Stop if encountering an occupied block
    }
    else{
      break;
    }
  }

  // Vertical movements (up)
  for (int i = row - 1; i >= 0; i--) {
    if(i+1 == row){
      //skip
    }
    else if (chessboard[i][col - 1] == '\0') {
      turnOnLED(i + 1, col);
      delay(time);
    } else if(color != checkPieceColor(i+1,col)){
      turnOnLED(i + 1, col);
      delay(time);
      break; // Stop if encountering an occupied block
    }
    else{
      break;
    }
  }

  // Vertical movements (down)
  for (int i = row; i <= numRows; i++) {
    if(i == row){
      //skip
    }
    else if (chessboard[i-1][col - 1] == '\0') {
      turnOnLED(i, col);
      delay(time);
    } else if(color != checkPieceColor(i,col)){
      turnOnLED(i, col);
      delay(time);
      break; // Stop if encountering an occupied block
    }
    else{
      break;
    }
  }
}

void turnQueenLEDs(int row, int col){
  turnRookLEDs(row,col,0.5);
  turnBishopLEDs(row,col,0.5);
}

void turnPawnLEDs(int row, int col) {

  // Turn on LED for the pawn's current position
  turnOnLED(row, col);
  delay(1);
  bool color = checkPieceColor(row, col);

  if (!isLowerCase(chessboard[row-1][col-1])) {
    if(row == twoWhite){
      if (row + 2 <= numRows && chessboard[row][col-1] == '\0' && chessboard[row+1][col - 1] == '\0') {
        turnOnLED(row+2, col);
        delay(1);
      }   
    }
    if (row + 1 <= numRows && chessboard[row][col-1] == '\0') {
      turnOnLED(row+1, col);
      delay(1);
    }
    if(row + 1 <= numRows && col-1 > 0 && color != checkPieceColor(row + 1, col-1)){
      turnOnLED(row+1, col - 1);
      delay(1);
    }
    if(col + 1 <= numCols && row+1 <= numRows && color != checkPieceColor(row + 1, col+1)){
      turnOnLED(row+1, col + 1);
      delay(1);
    }  
  }
  else{
    if(row == twoBlack){
      if (row - 2 > 0 && chessboard[row-2][col - 1] == '\0' && chessboard[row-3][col - 1] == '\0') {
        turnOnLED(row-2, col);
        delay(1);
      }
    }
    if (row - 1 > 0 && chessboard[row-2][col - 1] == '\0') {
      turnOnLED(row-1, col);
      delay(1);
    }
    if(col - 1 > 0 && row-1 > 0 && color != checkPieceColor(row - 1, col-1) && chessboard[row-2][col - 2] != '\0'){
      turnOnLED(row-1, col - 1);
      delay(1);
    } 
    if(row - 1 > 0 && col+1 <= numCols && color != checkPieceColor(row - 1, col+1) && chessboard[row-2][col] != '\0'){
      turnOnLED(row-1, col + 1);
      delay(1);
    } 
  }
}

void turnKingLEDs(int row, int col){
  turnOnLED(row, col);
  delay(1);
  bool color = checkPieceColor(row, col);
  if(row+1 <= numRows && col+1 <= numCols && (color != checkPieceColor(row+1,col+1) || chessboard[row][col] == '\0')){
    turnOnLED(row+1,col+1);
    delay(1);
  }
  if(row-1 > 0 && col+1 <= numCols && (color != checkPieceColor(row-1,col+1) || chessboard[row-2][col] == '\0')){
    turnOnLED(row-1,col+1);
    delay(1);
  }
  if(row+1 <= numRows && col-1 > 0 && (color != checkPieceColor(row+1,col-1) || chessboard[row][col-2] == '\0')){
    turnOnLED(row+1,col-1);
    delay(1);
  }
  if(row-1 > 0 && col-1 > 0 && (color != checkPieceColor(row-1,col-1) || chessboard[row-2][col-2] == '\0')){
    turnOnLED(row-1,col-1);
    delay(1);
  }
  if(row+1 <= numRows && (color != checkPieceColor(row+1,col) || chessboard[row][col-1] == '\0')){
    turnOnLED(row+1,col);
    delay(1);
  }
  if(row-1 > 0 && (color != checkPieceColor(row-1,col) || chessboard[row-2][col-1] == '\0')){
    turnOnLED(row-1,col);
    delay(1);
  }
  if(col-1 > 0 && (color != checkPieceColor(row,col-1) || chessboard[row-1][col-2] == '\0')){
    turnOnLED(row,col-1);
    delay(1);
  }
  if(col+1 <= numCols && (color != checkPieceColor(row,col+1) || chessboard[row-1][col] == '\0')){
    turnOnLED(row,col+1);
    delay(1);
  }
  if(!color){
    if(row == 8 && col == 5){
      if(chessboard[7][1] == '\0' && chessboard[7][2] == '\0' && chessboard[7][3] == '\0' && chessboard[7][0] == 'r'){
        //Serial.println("Here Cast2 Queen");
        cast2 = true;
        turnOnLED(8,3);
        delay(1);
      }
      if(chessboard[7][5] == '\0' && chessboard[7][6] == '\0' && chessboard[7][7] == 'r'){
        //Serial.println("Here Cast2 King");
        cast2 = true;
        turnOnLED(8,7);
        delay(1);
      }
    }
  }
  else if(color){
    if(row == 1 && col == 5){
      if(chessboard[0][1] == '\0' && chessboard[0][2] == '\0' && chessboard[0][3] == '\0' && chessboard[0][0] == 'R'){
        //Serial.println("Here Cast1 Queen");
        cast1 = true;
        turnOnLED(1,3);
        delay(1);
      }
      if(chessboard[0][5] == '\0' && chessboard[0][6] == '\0' && chessboard[0][7] == 'R'){
        //Serial.println("Here Cast1 King");
        cast1 = true;
        turnOnLED(1,7);
        delay(1);
      }
    }
  }
}

void turnOnPieceLED(int row, int col){
  char k = chessboard[row-1][col-1];
  if(k == 'Q' || k == 'q'){
    turnQueenLEDs(row,col);
  }
  else if(k == 'r' || k == 'R'){
    turnRookLEDs(row,col,1);
  }
  else if(k == 'b' || k == 'B'){
    turnBishopLEDs(row,col,1);
  }
  else if(k == 'k' || k == 'K'){
    turnKnightLEDs(row,col);
  }
  else if(k == 'p' || k == 'P'){
    turnPawnLEDs(row,col);
  }
  else if(k == 'c' || k == 'C'){
    turnKingLEDs(row,col);
  }
}

bool checkKnight(int current_row, int current_col){
  bool color = checkHandPiece();
  int row = piece_in_hand.row+1;
  int col = piece_in_hand.col+1;
  if(current_row == row && current_col == col){
    return true;
  }
  if(row+2 <= numRows && col+1 <= numCols && (color != checkPieceColor(row+2,col+1) || chessboard[row+1][col] == '\0')){
    if(current_row == row+2 && current_col == col+1){
      return true;
    }
  }
  if(row-2 > 0 && col+1 <= numCols && (color != checkPieceColor(row-2,col+1) || chessboard[row-3][col] == '\0')){
    if(current_row == row-2 && current_col == col+1){
      return true;
    }
  }
  if(row+2 <= numRows && col-1 > 0 && (color != checkPieceColor(row+2,col-1) || chessboard[row+1][col-2] == '\0')){
    if(current_row == row+2 && current_col == col-1){
      return true;
    }
  }
  if(row-2 > 0 && col-1 > 0 && (color != checkPieceColor(row-2,col-1) || chessboard[row-3][col-2] == '\0')){
    if(current_row == row-2 && current_col == col-1){
      return true;
    }
  }
  if(row+1 <= numRows && col+2 <= numCols && (color != checkPieceColor(row+1,col+2) || chessboard[row][col+1] == '\0')){
    if(current_row == row+1 && current_col == col+2){
      return true;
    }
  }
  if(row-1 > 0 && col+2 <= numCols && (color != checkPieceColor(row-1,col+2) || chessboard[row-2][col+1] == '\0')){
    if(current_row == row-1 && current_col == col+2){
      return true;
    }
  }
  if(row+1 <= numRows && col-2 > 0 && (color != checkPieceColor(row+1,col-2) || chessboard[row][col-3] == '\0')){
    if(current_row == row+1 && current_col == col-2){
      return true;
    }
  }
  if(row-1 > 0 && col-2 > 0 && (color != checkPieceColor(row-1,col-2) || chessboard[row-2][col-3] == '\0')){
    if(current_row == row-1 && current_col == col-2){
      return true;
    }
  }
  return false;
}

bool checkBishop(int current_row, int current_col){

  bool color = checkHandPiece();
  int row = piece_in_hand.row+1;
  int col = piece_in_hand.col+1;
  if(current_row == row && current_col == col){
    return true;
  }
  // Diagonal movements
  for (int i = 1; row + i <= numRows && col + i <= numCols; i++) {
    if(chessboard[row-1+i][col-1+i] == '\0'){
      if(current_row == row+i && current_col == col+i){
        return true;
      }
    }
    else if(color != checkPieceColor(row+i,col+i)){
      if(current_row == row+i && current_col == col+i){
        return true;
      }
      break;
    }
    else{
      break;
    }
  }
  for (int i = 1; row - i > 0 && col - i > 0; i++) {
    if(chessboard[row-1-i][col-1-i] == '\0'){
      if(current_row == row-i && current_col == col-i){
        return true;
      }
    }
    else if(color != checkPieceColor(row-i,col-i)){
      if(current_row == row-i && current_col == col-i){
        return true;
      }
      break;
    }
    else{
      break;
    }
  }
  for (int i = 1; row + i <= numRows && col - i > 0; i++) {
    if(chessboard[row-1+i][col-1-i] == '\0'){
      if(current_row == row+i && current_col == col-i){
        return true;
      }
    }
    else if(color != checkPieceColor(row+i,col-i)){
      if(current_row == row+i && current_col == col-i){
        return true;
      }
      break;
    }
    else{
      break;
    }
  }
  for (int i = 1; row - i > 0 && col + i <= numCols; i++) {
    if(chessboard[row-1-i][col-1+i] == '\0'){
      if(current_row == row-i && current_col == col+i){
        return true;
      }
    }
    else if(color != checkPieceColor(row-i,col+i)){
      if(current_row == row-i && current_col == col+i){
        return true;
      }
      break;
    }
    else{
      break;
    }
  }
  return false;
}

bool checkRook(int current_row, int current_col){

  bool color = checkHandPiece();
  int row = piece_in_hand.row+1;
  int col = piece_in_hand.col+1;
  if(current_row == row && current_col == col){
    return true;
  }
  // Horizontal movements (left)
  for (int i = col - 1; i >= 0; i--){
    if(i+1 == col){
      //skip
    }
    else if(chessboard[row - 1][i] == '\0') {
      if(current_row == row && current_col == i+1){
        return true;
      }
    } else if(color != checkPieceColor(row, i+1)){
      if(current_row == row && current_col == i+1){
        return true;
      }
      break; // Stop if encountering an occupied block
    }
    else{
      break;
    }
  }

  // Horizontal movements (right)
  for (int i = col; i <= numCols; i++) {
    if(i == col){
      //skip
    }
    else if (chessboard[row - 1][i-1] == '\0') {
      if(current_row == row && current_col == i){
        return true;
      }
    } 
    else if(color != checkPieceColor(row, i)){
      if(current_row == row+1 && current_col == i){
        return true;
      }
      break; // Stop if encountering an occupied block
    }
    else{
      break;
    }
  }

  // Vertical movements (up)
  for (int i = row - 1; i >= 0; i--) {
    if(i+1 == row){
      //skip
    }
    else if (chessboard[i][col - 1] == '\0') {
      if(current_row == i+1 && current_col == col){
        return true;
      }
    } else if(color != checkPieceColor(i+1,col)){
      if(current_row == i+1 && current_col == col){
        return true;
      }
      break; // Stop if encountering an occupied block
    }
    else{
      break;
    }
  }

  // Vertical movements (down)
  for (int i = row; i <= numRows; i++) {
    if(i == row){
      //skip
    }
    else if (chessboard[i-1][col - 1] == '\0') {
      if(current_row == i && current_col == col){
        return true;
      }
    } else if(color != checkPieceColor(i,col)){
      if(current_row == i && current_col == col){
        return true;
      }
      break; // Stop if encountering an occupied block
    }
    else{
      break;
    }
  }
  return false;
}

bool checkQueen(int current_row, int current_col){
  bool b = checkBishop(current_row, current_col);
  bool r = checkRook(current_row, current_col);
  if(b || r){
    return true;
  }
  return false;
}

bool checkPawn(int current_row, int current_col){
  bool color = checkHandPiece();
  int row = piece_in_hand.row+1;
  int col = piece_in_hand.col+1;
  //Serial.println(color);
  if(current_row == row && current_col == col){
    return true;
  }

  if (color) {
    if(row == twoWhite){
      if (row + 2 <= numRows && chessboard[row][col-1] == '\0' && chessboard[row+1][col] == '\0') {
        if(current_row == row+2 && current_col == col){
          return true;
        }
      }   
    }
    if (row + 1 <= numRows && chessboard[row][col-1] == '\0') {
        if(current_row == row+1 && current_col == col){
          if(row == twoBlack){
            promotion = true;
            promote.row = current_row;
            promote.col = current_col;
            promote.type = 'P';
          }
          return true;
        }
    }
    if(row + 1 <= numRows && col-1 > 0 && color != checkPieceColor(row + 1, col-1)){
        if(current_row == row+1 && current_col == col-1){
          if(row == twoBlack){
            promotion = true;
            promote.row = current_row;
            promote.col = current_col;
            promote.type = 'P';
          }
          return true;
        }
    }
    if(col + 1 <= numCols && row+1 <= numRows && color != checkPieceColor(row + 1, col+1)){
        if(current_row == row+1 && current_col == col+1){
          if(row == twoBlack){
            promotion = true;
            promote.row = current_row;
            promote.col = current_col;
            promote.type = 'P';
          }
          return true;
        }
    }  
  }
  else{
    if(row == twoBlack){
      if (row - 2 > 0 && chessboard[row-2][col - 1] == '\0' && chessboard[row-3][col - 1] == '\0') {
        if(current_row == row-2 && current_col == col){
          return true;
        }
      }
    }
    if (row - 1 > 0 && chessboard[row-2][col - 1] == '\0') {
        if(current_row == row-1 && current_col == col){
          if(row == twoWhite){
            promotion = true;
            promote.row = current_row;
            promote.col = current_col;
            promote.type = 'p';
          }
          return true;
        }
    }
    if(col - 1 > 0 && row-1 > 0 && color != checkPieceColor(row - 1, col-1) && chessboard[row-2][col - 2] != '\0'){
        if(current_row == row-1 && current_col == col-1){
          if(row == twoWhite){
            promotion = true;
            promote.row = current_row;
            promote.col = current_col;
            promote.type = 'p';
          }
          return true;
        }
    } 
    if(col + 1 <= numCols && row-1 <= numRows && color != checkPieceColor(row - 1, col+1) && chessboard[row-2][col] != '\0'){
        if(current_row == row-1 && current_col == col+1){
          if(row == twoWhite){
            promotion = true;
            promote.row = current_row;
            promote.col = current_col;
            promote.type = 'p';
          }
          return true;
        }
    } 
  }
  return false;
}

bool checkKing(int current_row, int current_col){
  int row = piece_in_hand.row + 1;
  int col = piece_in_hand.col + 1;
  bool color = checkHandPiece(); //this could be invalid here
  if(current_row == row && current_col == col){
      return true;
  }
  if(row+1 <= numRows && col+1 <= numCols && (color != checkPieceColor(row+1,col+1) || chessboard[row][col] == '\0')){
    if(current_row == row+1 && current_col == col+1){
      return true;
    }
  }
  if(row-1 > 0 && col+1 <= numCols && (color != checkPieceColor(row-1,col+1) || chessboard[row-2][col] == '\0')){
    if(current_row == row-1 && current_col == col+1){
      return true;
    }
  }
  if(row+1 <= numRows && col-1 > 0 && (color != checkPieceColor(row+1,col-1) || chessboard[row][col-2] == '\0')){
    if(current_row == row+1 && current_col == col-1){
      return true;
    }
  }
  if(row-1 > 0 && col-1 > 0 && (color != checkPieceColor(row-1,col-1) || chessboard[row-2][col-2] == '\0')){
    if(current_row == row-1 && current_col == col-1){
      return true;
    }
  }
  if(row+1 <= numRows && (color != checkPieceColor(row+1,col) || chessboard[row][col-1] == '\0')){
    if(current_row == row+1 && current_col == col){
      return true;
    }
  }
  if(row-1 > 0 && (color != checkPieceColor(row-1,col) || chessboard[row-2][col-1] == '\0')){
    if(current_row == row-1 && current_col == col){
      return true;
    }
  }
  if(col-1 > 0 && (color != checkPieceColor(row,col-1) || chessboard[row-1][col-2] == '\0')){
    if(current_row == row && current_col == col-1){
      return true;
    }
  }
  if(col+1 <= numCols && (color != checkPieceColor(row,col+1) || chessboard[row-1][col] == '\0')){
    if(current_row == row && current_col == col+1){
      return true;
    }
  }
  if(!color){
    if(row == 8 && col == 5){
      if(cast2 && current_col == 3 && current_row == row){
        cast2 = false;
        cast2yesQ = true;
        return true;
      }
      else if(cast2 && current_col == 7 && current_row == row){
        cast2 = false;
        cast2yesK = true;
        return true;
      }
    }
  }
  else if(color){
    if(row == 1 && col == 5){
      if(cast1 && current_col == 3 && current_row == row){
        cast1 = false;
        cast1yesQ = true;
        return true;
      }
      else if(cast1 && current_col == 7 && current_row == row){
        cast1 = false;
        cast1yesK = true;
        return true;        
      }
    }
  }

  return false;
}

bool checkPiece(int row, int col, char k){
  if(k == 'Q' || k == 'q'){
    return checkQueen(row,col);
  }
  else if(k == 'r' || k == 'R'){
    return checkRook(row,col);
  }
  else if(k == 'b' || k == 'B'){
    return checkBishop(row,col);
  }
  else if(k == 'k' || k == 'K'){
    return checkKnight(row,col);
  }
  else if(k == 'p' || k == 'P'){
    return checkPawn(row,col);
  }
  else if(k == 'c' || k == 'C'){
    return checkKing(row,col);
  }
  else{
    return true;
  }
}
// true = white, false = black
bool checkPieceColor(int row, int col){
  if(isLowerCase(chessboard[row-1][col-1])){
    return false;
  }
  return true;
}

bool checkHandPiece(){
  if(isLowerCase(piece_in_hand.type)){
    return false;
  }
  return true;
}

void setPiece(int preRow, int preCol, int newRow, int newCol, char piece){
  chessboard[preRow][preCol] = '\0';
  chessboard[newRow][newCol] = piece;
  set_occupancy(preRow,preCol,newRow,newCol);
  //printBoardState();
}

void setPlaceZero(int row, int col){
  chessboard[row][col] = '\0';
}

void setPieceSpecific(int row, int col, char k){
  chessboard[row][col] = k;
  //printBoardState();
}

char getPiece(int row, int col){
  return chessboard[row][col];
}

String sendAPiece(){
  String i = String(9-send_piece.row-1);
  String j = String(send_piece.col+1);
  String x = String(9-other_hand.row-1);
  String y = String(other_hand.col+1);
  //send_piece.row = 0;
  //send_piece.col = 0;
  return y + x + j + i;
}

bool moveMade(){
  return move_made;
}

void set_moveMade(bool f){
  move_made = f;
}

void reset(){
  char white[8] = {'R','K','B','Q','C','B','K','R'};
  char black[8] = {'r','k','b','q','c','b','k','r'};
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      if(i == 0){
        chessboard[i][j] = white[j];
        prev_occupied_squares[i][j] = 1;
        current_occupied_squares[i][j] = 1;
      }
      else if(i == 1){
        chessboard[i][j] = 'P';
        prev_occupied_squares[i][j] = 1;
        current_occupied_squares[i][j] = 1;  
      }
      else if(i == 6){
        chessboard[i][j] = 'p';
        prev_occupied_squares[i][j] = 1;
        current_occupied_squares[i][j] = 1;       
      }
      else if(i == 7){
        chessboard[i][j] = black[j];
        prev_occupied_squares[i][j] = 1;
        current_occupied_squares[i][j] = 1;         
      }
      else{
        chessboard[i][j] = '\0';
        prev_occupied_squares[i][j] = 0;
        current_occupied_squares[i][j] = 0;       
      }
    }
  }


  other_hand.row = 0;
  other_hand.col = 0;
  other_hand.pick_up = 0;
  other_hand.type = '\0';

  piece_in_hand.row = 0;
  piece_in_hand.col = 0;
  piece_in_hand.pick_up = 0;
  piece_in_hand.type = '\0';

  send_piece.row = 0;
  send_piece.col = 0;
  send_piece.pick_up = 0;
  send_piece.type = '\0';

  promote.row = 0;
  promote.col = 0;
  promote.pick_up = 0;
  promote.type = '\0';
  
  move_made = false;
  promotion = false;
}

void revert(){
  //Serial.println(other_hand.row);
  //Serial.println(other_hand.col);
  //Serial.println(send_piece.row);
  //Serial.println(send_piece.col);
  char piece_revert = getPiece(send_piece.row, send_piece.col);
  //Serial.println(piece_revert);
  while(!readSquare(other_hand.row+1, other_hand.col+1) || readSquare(send_piece.row+1,send_piece.col+1)){
    turnOnLED(other_hand.row+1, other_hand.col+1);
    delay(1);
    turnOnLED(send_piece.row+1,send_piece.col+1);
    delay(1);
  }
  setPiece(send_piece.row,send_piece.col,other_hand.row,other_hand.col, piece_revert);
  turnOffAll();
}

void win(){
  //TODO: set of led stuff to indicate win
  for (int i = 0; i < numRows; i++) {
    digitalWrite(rowPins[i], HIGH);
    delay(1);
    for (int j = 0; j < numCols; j++) {
      digitalWrite(colPins[j], LOW);
      delay(1);
      digitalWrite(colPins[j], HIGH);
      delay(1);
    }
  }


  
  turnOffAll();
}

bool readSquare(int row, int col){
  bool occupied;
  switch(row){
    case 1:{
      digitalWrite(sel0, HIGH);
      digitalWrite(sel1, HIGH);
      digitalWrite(sel2, HIGH);
      break;
    }
    case 2:{
      digitalWrite(sel0, HIGH);
      digitalWrite(sel1, HIGH);
      digitalWrite(sel2, LOW);
      break;
    }
    case 3:{
      digitalWrite(sel0, HIGH);
      digitalWrite(sel1, LOW);
      digitalWrite(sel2, HIGH);
      break;
    }
    case 4:{
      digitalWrite(sel0, HIGH);
      digitalWrite(sel1, LOW);
      digitalWrite(sel2, LOW);
      break;
    }
    case 5:{
      digitalWrite(sel0, LOW);
      digitalWrite(sel1, HIGH);
      digitalWrite(sel2, HIGH);
      break;
    }
    case 6:{
      digitalWrite(sel0, LOW);
      digitalWrite(sel1, HIGH);
      digitalWrite(sel2, LOW);
      break;
    }
    case 7:{
      digitalWrite(sel0, LOW);
      digitalWrite(sel1, LOW);
      digitalWrite(sel2, HIGH);
      break;
    }
    case 8:{
      digitalWrite(sel0, LOW);
      digitalWrite(sel1, LOW);
      digitalWrite(sel2, LOW);
      break;
    }
  }
  occupied = !digitalRead(colSel[col-1]);

  return occupied;
}

void readBoardState(){ // reads switches and sets them to 1 if occupied in current_occupied_squares
  bool occupied;

  digitalWrite(sel0, LOW);
  digitalWrite(sel1, LOW); //row 8
  digitalWrite(sel2, LOW);
  for(int k = 0; k < 8; k++){
    occupied = !digitalRead(colSel[k]); // k = 0 => col 1, k = 1 => col 2
    current_occupied_squares[7][k] = occupied;
  }

  digitalWrite(sel0, LOW);
  digitalWrite(sel1, LOW); // row 7
  digitalWrite(sel2, HIGH);
  for(int k = 0; k < 8; k++){
    occupied = !digitalRead(colSel[k]); // k = 0 => col 1, k = 1 => col 2
    current_occupied_squares[6][k] = occupied;
  }

  digitalWrite(sel0, LOW);
  digitalWrite(sel1, HIGH); // row 6
  digitalWrite(sel2, LOW);
  for(int k = 0; k < 8; k++){
    occupied = !digitalRead(colSel[k]); // k = 0 => col 1, k = 1 => col 2
    current_occupied_squares[5][k] = occupied;
  }

  digitalWrite(sel0, LOW);
  digitalWrite(sel1, HIGH); // row 5
  digitalWrite(sel2, HIGH);
  for(int k = 0; k < 8; k++){
    occupied = !digitalRead(colSel[k]); // k = 0 => col 1, k = 1 => col 2
    current_occupied_squares[4][k] = occupied;
  }

  digitalWrite(sel0, HIGH);
  digitalWrite(sel1, LOW); // row 4
  digitalWrite(sel2, LOW);
  for(int k = 0; k < 8; k++){
    occupied = !digitalRead(colSel[k]); // k = 0 => col 1, k = 1 => col 2
    current_occupied_squares[3][k] = occupied;
  }

  digitalWrite(sel0, HIGH);
  digitalWrite(sel1, LOW); // row 3
  digitalWrite(sel2, HIGH);
  for(int k = 0; k < 8; k++){
    occupied = !digitalRead(colSel[k]); // k = 0 => col 1, k = 1 => col 2
    current_occupied_squares[2][k] = occupied;
  }

  digitalWrite(sel0, HIGH);
  digitalWrite(sel1, HIGH); // row 2
  digitalWrite(sel2, LOW);
  for(int k = 0; k < 8; k++){
    occupied = !digitalRead(colSel[k]); // k = 0 => col 1, k = 1 => col 2
    current_occupied_squares[1][k] = occupied;
  }

  digitalWrite(sel0, HIGH);
  digitalWrite(sel1, HIGH); // row 1
  digitalWrite(sel2, HIGH);
  for(int k = 0; k < 8; k++){
    occupied = !digitalRead(colSel[k]); // k = 0 => col 1, k = 1 => col 2
    current_occupied_squares[0][k] = occupied;
  }
}

void compareBoards(){ // compares the previous and current board state. If it detects change, either turns on or turns off LEDs
  int count = 0;
  char piece;
  for(int i = 0; i < numRows; i++){
    for(int j = 0; j < numCols; j++){
      if(prev_occupied_squares[i][j] != current_occupied_squares[i][j]){
        if(current_occupied_squares[i][j] == 0){ // picking up a piece to move
          //Serial.println("In 0 State");
          if(piece_in_hand.pick_up == false){
            //Serial.println("In False PickUp 0");
            turnOnPieceLED(i+1,j+1);
            piece_in_hand.row = i;
            piece_in_hand.col = j;
            piece_in_hand.pick_up = true;
            piece_in_hand.type = getPiece(i,j);
            //Serial.println(String(piece_in_hand.type));
            //move_made = false; // for FSM
          }
          else{ // possiblity taking a piece
            turnOnPieceLED(piece_in_hand.row+1,piece_in_hand.col+1);
            //Serial.println("In True PickUp 0");
          }
          
        }
        else if(current_occupied_squares[i][j] == 1){
          //Serial.println("In 1 State");
          if(piece_in_hand.pick_up){
            bool h = checkPiece(i+1, j+1, piece_in_hand.type);
            //Serial.println("In True PickUp 1");
            //Serial.println("Check:");
            //Serial.println(i+1);
            //Serial.println(j+1);
            //Serial.println(piece_in_hand.row+1);
            //Serial.println(piece_in_hand.col+1);
            if(h){
              //Serial.println("Changing Piece in Hand");
              setPiece(piece_in_hand.row, piece_in_hand.col,i,j,piece_in_hand.type);
              if(cast2yesK){
                //Serial.println("cast2yesK");
                while(!readSquare(8,6)){}
                setPiece(7,7,7,5,'r');
                cast2yesK = false;
                cast2yesQ = false;
                //printBoardState(); 
              }
              else if(cast2yesQ){
                //Serial.println("cast2yesQ");
                while(!readSquare(8,4)){}
                setPiece(7,0,7,3,'r');
                cast2yesK = false;
                cast2yesQ = false;
                //printBoardState();                
              }
              else if(cast1yesK){
                //Serial.println("cast1yesK");
                while(!readSquare(1,6)){}
                setPiece(0,7,0,5,'R');
                cast1yesK = false;
                cast1yesQ = false;
                //printBoardState();                
              }
              else if(cast1yesQ){
                //Serial.println("cast1yesQ");
                while(!readSquare(1,4)){}
                setPiece(0,0,0,3,'R');
                cast1yesK = false;
                cast1yesQ = false;
                //printBoardState(); 
              }
              if(i == piece_in_hand.row && j == piece_in_hand.col){
               // move_made = false;
              }
              else{
                other_hand.row = piece_in_hand.row;
                other_hand.col = piece_in_hand.col;
                other_hand.type = piece_in_hand.type;
                send_piece.row = i;
                send_piece.col = j;
                send_piece.type = piece_in_hand.type;
                move_made = true;
              }
              //Serial.println("Send Piece Row = "  + String(send_piece.row+1));
              //Serial.println("Send Piece Col = "  + String(send_piece.col+1));
              //Serial.println("Other Hand Row = " + String(other_hand.row+1));
              //Serial.println("Other Hand Col = " + String(other_hand.col+1));
              piece_in_hand.pick_up = false;
              prev_occupied_squares[piece_in_hand.row][piece_in_hand.col] = 0;
              prev_occupied_squares[i][j] = 1;
              turnOffAll();

               // for the FSM
            }
          }
          else{
            //Serial.println("In False PickUp 1");
          }
        }
        prev_occupied_squares[i][j] = current_occupied_squares[i][j];
      }
      else{
        if(piece_in_hand.pick_up){
          turnOnPieceLED(piece_in_hand.row+1,piece_in_hand.col+1);
        }
      }
      
    }
  }
}

void printBoardState(){
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      if(chessboard[i][j] == '\0'){
        Serial.print("0");
      }
      else{
        Serial.print(String(chessboard[i][j]));
      }

    }
    Serial.println();
  }
}

void setupPlayer() {

  // Initialize row pins as outputs
  for (int i = 0; i < numRows; i++) {
    pinMode(rowPins[i], OUTPUT);
  }
  
  // Initialize column pins as outputs
  for (int j = 0; j < numCols; j++) {
    pinMode(colPins[j], OUTPUT);
  }

  pinMode(sel0, OUTPUT);
  pinMode(sel1, OUTPUT);
  pinMode(sel2, OUTPUT);

  for(int k = 0; k < numcolSel; k++){
    pinMode(colSel[k], INPUT_PULLUP);
  }
}

void test(){
 // digitalWrite(26, HIGH);
 // digitalWrite(27, LOW); // row 1
 // digitalWrite(28, LOW);
 // bool occupied = !digitalRead(32);
 // if(occupied){
  for(int i = 1; i <= 8; i++){
    turnOnLED(i,1);
    delay(1);
  }
  
  //}
}

