char ON_OFF;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
}


void loop() {         // the loop function runs over and over again forever
  
  if (Serial.available() > 0)
  {
    ON_OFF = Serial.read();
    if (ON_OFF == 'A')
    {
      digitalWrite(LED_BUILTIN, HIGH);  // turn the LED off (HIGH is the voltage level)
    }
    if (ON_OFF == 'V')
    {
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on by making the voltage LOW
    }
  }
}
