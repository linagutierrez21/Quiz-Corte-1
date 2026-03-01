// --- CÓDIGO ARDUINO UNO ---
// Desarrollado para el sistema de detección de postura

const int RED_LED = 8;    // Indicador "PARADO"
const int BLUE_LED = 9;  // Indicador "SENTADO"

void setup() {
  Serial.begin(9600);
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  
  // Test inicial
  digitalWrite(RED_LED, HIGH);
  digitalWrite(BLUE_LED, HIGH);
  delay(1000);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
}

void loop() {
  if (Serial.available() > 0) {
    char receivedChar = Serial.read();
    
    // El protocolo espera 'S' para Standing o 'C' para Sitting
    if (receivedChar == 'S') {
      digitalWrite(RED_LED, HIGH);
      digitalWrite(BLUE_LED, LOW);
    } 
    else if (receivedChar == 'C') {
      digitalWrite(RED_LED, LOW);
      digitalWrite(BLUE_LED, HIGH);
    }
  }
}