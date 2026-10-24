/*
  Arduino Mousetrap v1
  Board: Inland Mega 2560 (#44)

  How it works:
    - The servo (#23) holds the door open.
    - The photoresistor (#13) watches the light inside the trap.
    - When a mouse walks in and blocks the light, the door snaps shut,
      the red LED (#2) turns on and the active buzzer (#9) beeps.
    - Press the button (#11) to open the door and re-arm the trap.

  Wiring (see README.md for the full table):
    Servo signal (orange) -> pin 9, red -> 5V, brown -> GND
    Photoresistor         -> 5V and A0, 10K resistor (#6) from A0 to GND
    LED long leg          -> 220 ohm resistor (#5) -> pin 8, short leg -> GND
    Active buzzer +       -> pin 6, - -> GND
    Button                -> pin 2 and GND
*/

#include <Servo.h>

// ---- Pins ----
const int SERVO_PIN  = 9;
const int LIGHT_PIN  = A0;
const int LED_PIN    = 8;
const int BUZZER_PIN = 6;
const int BUTTON_PIN = 2;

// ---- Settings you can tune ----
const int OPEN_ANGLE   = 90;   // servo angle that holds the door open
const int CLOSED_ANGLE = 0;    // servo angle that lets the door shut
const float TRIGGER_DROP = 0.70;          // trip when light falls below 70% of normal
const unsigned long TRIGGER_HOLD_MS = 150; // light must stay low this long (ignores flicker)
const unsigned long ARM_DELAY_MS = 3000;   // time to move your hand away after arming
const unsigned long ALARM_MS = 5000;       // how long the buzzer beeps after a catch

Servo door;

enum TrapState { ARMING, ARMED, CAUGHT };
TrapState state;

int lightThreshold = 0;
unsigned long stateStartedAt = 0;
unsigned long darkSince = 0;

// Average a few readings so one noisy value doesn't trip the trap.
int readLight() {
  long total = 0;
  for (int i = 0; i < 8; i++) {
    total += analogRead(LIGHT_PIN);
  }
  return total / 8;
}

// Measure normal light for one second and set the trip level from it.
void calibrate() {
  long total = 0;
  for (int i = 0; i < 20; i++) {
    total += readLight();
    delay(50);
  }
  int baseline = total / 20;
  lightThreshold = baseline * TRIGGER_DROP;
  Serial.print("Normal light: ");
  Serial.print(baseline);
  Serial.print("  Trips below: ");
  Serial.println(lightThreshold);
}

void startArming() {
  door.write(OPEN_ANGLE);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  state = ARMING;
  stateStartedAt = millis();
  Serial.println("Door open. Arming in 3 seconds...");
}

void closeDoor() {
  door.write(CLOSED_ANGLE);
  digitalWrite(LED_PIN, HIGH);
  state = CAUGHT;
  stateStartedAt = millis();
  Serial.println("Caught! Door closed. Press the button to re-arm.");
}

bool buttonPressed() {
  if (digitalRead(BUTTON_PIN) == LOW) {   // LOW means pressed (INPUT_PULLUP)
    delay(30);                            // debounce
    if (digitalRead(BUTTON_PIN) == LOW) {
      while (digitalRead(BUTTON_PIN) == LOW) {}  // wait for release
      return true;
    }
  }
  return false;
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  door.attach(SERVO_PIN);

  Serial.println("Mousetrap v1 starting");
  startArming();
}

void loop() {
  unsigned long now = millis();

  switch (state) {
    case ARMING:
      // Slow blink while arming so you know to step back.
      digitalWrite(LED_PIN, (now / 250) % 2);
      if (now - stateStartedAt >= ARM_DELAY_MS) {
        digitalWrite(LED_PIN, LOW);
        calibrate();
        darkSince = 0;
        state = ARMED;
        Serial.println("Armed.");
      }
      break;

    case ARMED:
      if (readLight() < lightThreshold) {
        if (darkSince == 0) darkSince = now;
        if (now - darkSince >= TRIGGER_HOLD_MS) {
          closeDoor();
        }
      } else {
        darkSince = 0;
      }
      if (buttonPressed()) startArming();  // re-calibrate if lighting changed
      break;

    case CAUGHT:
      // Beep on and off for a few seconds, then stay quiet with the LED on.
      if (now - stateStartedAt < ALARM_MS) {
        digitalWrite(BUZZER_PIN, (now / 200) % 2);
      } else {
        digitalWrite(BUZZER_PIN, LOW);
      }
      if (buttonPressed()) startArming();
      break;
  }
}
