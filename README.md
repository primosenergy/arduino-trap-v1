# Arduino Mousetrap v1

A live-catch mousetrap built from the Inland Mega 2560 starter kit. A servo holds the door open. When a mouse walks in and blocks the light reaching the photoresistor, the door shuts, the red LED turns on and the buzzer beeps. Press the button to open the door and arm the trap again.

## Parts (kit numbers)

| # | Part | Qty |
|---|------|-----|
| 44 | Mega 2560 board | 1 |
| 43 | USB cable | 1 |
| 23 | Servo motor (with horns and screws) | 1 |
| 39 | 830-hole breadboard | 1 |
| 41 | Jumper wires | ~12 |
| 2 | Red LED | 1 |
| 13 | Photoresistor | 1 |
| 5 | 220Ω resistor (strip labeled 220R) | 1 |
| 6 | 10KΩ resistor (strip labeled 10K) | 1 |
| 9 | Active buzzer (sealed black bottom) | 1 |
| 11 | Large button switch | 1 |
| 42 | 6-cell battery pack | 1 |

## Wiring

Connect the Mega's **5V** pin to the breadboard's red (+) rail and a **GND** pin to the blue (−) rail first. Everything below that says 5V or GND uses those rails.

| Part | Connection |
|------|------------|
| Servo #23 | Orange wire → pin **9**, red → 5V, brown → GND |
| Photoresistor #13 | One leg → 5V. Other leg → pin **A0**, and also through the 10K resistor #6 to GND |
| Red LED #2 | Long leg → 220Ω resistor #5 → pin **8**. Short leg → GND |
| Active buzzer #9 | + leg (longer, marked +) → pin **6**. Other leg → GND |
| Button #11 | One leg → pin **2**. The diagonally opposite leg → GND |

The button needs no resistor, because the sketch turns on the Mega's built-in pull-up.

## Files

| Sketch | What it does |
|--------|--------------|
| `tests/servo_test/servo_test.ino` | Swings the servo between open and closed every 2 seconds. Use it to find the right door angles. |
| `tests/light_sensor_test/light_sensor_test.ino` | Prints light readings to the Serial Monitor and lights the LED when the sensor is covered. |
| `arduino-trap-v1/arduino-trap-v1.ino` | The full trap. |

Open a sketch in the Arduino IDE with **File → Open** and pick the `.ino` file. The IDE expects each sketch to sit in a folder with the same name, which is why each one has its own folder.

## Using the trap

1. Upload `arduino-trap-v1.ino`. Open **Tools → Serial Monitor** at 9600 baud to see what it's doing.
2. On power-up the door opens and the LED blinks for 3 seconds. Keep your hands clear of the sensor.
3. The trap measures the normal light level, then arms. The LED goes off.
4. When the light drops below 70% of normal for 0.15 seconds, the door shuts, the LED turns on and the buzzer beeps for 5 seconds.
5. Press the button to open the door and re-arm. Pressing it while armed also re-measures the light, which helps if the room got brighter or darker.

## Tuning

These are at the top of `arduino-trap-v1.ino`:

- `OPEN_ANGLE` / `CLOSED_ANGLE`: servo positions for the door. Find good values with the servo test sketch.
- `TRIGGER_DROP`: how much darker it must get to trip. Try 0.80 if it doesn't trip, or 0.60 if it trips on its own.
- `TRIGGER_HOLD_MS`: how long it must stay dark before tripping.
- `ARM_DELAY_MS`, `ALARM_MS`: arming delay and beep time.

## Running on batteries

Plug the 6-cell battery pack #42 into the Mega's round barrel jack. The board's regulator powers the servo, LED and buzzer from it. Unplug USB first if you don't need the Serial Monitor.
