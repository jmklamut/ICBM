#include "Player_Turn.h"
#include "ClockCommunication.h"
#include "CPU_Move.h"
#include <TinyStepper_28BYJ_48.h>

enum State {GAMESET, PLAYERMOVE, PROMOTION, TURN, CPUMOVE, CPUMOVEINT, REVERT, WIN, TEST, RESTART};

State state = GAMESET;


bool chessbot = true;
bool playerturn;
bool button;
bool current_color; 
String clock_input;
bool p = false;
String inital = "";

const int MOTOR_IN1_PIN = 52;
const int MOTOR_IN2_PIN = 48;
const int MOTOR_IN3_PIN = 46;
const int MOTOR_IN4_PIN = 44;

const int STEPS_PER_REVOLUTION = 2048;

TinyStepper_28BYJ_48 stepper;

void setup() {
  setupPlayer();
  clockSetup();
  stepper.connectToPins(MOTOR_IN1_PIN, MOTOR_IN2_PIN, MOTOR_IN3_PIN, MOTOR_IN4_PIN);
}

void loop() {
  switch(state){
    case GAMESET: {
      setTimeout("X");
      setPawn("X");
      nothing();
      clock_input = decoder();
      //String x = "PvCPU";
      if(clock_input=="PvCPU")
      {
        //send to chess computer
        chessbot = true;
        inital = inital + "E";

      }
      else if(clock_input=="PvP")
      {
        //send to chess computer
        chessbot = false;
        inital = inital + "PE";
      }
      else if(clock_input=="Easy")
      {
        //send to the chess computer
        inital = inital + "E";
        delay(1000);
      }
      else if(clock_input=="Medium")
      {
        //send to chess computer
        inital = inital + "M";
        delay(1000);
      }
      else if(clock_input=="Hard")
      {
        //send to chess computer
        inital = inital + "H";
        delay(1000);
      }
      else if(clock_input=="WHITE")
      {
        //send to chess computer
        playerturn = true;
        inital = inital + "W";

      }
      else if(clock_input=="BLACK")
      {
        //send to chess computer
        inital = inital + "B";
        playerturn = false; // send a one or a zero
        setBlack(true);
        setBlackPlayer();
      }
      else if(clock_input=="Game")
      {
      //  for(int i = 0; i < 20; i++){
          Serial.print(inital + "999");
          delay(300);
      //  }
        if(playerturn){
          inital = "";
          state = PLAYERMOVE;
        }
        else if(chessbot){
          inital = "";
          state = CPUMOVEINT;
        }
        else{
          inital = "";
          state = PLAYERMOVE;
        }
      }
      break;
    }
    case TURN: {
      clock_input = decoder();
      //Serial.println(clock_input);
      if(clock_input == "Turn"){
        //Serial.println("I am in here");
        set_moveMade(false);
        nothing();
        set_Zero();
        setSend(false);
        if(chessbot){
          state = CPUMOVE;
        }
        else{
          state = PLAYERMOVE;
        }
        
      }
      else{
        state = PLAYERMOVE;
      }
      //state = CPUMOVE;

      break;
    }
    case PROMOTION: { //player pawn promotion
      //Pawn Promotion Decoder
      clock_input = decoder();
      pawnPromotion();
      if(clock_input == "Turn" || p){
        p = true;
        if(clock_input=="Knight")
        {
          setPawn("K");
          if(get_promote_type()  == 'p'){
            setPieceSpecific(get_promote_row()-1, get_promote_col()-1, 'k');
          }
          else if(get_promote_type()  == 'P'){
            setPieceSpecific(get_promote_row()-1, get_promote_col()-1, 'K');
          }
          //Serial.println("Knight");
          set_promotion(false);
          set_moveMade(false);
          setSend(false);
          nothing();
          p = false;
          state = CPUMOVE;
        }
        if(clock_input=="Queen")
        {
          setPawn("Q");
          if(get_promote_type()  == 'p'){
            setPieceSpecific(get_promote_row()-1, get_promote_col()-1, 'q');
          }
          else if(get_promote_type()  == 'P'){
            setPieceSpecific(get_promote_row()-1, get_promote_col()-1, 'Q');         
          }
          //send to chess computer
          //Serial.println("Queen");
          //Serial.println(get_promote_row());
          //Serial.println(get_promote_col());
          set_promotion(false);
          set_moveMade(false);
          setSend(false);
          nothing();
          p = false;
          state = CPUMOVE;
        }
      }
      break;
    }
    case PLAYERMOVE:{
      //Serial.println("PLAYERMOVE");
      readBoardState(); //updates board
      compareBoards(); //sets LEDs and board 
      if(moveMade()){
        //Serial.println("moveMade");
        if(get_promotion()){
          state = PROMOTION;
          break;
        }
        state = TURN;
      }
      else{
        clock_input = decoder();
       // Serial.println(clock_input);
        if(clock_input == "Turn"){
          //Serial.println("No No Nooo");
          error();
        }
        else if(clock_input == "Surrender"){
          state = WIN;
        }
        else if(clock_input == "End"){
          state = WIN;
        }
        else{
          nothing();
        }
      }
      break;
    }
    case CPUMOVE:{ // need to make state for pawn promotion
      clock_input = decoder();
      delay(50);
      String send = sendAPiece();
      if(!getSend()){
      //  for(int i = 0; i < 20; i++){
          transmitCPU(send);
          delay(300);
      //  }
      }
      if(clock_input == "End" || clock_input == "Surrender"){
        state = WIN;
      }
      setSend(true);
      if(recieveCPU()){
        //wait for vaild input
        setTimeout("X");
        setPawn("X");
        String move  = getCPUMove();
        //Serial.println(move);
        if(!getValid()){
          //go to error state, send to clock
          //Serial.println("InValid");
          if(chessbot){
            stepper.setSpeedInStepsPerSecond(256);
            stepper.setAccelerationInStepsPerSecondPerSecond(512);
            stepper.moveRelativeInSteps(-1090);
            delay(1000);
            stepper.moveRelativeInSteps(1090);
            delay(1000);
          }  
          state = REVERT;
          break;
        }
        else if(getCheckMate()){
          // send to clock/ go to win state
          //Serial.println("CheckMate");
          if(chessbot && !getMoveValid()){
            stepper.setSpeedInStepsPerSecond(256);
            stepper.setAccelerationInStepsPerSecondPerSecond(512);
            stepper.moveRelativeInSteps(-1090);
            delay(1000);
            stepper.moveRelativeInSteps(1090);
            delay(1000);
            state = RESTART;
            break;
          }         
        }
        else if(getCheck()){
          // send to clock
          //Serial.println("Check");
          check();
        }
        else if(!chessbot){
          setSend(false);
          state = PLAYERMOVE;
          break;
        }
        if(getCast2King()){
          while(!readSquare(8,6)){
            turnOnLED(8,5);
            delay(1);
            turnOnLED(8,7);
            delay(1);
          }
          turnOffAll();
          setPiece(7,4,7,6,'c');
          setPiece(7,7,7,5,'r');
          if(chessbot){
            stepper.setSpeedInStepsPerSecond(256);
            stepper.setAccelerationInStepsPerSecondPerSecond(512);
            stepper.moveRelativeInSteps(-1024);
            delay(1000);
            stepper.moveRelativeInSteps(1024);
            delay(1000);
          }    
          set_Zero();
          set_moveMade(false);
          setSend(false);
          state = PLAYERMOVE;
        }
        else if(getCast2Queen()){
          while(!readSquare(8,4)){
            turnOnLED(8,5);
            delay(1);
            turnOnLED(8,3);
            delay(1);
          }
          turnOffAll();
          setPiece(7,4,7,2,'c');
          setPiece(7,0,7,3,'r');
          if(chessbot){
            stepper.setSpeedInStepsPerSecond(256);
            stepper.setAccelerationInStepsPerSecondPerSecond(512);
            stepper.moveRelativeInSteps(-1024);
            delay(1000);
            stepper.moveRelativeInSteps(1024);
            delay(1000);
          } 
          set_Zero();
          set_moveMade(false);
          setSend(false);
          state = PLAYERMOVE;
        }
        else if(getCast1King()){
          while(!readSquare(1,6)){
            turnOnLED(1,5);
            delay(1);
            turnOnLED(1,7);
            delay(1);
          }
          turnOffAll();
          setPiece(0,4,0,6,'C');
          setPiece(0,7,0,5,'R');
          if(chessbot){
            stepper.setSpeedInStepsPerSecond(256);
            stepper.setAccelerationInStepsPerSecondPerSecond(512);
            stepper.moveRelativeInSteps(-1024);
            delay(1000);
            stepper.moveRelativeInSteps(1024);
            delay(1000);
          } 
          set_Zero();
          set_moveMade(false);
          setSend(false);
          state = PLAYERMOVE;
        }
        else if(getCast1Queen()){
          while(!readSquare(1,4)){
            turnOnLED(1,5);
            delay(1);
            turnOnLED(1,3);
            delay(1);
          }
          turnOffAll();
          setPiece(0,4,0,2,'C');
          setPiece(0,0,0,3,'R');
          if(chessbot){
            stepper.setSpeedInStepsPerSecond(256);
            stepper.setAccelerationInStepsPerSecondPerSecond(512);
            stepper.moveRelativeInSteps(-1024);
            delay(1000);
            stepper.moveRelativeInSteps(1024);
            delay(1000);
          }
          set_Zero();
          set_moveMade(false);
          setSend(false);
          state = PLAYERMOVE; 
        }
        else{
        //send lights to the chess board
         // Serial.println(move);
          int prev_row = move.substring(0,1).toInt();
          int prev_col = move.substring(1,2).toInt();
          int cur_row = move.substring(2,3).toInt();
          int cur_col = move.substring(3).toInt();
          char piece = getPiece(9-prev_row-1, prev_col-1);
          //Serial.println(String(piece));
          //checks if square going is occupied
          bool cur_state = readSquare(9-cur_row,cur_col);
         // Serial.println(cur_state);            
          while(readSquare(9-prev_row, prev_col) || !(cur_state != readSquare(9-cur_row,cur_col))){
            turnOnLED(9-prev_row,prev_col);
            delay(1);
            turnOnLED(9-cur_row,cur_col);
            delay(1);
          }
          turnOffAll();
          setPiece(9-prev_row-1, prev_col-1, 9-cur_row-1, cur_col-1, piece);
          if(getCPUPromote()){
            if(getPromote() == 'Q'){
              //Serial.println("In Q");
              sendQueen();
              //Serial.println(cur_row);
              if(cur_row == 8){
                setPieceSpecific(9-cur_row-1,cur_col-1,'Q');
              }
              else if(cur_row == 1){
                setPieceSpecific(9-cur_row-1,cur_col-1,'q');
              }
            }
            else if(getPromote() == 'K'){
              //Serial.println("In K");
              sendKnight();
              if(cur_row == 8){
                setPieceSpecific(9-cur_row-1,cur_col-1,'K');
              }
              else if(cur_row == 1){
                setPieceSpecific(9-cur_row-1,cur_col-1,'k');               
              }
            }
          }
          if(chessbot){
            stepper.setSpeedInStepsPerSecond(256);
            stepper.setAccelerationInStepsPerSecondPerSecond(512);
            stepper.moveRelativeInSteps(-1070);
            delay(1000);
            stepper.moveRelativeInSteps(1070);
            delay(1000);
          }    
          if(getCheckMate()){
            //Serial.println("Check mate black");
            state = RESTART;
            break;
          }
          set_moveMade(false);
          set_Zero();
          setSend(false);
          state = PLAYERMOVE;
        }
      }
      break;
    }
    case CPUMOVEINT:{
      //TODO: Inital State to CPU when its White: What to try next, see if sending a regular string will work=> Central COntroller to Engine works but Engine to controller is inconsistent 
      delay(50);
      if(recieveCPU()){
        //wait for vaild input
        setTimeout("X");
        setPawn("X");
        String move  = getCPUMove();
        //Serial.println(move);
        if(!getValid()){
          //go to error state, send to clock
          //Serial.println("InValid");
          if(chessbot){
            stepper.setSpeedInStepsPerSecond(256);
            stepper.setAccelerationInStepsPerSecondPerSecond(512);
            stepper.moveRelativeInSteps(-1090);
            delay(1000);
            stepper.moveRelativeInSteps(1090);
            delay(1000);
          }  
          state = REVERT;
          break;
        }
        else if(getCheckMate()){
          // send to clock/ go to win state
          //Serial.println("CheckMate");
          if(chessbot && !getMoveValid()){
            stepper.setSpeedInStepsPerSecond(256);
            stepper.setAccelerationInStepsPerSecondPerSecond(512);
            stepper.moveRelativeInSteps(-1070);
            delay(1000);
            stepper.moveRelativeInSteps(1070);
            delay(1000);
            state = RESTART;
            break;
          }         
        }
        else if(getCheck()){
          // send to clock
          //Serial.println("Check");
          check();
        }
        else if(!chessbot){
          setSend(false);
          state = PLAYERMOVE;
        }
        if(getCast2King()){
          while(!readSquare(8,6)){
            turnOnLED(8,5);
            delay(1);
            turnOnLED(8,7);
            delay(1);
          }
          turnOffAll();
          setPiece(7,4,7,6,'c');
          setPiece(7,7,7,5,'r');
          if(chessbot){
            stepper.setSpeedInStepsPerSecond(256);
            stepper.setAccelerationInStepsPerSecondPerSecond(512);
            stepper.moveRelativeInSteps(-1024);
            delay(1000);
            stepper.moveRelativeInSteps(1024);
            delay(1000);
          }    
          set_Zero();
          set_moveMade(false);
          setSend(false);
          state = PLAYERMOVE;
        }
        else if(getCast2Queen()){
          while(!readSquare(8,4)){
            turnOnLED(8,5);
            delay(1);
            turnOnLED(8,3);
            delay(1);
          }
          turnOffAll();
          setPiece(7,4,7,2,'c');
          setPiece(7,0,7,3,'r');
          if(chessbot){
            stepper.setSpeedInStepsPerSecond(256);
            stepper.setAccelerationInStepsPerSecondPerSecond(512);
            stepper.moveRelativeInSteps(-1024);
            delay(1000);
            stepper.moveRelativeInSteps(1024);
            delay(1000);
          } 
          set_Zero();
          set_moveMade(false);
          setSend(false);
          state = PLAYERMOVE;
        }
        else if(getCast1King()){
          while(!readSquare(1,6)){
            turnOnLED(1,5);
            delay(1);
            turnOnLED(1,7);
            delay(1);
          }
          turnOffAll();
          setPiece(0,4,0,6,'C');
          setPiece(0,7,0,5,'R');
          if(chessbot){
            stepper.setSpeedInStepsPerSecond(256);
            stepper.setAccelerationInStepsPerSecondPerSecond(512);
            stepper.moveRelativeInSteps(-1024);
            delay(1000);
            stepper.moveRelativeInSteps(1024);
            delay(1000);
          } 
          set_Zero();
          set_moveMade(false);
          setSend(false);
          state = PLAYERMOVE;
        }
        else if(getCast1Queen()){
          while(!readSquare(1,4)){
            turnOnLED(1,5);
            delay(1);
            turnOnLED(1,3);
            delay(1);
          }
          turnOffAll();
          setPiece(0,4,0,2,'C');
          setPiece(0,0,0,3,'R');
          if(chessbot){
            stepper.setSpeedInStepsPerSecond(256);
            stepper.setAccelerationInStepsPerSecondPerSecond(512);
            stepper.moveRelativeInSteps(-1024);
            delay(1000);
            stepper.moveRelativeInSteps(1024);
            delay(1000);
          }
          set_Zero();
          set_moveMade(false);
          setSend(false);
          state = PLAYERMOVE; 
        }
        else{
        //send lights to the chess board
         // Serial.println(move);
          int prev_row = move.substring(0,1).toInt();
          int prev_col = move.substring(1,2).toInt();
          int cur_row = move.substring(2,3).toInt();
          int cur_col = move.substring(3).toInt();
          char piece = getPiece(9-prev_row-1, prev_col-1);
          //Serial.println(String(piece));
          //checks if square going is occupied
          bool cur_state = readSquare(9-cur_row,cur_col);
         // Serial.println(cur_state);            
          while(readSquare(9-prev_row, prev_col) || !(cur_state != readSquare(9-cur_row,cur_col))){
            turnOnLED(9-prev_row,prev_col);
            delay(1);
            turnOnLED(9-cur_row,cur_col);
            delay(1);
          }
          turnOffAll();
          setPiece(9-prev_row-1, prev_col-1, 9-cur_row-1, cur_col-1, piece);
          if(getCPUPromote()){
            if(getPromote() == 'Q'){
              //Serial.println("In Q");
              sendQueen();
              //Serial.println(cur_row);
              if(cur_row == 8){
                setPieceSpecific(9-cur_row-1,cur_col-1,'Q');
              }
              else if(cur_row == 1){
                setPieceSpecific(9-cur_row-1,cur_col-1,'q');
              }
            }
            else if(getPromote() == 'K'){
              //Serial.println("In K");
              sendKnight();
              if(cur_row == 8){
                setPieceSpecific(9-cur_row-1,cur_col-1,'K');
              }
              else if(cur_row == 1){
                setPieceSpecific(9-cur_row-1,cur_col-1,'k');               
              }
            }
          } 
          if(chessbot){
            stepper.setSpeedInStepsPerSecond(256);
            stepper.setAccelerationInStepsPerSecondPerSecond(512);
            stepper.moveRelativeInSteps(-1070);
            delay(1000);
            stepper.moveRelativeInSteps(1070);
            delay(1000);
          }    
          if(getCheckMate()){
            //Serial.println("Check mate black");
            state = RESTART;
            break;
          }
          set_moveMade(false);
          set_Zero();
          setSend(false);
          state = PLAYERMOVE;
        }
      }
      break;
    }
    case REVERT:{
      revert();
      set_Zero();
      set_moveMade(false);
      setSend(false);
      state = PLAYERMOVE;
      break;
    }
    case RESTART:{
      reset();
      winningScreen();
      win();
      clock_input = decoder();
      if(clock_input == "End"){
        Serial.print("XXXXXXT999");
        reset();
        setBlack(false);
        turnOffAll();
        state = GAMESET;
      }
      break;
    }
    case WIN:{
      reset();
      setSend(false);
      clock_input = decoder();
      setTimeout("T");
      Serial.print("XXXXXXT999");
      delay(300);
      turnOffAll();
      setBlack(false);
      state = GAMESET;
      break;
    }
    case TEST:{
    //  reset();
    //  printBoardState();
      state = PLAYERMOVE;

   //   if(recieveCPU()){
    //    state = CPUMOVEINT;
    //  }
      
      //transmitCPU("1234");
      /*
      Serial.println("X");
      delay(1000);
      if(recieveCPU()){
        while(1){
          Serial.println(getCPUMove());
        }
      }
      */
      
      //readBoardState(); //updates board
      //compareBoards(); //sets LEDs and board
      
      /*
      while(!readSquare(6,8) && readSquare(8,8)){
        turnOnLED(8,8);
        delay(1);
        turnOnLED(6,8);
        delay(1);
      }
      turnOffAll();
      while(1){
        Serial.println("End");
      }
      */
      
      /*
      Serial.println("In Test");
      while(1){
        turnOnLED(9-1,8);
        delay(1);
        turnOnLED(9-3,8);
        delay(1);
      }
      */
      /*
      while(recieveCPU()){
        Serial.println(getValid());
        Serial.println(getCPUMove());
        Serial.println(getCheck());
        Serial.println(getCheckMate());
        delay(1000);
      }
      */
      //transmitCPU("1223");
      //winningScreen();
      //readBoardState(); //updates board
      //compareBoards(); //sets LEDs and board
      /*
      bool occupied;
      int i = 8;
      int y = 8;

      occupied = readSquare(8,i);
      if(occupied){
        turnOnLED(8,y);
        Serial.println("8");
        delay(1);
      }


      occupied = readSquare(7,i);
      if(occupied){
        turnOnLED(7,y);
        Serial.println("7");
        delay(1);
      }


      occupied = readSquare(6,i);
      if(occupied){
        turnOnLED(6,y);
        Serial.println("6");
        delay(1);
      }


      occupied = readSquare(5,i);
      if(occupied){
        turnOnLED(5,y);
        Serial.println("5");
        delay(1);
      }


      occupied = readSquare(4,i);
      if(occupied){
        turnOnLED(4,y);
        Serial.println("4");
        delay(1);
      }


      occupied = readSquare(3,i);
      if(occupied){
        turnOnLED(3,y);
        Serial.println("3");
        delay(1);
      }


      occupied = readSquare(2,i);
      if(occupied){
        turnOnLED(2,y);
        Serial.println("2");
        delay(1);
      }


      occupied = readSquare(1,i);
      if(occupied){
        turnOnLED(1,y);
        Serial.println("1");
        delay(1);
      }
      */
      
      
      

      //turnOffAll();
      

      break;
    }
    default:{
      Serial.println("Why");
      delay(1000);
      break;
    }

  }

}



