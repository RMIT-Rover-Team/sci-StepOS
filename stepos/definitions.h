#define DEBUG false
#define OS_VER_MAJ 1
#define OS_VER_MIN 18

#define ELEMENTS(x) (sizeof(x) / sizeof(x[0]))

#define STEPPER_A_STEP  6
#define STEPPER_A_DIR   7
#define STEPPER_A_LIM   27
#define STEPPER_A_SPD   26
#define STEPPER_A_ENA   22
#define STEPPER_A_FLIP  false
#define STEPPER_A_MAX   7200

#define STEPPER_B_STEP  2
#define STEPPER_B_DIR   3
#define STEPPER_B_LIM   21
#define STEPPER_B_SPD   20
#define STEPPER_B_ENA   19
#define STEPPER_B_FLIP  true
#define STEPPER_B_MAX   8000

#define STEP_US         1000
#define LED             25

void stepperInit();
void stepperEnable(char motor, boolean enabled);
void stepperStepSlow(char motor, int steps);
void stepperStep(char motor, int steps, int stepSize);
void stepperStep(char motor, int steps);
void stepperHome(char motor);
void stepperSetPos(char motor, int pos);
int stepperGetPos(char motor);

void terminalInit();
int terminalPrompt(const char* message, const char* options, int optionCount);
int terminalGetNumber(const char* message, int minimum, int maximum);
