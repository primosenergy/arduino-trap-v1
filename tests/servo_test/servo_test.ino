/*
  Servo test for the #23 servo.
  Wiring: orange -> pin 9, red -> 5V, brown -> GND

  Sweeps between the open and closed angles every 2 seconds so you can
  check the servo works and pick good angles for your door.
  Change the two numbers below, upload again, and copy the ones you like
  into OPEN_ANGLE and CLOSED_ANGLE in arduino-trap-v1.ino.
*/

#include <Servo.h>

const int SERVO_PIN    = 9;
const int OPEN_ANGLE   = 90;
const int CLOSED_ANGLE = 0;

Servo door;

void setup() {
  Serial.begin(9600);
  door.attach(SERVO_PIN);
}

void loop() {
  Serial.println("Open");
  door.write(OPEN_ANGLE);
  delay(2000);

  Serial.println("Closed");
  door.write(CLOSED_ANGLE);
  delay(2000);
}
