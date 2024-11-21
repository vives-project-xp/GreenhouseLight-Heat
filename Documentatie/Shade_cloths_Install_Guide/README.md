# Install shade Shade cloths

## Step 1 : connecting stepper motor

### Requirements
1. **Nema23-02 Stepper Motor**
2. **Joy-it SBC-MD-TB6600 Stepper Motor Driver**
3. **Mean Well UHP-200R-36 Power Supply** (36 V, 5.6 A)
4. **ESP32 Microcontroller**
5. Connection cables and optionally: screw terminals or a breadboard.

---

### Step-by-Step Guide

#### 1. Connecting the Power Supply to the Driver
- Connect the **Mean Well 36 V power supply** to the TB6600 driver's power terminals:
  - **+V**: Connect to the positive output of the power supply.
  - **GND**: Connect to the negative output of the power supply.
- Only turn on the power supply after completing all other connections.

---

#### 2. Connecting the Stepper Motor to the Driver
- Connect the four wires of the Nema23-02 motor to the driver:
  - **A+ and A-**: Connect to the corresponding "A+" and "A-" terminals of the TB6600.
  - **B+ and B-**: Connect to the corresponding "B+" and "B-" terminals of the TB6600.
- Verify the correct pin order in the motor's datasheet to prevent a short circuit.

---

#### 3. Connecting the ESP32 to the Driver
- Connect the ESP32 to the STEP/DIR inputs of the driver:
  - **DIR+ (Direction)**: Connect to a GPIO pin on the ESP32, for example, GPIO 18.
  - **STEP+ (Pulse)**: Connect to another GPIO pin on the ESP32, for example, GPIO 19.
  - **DIR- and STEP-**: Connect both to the **GND** of the ESP32.
- Ensure the ESP32 and the driver share a common **GND**.

---

#### 4. Setting the DIP Switches on the TB6600
- Configure the DIP switches as follows:
  - **Microstepping**: Choose a desired resolution, such as 16 microsteps.
  - **Current Limit**: Set the current limit to **2.5 A**, the nominal current of the Nema23-02 motor.
- Refer to the TB6600 manual for the correct DIP switch settings.

---

#### 5. Setting Up Software on the ESP32
- Use the Arduino IDE or another development environment.
- Install the **AccelStepper** library for easy stepper motor control.

##### Example Code (not tested yet)
```cpp
#include <AccelStepper.h>

// Define pins
#define STEP_PIN 19
#define DIR_PIN 18

// Configure stepper motor (driver type = 1)
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

void setup() {
  // Set maximum speed and acceleration
  stepper.setMaxSpeed(1000); // Max speed (steps per second)
  stepper.setAcceleration(500); // Acceleration (steps per second^2)
}

void loop() {
  // Rotate motor 1000 steps forward
  stepper.moveTo(1000);
  stepper.runToPosition(); // Wait until target position is reached

  delay(1000); // Wait 1 second

  // Rotate motor 1000 steps backward
  stepper.moveTo(-1000);
  stepper.runToPosition();

  delay(1000); // Wait 1 second
}
