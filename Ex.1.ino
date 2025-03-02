
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  Serial.println("ola");
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on 
  delay(1000);                      // wait for a second 1000 ms
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off 
  delay(1000);                      // wait for a second 1000 ms
}
