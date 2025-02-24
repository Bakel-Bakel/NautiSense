#include <Stepper.h>

#define STEPS_PER_REV 2048  // 28BYJ-48 has 2048 steps per revolution
Stepper myStepper(STEPS_PER_REV, 8, 10, 9, 11); // Pins connected to ULN2003

void setup() {
    myStepper.setSpeed(1); // Set motor speed (RPM)
}

void loop() {
    myStepper.step(2048);  // Move one full revolution forward
    delay(1000);
    myStepper.step(-2048); // Move one full revolution backward
    delay(1000);
}
