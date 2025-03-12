#include <pitches.h>
#include <NewPingESP8266.h>

// Buzzer and melody setup
#define BUZZER_PIN D3 // buzzer channel
int melody[] = {
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6
}; //array of notes
int duration = 500; // duration of each note

// Ultrasonic sensor setup
#define TRIGGER_PIN D1 //ultrasonic sensor channel (trigger)
#define ECHO_PIN D2 // ultrasonic sensor channel (echo)
#define MAX_DISTANCE 200 // Max distance in cm
NewPingESP8266 sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); //initialize ultrasonic sensor with trigger and echo pin

// Threshold distance to trigger buzzer
#define THRESHOLD_DISTANCE 5 // 5 cm: distance that will trigger bzzer

void setup() {
  pinMode(BUZZER_PIN, OUTPUT); // buzzer : output
  Serial.begin(9600); // start seerial communication at 9600 baud
}

void loop() {
  delay(500); // Wait 500 ms for avoiding excessive pinging
  unsigned int uS = sonar.ping_cm(); // Get distance(uS) in cm and store it in "distance" variable

  Serial.print("Distance: "); //print to serial monitor
  Serial.print(uS / US_ROUNDTRIP_CM); // print distance variable 
  Serial.println(" cm"); // print "cm" and ln for new line

  if (uS > 0 && uS <= THRESHOLD_DISTANCE) { // check if an object is in the threshold distance
    for (int thisNote = 0; thisNote < 8; thisNote++) { // look of the melody array, 0 first, <8 0-7 notes; ++ increase by 1 through melody
      tone(BUZZER_PIN, melody[thisNote], duration); // Play the note on the buzzer for "duration"
      delay(100); // delay between notes in ms
    }
    noTone(BUZZER_PIN); // Turn off buzzer after melody
  } else {
    noTone(BUZZER_PIN); // and Turn off buzzer if no object in on the threshold distance
  }
}
