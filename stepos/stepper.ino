#include "definitions.h"

double stepper_pos[] = {0,0};

void stepperInit(){
  pinMode(STEPPER_A_STEP, OUTPUT);
  pinMode(STEPPER_A_DIR,  OUTPUT);
  pinMode(STEPPER_A_SPD,  OUTPUT);
  pinMode(STEPPER_A_ENA,  OUTPUT);
  pinMode(STEPPER_A_LIM,  INPUT_PULLUP);
  
  pinMode(STEPPER_B_STEP, OUTPUT);
  pinMode(STEPPER_B_DIR,  OUTPUT);
  pinMode(STEPPER_B_SPD,  OUTPUT);
  pinMode(STEPPER_B_ENA,  OUTPUT);
  pinMode(STEPPER_B_LIM,  INPUT_PULLUP);

  stepperEnable('A', false);
  stepperEnable('B', false);
}

void stepperEnable(char motor, boolean enabled){
  int pin_ena = motor=='A'?STEPPER_A_ENA:STEPPER_B_ENA;
  digitalWrite(pin_ena, enabled?LOW:HIGH);
}

void stepperStepSlow(char motor, int steps){
  int pin_spd = motor=='A'?STEPPER_A_SPD:STEPPER_B_SPD;
  digitalWrite(pin_spd, HIGH);
  stepperStep(motor, steps, 1);
  digitalWrite(pin_spd, LOW);
}

void stepperStep(char motor, int steps){
    stepperStep(motor, steps, 16);
}

void stepperStep(char motor, int steps, int stepSize){
  digitalWrite(LED, HIGH);

  int dir_sign = constrain(steps, -1, 1);
  boolean dir = dir_sign == ((motor=='A'?STEPPER_A_FLIP:STEPPER_B_FLIP)?1:-1);
  steps = abs(steps);
  int pin_step = motor=='A'?STEPPER_A_STEP:STEPPER_B_STEP;
  int pin_dir = motor=='A'?STEPPER_A_DIR:STEPPER_B_DIR;
  int pin_lim = motor=='A'?STEPPER_A_LIM:STEPPER_B_LIM;
  int stepMax = motor=='A'?STEPPER_A_MAX:STEPPER_B_MAX;
  
  digitalWrite(pin_dir, dir?HIGH:LOW);

  if(DEBUG){
    // DEBUG: Print step command details
    Serial.print(steps);
    Serial.print(", ");

    Serial.print(dir);
    Serial.print(", ");

    Serial.print(pin_dir);
    Serial.print(", ");

    Serial.println(pin_step);   
  }

  double tempMotPos;
  for (int i = 0; i < steps; i++){
    // Step Motor
    digitalWrite(pin_step, HIGH);
    delayMicroseconds(STEP_US);
    digitalWrite(pin_step, LOW);
    delayMicroseconds(STEP_US);

    // Safety Checks
    tempMotPos = stepperGetPos(motor) + dir_sign * stepSize;
    stepperSetPos(motor, tempMotPos);
    if (tempMotPos + dir_sign * stepSize > stepMax) break;
    if (dir_sign == -1 && !digitalRead(pin_lim))    break;

    // DEBUG: Print stepper position
    if(DEBUG){
      Serial.print("Stepper Pos: ");
      Serial.println(tempMotPos);  
    }
  }

  digitalWrite(LED, LOW);
}

void stepperHome(char motor){
    int pin_lim = motor=='A'?STEPPER_A_LIM:STEPPER_B_LIM;

    stepperEnable(motor, true);

    // Coarse Home
    while(digitalRead(pin_lim)){
      stepperStep(motor, -1);
    }

    stepperStep(motor, 30);

    // Fine Home
    while(digitalRead(pin_lim)){
      stepperStepSlow(motor, -1);
    }

    stepperStepSlow(motor, 80);

    // Ultra Fine Home
    while(digitalRead(pin_lim)){
      stepperStepSlow(motor, -1);
      delay(100);
    }

    stepperStep(motor, 100);
    stepperEnable(motor, false);
    stepperSetPos(motor, 16*100);
}

void stepperSetPos(char motor, int pos){
  stepper_pos[motor=='A'?0:1] = pos;
}

int stepperGetPos(char motor){
  return stepper_pos[motor=='A'?0:1];
}
