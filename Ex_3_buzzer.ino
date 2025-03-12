#include <pitches.h>

int melody[] = {
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6
};
int duration = 500; //500 ms
bool turnOffBuzzer = false; // bool works with false and true

void setup() {

  // put your setup code here, to run once:

}

void loop() {

  for (int thisNote = 0; thisNote < 8 ; thisNote++) {

    if (turnOffBuzzer) {
      noTone(D1); // Stop the buzzer
      break; // Exit the loop
    }
    //pin D1 controls sound, emitting each note every 'duration' ms
    tone(D1, melody[thisNote], duration);
    //output voice after several min
    delay(1000);
  }
  noTone(D1);
  turnOffBuzzer = true;
  // restart after 2 secs
  delay(2000);

}
