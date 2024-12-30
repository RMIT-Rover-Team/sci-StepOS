# StepOS Operating System

Version `1.18`

(C) Copyright 2025 RMIT Rover Team, Space Resources Subsystem.

Authors: Aston Bolwell, Andrew Fagioli.

## What is StepOS?

StepOS is a pseudo-operating system which mimics a CLI OS. It does this by utilising a microcontroller's serial interface, which allows for 2 way communication between the microcontroller and a computer.

This CLI-esque interface allows for the control of stepper motors which are connected to the microcontroller running the firmware.

## Scope and Limitations

Currently, StepOS only supports the current implementation:
- Running on a Raspberry Pi Pico
- Using A4988 Stepper Drivers
  
Additionally, the following limitations are present:
- Only 2 stepper motors are supported
- Step position is based on a 16th step resolution
- No direct control over the steps - only a position can be inputted

## Use Case

StepOS was developed exclusivley for the RMIT Rover Team, specifically the Space Resources Subsytem, to implement the two stepper drivers required for a test-rig.

Not only was StepOS intended to be used and controlled by a user, but it was also intended to be controlled by additional software on a computer which could run automated tests along with the apparatus attached to the stepper-motor controlled rigs.

For more information on the RMIT Rover, visit [rmitroverteam.com](https://www.rmitroverteam.com/)
