#define LED 3
#define LDR A0

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);

}

void loop() {
  int deger = analogRead(LDR);
  Serial.println(deger);
  if (deger < 10) {
    digitalWrite(LED, 1);
    
  }
  else
    digitalWrite(LED, LOW);
}
