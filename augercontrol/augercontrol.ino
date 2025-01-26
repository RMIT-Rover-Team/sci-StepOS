#include <string.h> // Library for string manipulation

// Pin definitions
#define FS_DIR_PIN 7   // Board Pin 10
#define FS_STEP_PIN 6  // Board Pin 9

#define FD_IN1 20      // Board Pin 27
#define FD_IN2 21      // Board Pin 26
#define FD_PWM 19      // Board Pin 25

#define FS_LIM 27      // Limit switch(es)
#define LIM_STEP_BACK 1000

// Default motor configurations
int FS_motor_config[3] = {1, 2000, 1000}; // {direction, step_delay, step_no}
int FD_motor_config[2] = {1, 50};        // {direction, speed}

const int max_input_size = 30;

void setup() {
  // Stepper motor pin setup
  pinMode(FS_DIR_PIN, OUTPUT);
  pinMode(FS_STEP_PIN, OUTPUT);
  pinMode(FS_LIM, INPUT_PULLUP);

  // Drill motor pin setup
  pinMode(FD_IN1, OUTPUT);
  pinMode(FD_IN2, OUTPUT);
  pinMode(FD_PWM, OUTPUT);

  // Serial communication setup
  Serial.begin(9600);
  Serial.println("FORGE control initialized");
  Serial.println("Enter commands in the format:");
  Serial.println("FS,<dir>,<step_delay>,<step_no>");
  Serial.println("FD,<dir>,<speed>");
}

void loop() {
  if (Serial.available() > 0) {
    char input_buffer[max_input_size] = {0};
    read_serial_input(input_buffer, max_input_size);
    process_command(input_buffer);
  }
}

void read_serial_input(char *buffer, int size) {
  int index = 0;
  while (Serial.available() > 0 && index < size - 1) {
    char received_char = Serial.read();
    if (received_char == '\n' || received_char == '\r') break;
    buffer[index++] = received_char;
  }
  buffer[index] = '\0'; // Null-terminate the string
}

void process_command(const char *input) {
  // Convert input to uppercase for case-insensitive parsing
  char command[max_input_size];
  strcpy(command, input);
  for (int i = 0; command[i]; i++) command[i] = toupper(command[i]);

  // Parse command prefix
  if (strncmp(command, "FS,", 3) == 0) {
    if (parse_fs_command(command + 3)) {
      step_motor(FS_motor_config);
      clear_serial_buffer(); // Clear any leftover characters
    }
  } else if (strncmp(command, "FD,", 3) == 0) {
    if (parse_fd_command(command + 3)) {
      control_drill(FD_motor_config);
      clear_serial_buffer(); // Clear any leftover characters
    }
  } else {
    Serial.println("Invalid command. Please try again.");
    clear_serial_buffer(); // Ensure no leftover characters trigger false errors
  }
}


bool parse_fs_command(const char *input) {
  int params[3] = {0};
  if (parse_parameters(input, params, 3)) {
    if (params[0] < 1 || params[0] > 2 || params[1] < 600 || params[2] <= 0) {
      Serial.println("Invalid FS parameters. Please retry.");
      return false;
    }
    FS_motor_config[0] = params[0];
    FS_motor_config[1] = params[1];
    FS_motor_config[2] = params[2];
    return true;
  }
  return false;
}

bool parse_fd_command(const char *input) {
  int params[2] = {0};
  if (parse_parameters(input, params, 2)) {
    if (params[0] < 0 || params[0] > 2 || params[1] < 0 || params[1] > 255) {
      Serial.println("Invalid FD parameters. Please retry.");
      return false;
    }
    FD_motor_config[0] = params[0];
    FD_motor_config[1] = params[1];
    return true;
  }
  return false;
}

bool parse_parameters(const char *input, int *params, int count) {
  char *token;
  char input_copy[max_input_size];
  strcpy(input_copy, input);

  token = strtok(input_copy, ",");
  for (int i = 0; i < count; i++) {
    if (token == NULL) return false;
    params[i] = atoi(token);
    token = strtok(NULL, ",");
  }
  return (token == NULL); // Ensure no extra parameters
}

void step_motor(int *config) {
  digitalWrite(FS_DIR_PIN, config[0] == 1 ? HIGH : LOW);
  for (int i = 0; i < config[2]; i++) {
    if(!digitalRead(FS_LIM)){
      Serial.println("Limit switch triggered.");
      for (int j = 0; j < LIM_STEP_BACK; j++) {
        digitalWrite(FS_DIR_PIN, config[0] == 1 ? LOW : HIGH);
        step_motor_unsafe(config[1]);
      }
      break;
    }
    step_motor_unsafe(config[1]);
  }
  Serial.println("Stepper motor action completed.");
}

void step_motor_unsafe(int delay) {
  digitalWrite(FS_STEP_PIN, HIGH);
  delayMicroseconds(delay);
  digitalWrite(FS_STEP_PIN, LOW);
  delayMicroseconds(delay);
}

void control_drill(int *config) {
  switch (config[0]) {
    case 0: // Brake
      digitalWrite(FD_IN1, LOW);
      digitalWrite(FD_IN2, LOW);
      analogWrite(FD_PWM, 0);
      break;
    case 1: // Clockwise
      digitalWrite(FD_IN1, HIGH);
      digitalWrite(FD_IN2, LOW);
      analogWrite(FD_PWM, config[1]);
      break;
    case 2: // Counterclockwise
      digitalWrite(FD_IN1, LOW);
      digitalWrite(FD_IN2, HIGH);
      analogWrite(FD_PWM, config[1]);
      break;
  }
  Serial.println("Drill motor action completed.");
}

void clear_serial_buffer() {
  while (Serial.available() > 0) {
    Serial.read(); // Clear all available characters in the buffer
  }
}
