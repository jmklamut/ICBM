#include "CPU_Move.h"
#include "string.h"
#include "HardwareSerial.h"

String recieved_move;

const String rowLetter[8] = {"A","B","C","D","E","F","G","H"};

bool checkmate = false;
bool check = false;
bool valid = false;
bool promote_bool = false;
char promote_char = '\0';
String cpu_move;
String prev_move = "M";
String pawn = "X";
String timeout = "X";
String move = "";
bool cast2yesKCPU = false;
bool cast2yesQCPU = false;
bool cast1yesKCPU = false;
bool cast1yesQCPU = false;
bool send = false;
bool move_valid = true;
bool black = false;

bool setBlack(bool v){
  black = v;
}

bool getCheckMate(){
  return checkmate;
}

bool getCheck(){
  return check;
}

bool getValid(){
  return valid;
}

bool getMoveValid(){
  return move_valid;
}

String getCPUMove(){
  return cpu_move;
}

bool getCPUPromote(){
  return promote_bool;
}

char getPromote(){
  return promote_char;
}

void setCPUPromote(bool c){
  promote_bool = c;
}

void setPromote(char c){
  promote_char = c;
}

void setPawn(String x){
  pawn = x;
}

void setTimeout(String x){
  timeout = x;
}

bool getCast2King(){
  return cast2yesKCPU;
}

bool getCast2Queen(){
  return cast2yesQCPU;
}

bool getCast1King(){
  return cast1yesKCPU;
}

bool getCast1Queen(){
  return cast1yesQCPU;
}

bool getSend(){
  return send;
}

void setSend(bool f){
  send = f;
}

bool recieveCPU(){
 //TODO: function that will take in the serial input from Chess Engine
  if (Serial.available() > 0) {
    send = true;
    char i = Serial.read();
    //Serial.println(String(i));
    if( i == '1' || i == '0'){
      String incomingData = Serial.readStringUntil('\n');
     // Serial.println(incomingData);
   // String incomingData = Serial.readStringUntil('\n');
      //Serial.println(incomingData);
      incomingData = String(i) + incomingData;
      String v = incomingData.substring(0,1);
      String move = incomingData.substring(1,5);
      String space1 = incomingData.substring(5,6);
      String k = incomingData.substring(6,7);
      String space2 = incomingData.substring(7,8);
      String mate = incomingData.substring(8,9); // if game is over
      String space3 = incomingData.substring(9,10);
      String promote = incomingData.substring(10,11);
      String nines = incomingData.substring(11);

      //Serial.println(promote);

      if(incomingData == prev_move){
       // Serial.println("Same Move");
        return false;
      }
      cast2yesKCPU = false;
      cast2yesQCPU = false;
      cast1yesKCPU = false;
      cast1yesQCPU = false;

      bool truth = false;

      if(space1 == "X" && space2 == "X" && space3 == "X" && nines.substring(0,1) == "9"){ // error checking

        if(v == "0"){
          //Serial.println("I am Here");
          valid = false;
          prev_move = incomingData;
          //Serial.println(incomingData);
          return true;
        }
        else{
          prev_move = incomingData;
          valid = true;
        }

        if(k == "1"){
          prev_move = incomingData;
          //Serial.println("Check");
          check = true;
        }
        else{
          check = false;
        }
        if(mate == "1"){
          checkmate = true;
          prev_move = incomingData;
          truth = true;
        }
        else{
          checkmate = false;
        }
        if(promote == "Q"){
          promote_bool = true;
          promote_char = 'Q';

        }
        else if(promote == "K"){
          promote_bool = true;
          promote_char = 'K';
        }
        else{
          promote_bool = false;
          promote_char = '\0';
        }
        if(move == "E1G1"){
          cast2yesKCPU = true;
        }
        else if(move == "E1C1"){
          cast2yesQCPU = true;
        }
        else if(move == "E8G8"){
           cast1yesKCPU = true;
        }
        else if(move == "E8C8"){
          cast1yesQCPU = true;
        }

        move_valid = true;
        for(int i = 0; i < 4; i++){
          if(move.substring(i,i++) == "X"){
            //invalid move dont know what to do in this case yet
            move_valid = false;
            prev_move = incomingData;
            truth = true;
          }
        }

        if(move_valid){
          cpu_move = chessToCor(move);
          prev_move = incomingData;
         // Serial.println(incomingData);
          truth = true;
        }
        
        if(truth){
          prev_move = incomingData;
          //Serial.println(incomingData);
          return true;
        }
        return false;
      }
      return false;
    }
  }
  return false;
  
}

String chessToCor(String convert){
  //TODO: Create a function that converts corridanates
  String sub1 = convert.substring(0,1);
  String sub2 = convert.substring(2,3);
  String sub3 = convert.substring(1,2);
  String sub4 = convert.substring(3);
  int num1;
  int num2;
  for(int i = 0; i < 8; i++){
    if(sub1 == rowLetter[i]){
      num1 = i+1;
    }
    if(sub2 == rowLetter[i]){
      num2 = i+1;
    }
  }
  if(black){
    //Serial.println("In Black");
    num2 = 9 - num2;
    num1 = 9 - num1;
    int num3 = sub3.toInt();
    int num4 = sub4.toInt();
    num3 = 9 - num3;
    num4 = 9 - num4;
    sub3 = String(num3);
    sub4 = String(num4);
    //Serial.println(sub3 + String(num1) + sub4 + String(num2));
  }
  return sub3 + String(num1) + sub4 + String(num2);
}

String corToChess(String convert){
  String sub1 = convert.substring(0,1);
  String sub2 = convert.substring(2,3);
  String sub3 = convert.substring(1,2);
  String sub4 = convert.substring(3);
  int num1 = sub1.toInt();
  int num2 = sub2.toInt();
  if(black){
    //Serial.println("In Black");
    num2 = 9 - num2;
    num1 = 9 - num1;
    int num3 = sub3.toInt();
    int num4 = sub4.toInt();
    num3 = 9 - num3;
    num4 = 9 - num4;
    sub3 = String(num3);
    sub4 = String(num4);
    //Serial.println(rowLetter[num1-1] + sub3 + rowLetter[num2-1] + sub4);
  }
  return rowLetter[num1-1] + sub3 + rowLetter[num2-1] + sub4;
}

void transmitCPU(String send){
//TODO: transmit a move to the Chess Engine in cor
  String transmit = corToChess(send);
  Serial.print(transmit + "X" + pawn + timeout + "999");
  
}