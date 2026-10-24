/*
  Light sensor test for the #13 photoresistor.
  Wiring: one leg -> 5V, other leg -> A0, 10K resistor (#6) from A0 to GND

  Open Tools -> Serial Monitor (9600 baud) and watch the numbers.
  Cover the sensor with your finger: the number should drop a lot.
  The red LED (#2) on pin 8 lights up when the reading falls below
  70% of what it saw at startup, same as the real trap.
*/

const int LIGHT_PIN = A0;
const int LED_PIN   = 8;

int threshold;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  delay(500);
  threshold = analogRead(LIGHT_PIN) * 0.70;
  Serial.print("Trips below: ");
  Serial.println(threshold);
}

void loop() {
  int light = analogRead(LIGHT_PIN);
  Serial.println(light);
  digitalWrite(LED_PIN, light < threshold ? HIGH : LOW);
  delay(200);
}
