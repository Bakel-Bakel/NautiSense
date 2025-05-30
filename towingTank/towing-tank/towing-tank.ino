// Arduino Uno + HC-SR04 + Stepper Driver

// HC-SR04 pins
#define TRIG_PIN 8  // Digital Pin 8
#define ECHO_PIN 9  // Digital Pin 9

// Stepper driver pins
const int dirPin  = 4;  // Digital Pin 4
const int stepPin = 3;  // Digital Pin 3

// Constants
const float SOUND_SPEED = 0.0343;         // cm per µs
const int stepsPerRevolution = 1600;      // e.g. 1/8 microstepping
const int rotationDelay = 200;            // µs between steps

void setup() {
  Serial.begin(9600);
  delay(50);

  // Setup ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);

  // Setup stepper motor driver pins
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  digitalWrite(dirPin, HIGH);  // Set initial direction
}

void loop() {
  // --- 1) Measure distance using HC-SR04 ---
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo with a 30 ms timeout
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL);

  if (duration == 0) {
    Serial.println("Distance: Out of range");
  } else {
    float distanceCm = (duration * SOUND_SPEED) / 2.0;
    Serial.print("Distance: ");
    Serial.print(distanceCm, 1);
    Serial.println(" cm");
  }

  // --- 2) Rotate stepper one full revolution ---
  for (int i = 0; i < stepsPerRevolution; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(rotationDelay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(rotationDelay);
  }

  //delay(200);  // Short pause before next cycle
}
