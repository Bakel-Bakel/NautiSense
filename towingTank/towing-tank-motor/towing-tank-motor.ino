const int dirPin = 4;
const int stepPin = 3;

const int stepsPerRevolution = 1600;  // 1/8 microstepping
const int rotationDelay = 200;        // microseconds between steps (adjust to control speed)

void setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  digitalWrite(dirPin, HIGH); // Set initial direction
}

void loop() {
  // Rotate continuously at high speed
  for (int i = 0; i < stepsPerRevolution; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(rotationDelay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(rotationDelay);
  }


}
