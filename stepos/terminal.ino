#include "definitions.h"

void terminalInit(){
  Serial.begin(115200);
  while (!Serial);
  delay(500);
  
  Serial.print("StepOS Operating System [Version ");
  Serial.print(OS_VER_MAJ);
  Serial.print(".");
  Serial.print(OS_VER_MIN);
  Serial.println(" PROD-BUILD target: pi_pico]");
  Serial.println("(C) Copyright 2025 RMIT Rover Team, Space Resources Subsystem.");
  Serial.println("Authors: Aston Bolwell, Andrew Fagioli.");
  Serial.print("\n");
}

int terminalPrompt(const char* message, const char* options, int optionCount){

  Serial.print(message);
  Serial.print(" (");
  
  for(int i=0; i < optionCount; i++){
    Serial.print(options[i]);
    if(i<optionCount-1) Serial.print("|");
  }
  
  Serial.print(") [");
  Serial.print(options[0]);
  Serial.print("]: ");

  char incomingByte;
  while(!Serial.available() > 0);
  incomingByte = Serial.read();
  while(Serial.available() > 0) Serial.read();
  Serial.println(incomingByte);
  
  for(int i=0; i < optionCount; i++){
    if(options[i] == incomingByte) return i;
  }

  return 0;
}

int terminalGetNumber(const char* message, int minimum, int maximum){
  Serial.print(message);
  Serial.print("[");
  Serial.print(minimum, DEC);
  Serial.print("-");
  Serial.print(maximum, DEC);
  Serial.print("]: ");

  char incomingByte;
  int number = 0;
  while(!Serial.available() > 0);
  while(Serial.available() > 0){
    incomingByte = Serial.read();
    if( !isDigit(incomingByte) ) break;
    number *= 10;
    number += incomingByte - '0';
  };
  while(Serial.available() > 0) Serial.read();
  number = constrain(number,minimum,maximum);
  Serial.println(minimum, DEC);

  return number;
}
