
#include "ClockCommunication.h"
#include <Wire.h>

//variable read from master
int x = 5;
String ret;

void receiveEvent(int bytes) {
  x = Wire.read();    // read one character from the I2C
}

String decoder(){
  
  if(x == 5){
      ret = "none";
  }
  //menu decoder
  else if(x==10)
  {
    //Serial.println("PvCPU Selected");
    ret = "PvCPU";
    x = 5;
  }
  else if(x==11)
  {
    //Serial.println("PvP Selected");
    ret = "PvP";
    x = 5;
  }
  else if(x==20)
  {
    //Serial.println("Easy Selected");
    ret = "Easy";
    x = 5;
  }
  else if(x==21)
  {
    //Serial.println("Medium Selected");
    ret = "Medium";
    x = 5;
  }
  else if(x==22)
  {
    //Serial.println("Hard Selected");
    ret = "Hard";
    x = 5;
  }
  else if(x==30)
  {
    //Serial.println("WHITE Selected");
    ret = "WHITE";
    x = 5;
  }
  else if(x==31)
  {
    //Serial.println("BLACK Selected");
    ret = "BLACK";
    x = 5;
  }
  else if(x==50)
  {
    //Serial.println("Game time");
    ret = "Game";
    x = 5;
  }
  //Pawn Promotion Decoder
  else if(x==0)
  {
    //Serial.println("Knight Selected");
    ret = "Knight";
    x = 5;
  }
  else if(x==1)
  {
    //Serial.println("Rook Selected");
    ret = "Rook";
    x = 5;
  }
  else if(x==2)
  {
    //Serial.println("Bishop Selected");
    ret = "Bishop";
    x = 5;
  }
  else if(x==3)
  {
    //Serial.println("Queen Selected");
    ret = "Queen";
    x = 5;
  }
  else if(x == 100){
    //Serial.println("Turn Ended");
    ret = "Turn";
  }
  else if(x == 101){
    //Serial.println("Game Ended");
    ret = "End";
  }
  else if(x == 69){
    //Serial.println("Surrender");
    ret = "Surrender";
  }
  else{
    ret = "Random";
  }

  return ret;
  
}

void set_ret(String x){
  ret = x;
}

void set_Zero(){
  x = 5;
}


void setPinOne(uint8_t i){
  digitalWrite(37, i);
}

void setPinTwo(uint8_t i){
  digitalWrite(38, i);
}

void setPinThree(uint8_t i){
  digitalWrite(39, i);
}

void nothing(){
  setPinOne(0);
  setPinTwo(0);
  setPinThree(0);
}

void check(){
  setPinOne(1);
  setPinTwo(0);
  setPinThree(0);
}


void pawnPromotion(){
  setPinOne(0);
  setPinTwo(1);
  setPinThree(0);
}

void sendKnight(){
  setPinOne(1);
  setPinTwo(1);
  setPinThree(0);
}

void error(){
  setPinOne(0);
  setPinTwo(0);
  setPinThree(1);  
}

void draw(){
  setPinOne(1);
  setPinTwo(0);
  setPinThree(1); 
}

void sendQueen(){
  setPinOne(0);
  setPinTwo(1);
  setPinThree(1); 
}

void winningScreen(){
  setPinOne(1);
  setPinTwo(1);
  setPinThree(1);  
}



void clockSetup() {
  // Start the I2C Bus as Slave on address 9
  Wire.begin(9); 
  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);

  pinMode(37, OUTPUT);
  pinMode(38, OUTPUT);
  pinMode(39, OUTPUT);
}
