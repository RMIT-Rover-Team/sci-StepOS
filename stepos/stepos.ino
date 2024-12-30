#include <dummy_rp2040.h>
#include "definitions.h"

int promptResult;
char selectedMotor;

void setup() {
  // Set up stepper motors
  stepperInit();

  // Set up terminal
  terminalInit();

  promptResult = terminalPrompt("[h]ome stepper motors, [c]ontinue, or [r]eboot? ", "hcr", 3);
  if(promptResult==2){ Serial.println("Restarting OS: Please close the terminal."); rp2040.reboot();}
  if(promptResult==0){
    // Begin home sequence
    Serial.println("\nHoming...");
    stepperHome('A');
    stepperHome('B');
    Serial.println("Homing Complete.\n"); 
  }  
}

void loop() {
  promptResult = terminalPrompt("Select a motor ", "ab", 2);
  selectedMotor = promptResult==0?'A':'B';

  while(1){
    promptResult = terminalPrompt("\nChoose an action:\n[h]ome\n[s]et position\n[e]xit ", "hse", 3);
    if(promptResult==2)break;
    if(promptResult==0){
      Serial.println("\nHoming...");
      stepperHome(selectedMotor);
      Serial.println("Homing Complete.\n");
    }
    if(promptResult==1){
      promptResult = terminalGetNumber("Enter position ", 0, selectedMotor=='A'?STEPPER_A_MAX:STEPPER_B_MAX);
      Serial.println("\nMoving...");
      stepperEnable(selectedMotor, true);
      stepperStepSlow(selectedMotor, promptResult - stepperGetPos(selectedMotor));
      stepperEnable(selectedMotor, false);
      Serial.println("Moving Complete.\n");
    }
  }
}
