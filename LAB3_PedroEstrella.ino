#include "DHT.h"  // Biblioteca para sensor DHT

#define DHTTYPE DHT11   // Define o tipo do sensor (DHT11)
#define dht_dpin D1     // Define o pino do sensor de temperatura/umidade

DHT dht(dht_dpin, DHTTYPE);  // Inicializa o sensor DHT11

#define LED_PIN D2   // Define o pino onde o LED está conectado

void setup() {
  pinMode(LED_PIN, OUTPUT); // Configura o pino do LED como saída
  dht.begin();              // Inicializa o sensor DHT
  Serial.begin(9600);       // Inicia a comunicação serial com velocidade de 9600 bps
  delay(10);                // Pequeno atraso para garantir inicialização correta
  Serial.println("Inicializando ESP8266...");
}

void loop() {
  delay(1000);  // Aguarda 1 segundo entre as medições
  
  float h = dht.readHumidity();    // Lê a umidade do sensor DHT11
  float t = dht.readTemperature(); // Lê a temperatura do sensor DHT11

  // Exibe os valores lidos no monitor serial
  Serial.print("Current Humidity = ");
  Serial.print(h);
  Serial.println("% ");
  
  Serial.print("Temperature = ");
  Serial.print(t);
  Serial.println("C ");

  // Verifica as condições para ativar o LED
  if (t > 28 || h > 60) { 
    Serial.println("WARNING: Hight Temperature or Humidity!");
    digitalWrite(LED_PIN, HIGH);  // Liga o LED Blink (começa a mudar de cor automaticamente)
  } 
  else {
    digitalWrite(LED_PIN, LOW);   // Desliga o LED Blink
  }
}
