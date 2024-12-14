# Install shade Shade cloths

## Requirements

1. **SY42STH47-1206A**
2. **Joy-it SBC-MD-TB6600 Stepper Motor Driver**
3. **Mean Well UHP-200R-36 Power Supply** (36 V, 5.6 A)
4. **ESP32 Microcontroller**
5. Connection cables and optionally: screw terminals or a breadboard.

---

## Step-by-Step Guide

### Step 1 : connecting stepper motor

#### 1. Connecting the Power Supply to the Driver

- Connect the **Mean Well 36 V power supply** to the TB6600 driver's power terminals:
  - **+V**: Connect to the positive output of the power supply.
  - **GND**: Connect to the negative output of the power supply.
- Only turn on the power supply after completing all other connections.

---

#### 2. Connecting the Stepper Motor to the Driver

- Connect the four wires of the SY42STH47-1206A stepmotor to the driver:
  - **A+ and A-**: Connect to the corresponding "A+" and "A-" terminals of the TB6600.

    A+ --> Red 

    A- --> Bleu

  - **B+ and B-**: Connect to the corresponding "B+" and "B-" terminals of the TB6600.

    B+ --> Green

    B- --> Black

- Verify the correct pin order in the motor's datasheet to prevent a short circuit.

---

#### 3. Connecting the ESP32 to the Driver

- Connect the ESP32 to the STEP/DIR inputs of the driver:
  - **DIR+ (Direction)**: Connect to a GPIO pin on the ESP32, for example, GPIO 5.
  - **STEP+ (Pulse)**: Connect to another GPIO pin on the ESP32, for example, GPIO 4.
  - **DIR- and STEP-**: Connect both to the **GND** of the ESP32.
- Ensure the ESP32 and the driver share a common **GND**.

### Images Connections

![Architectuur_Doc](../images/Wiring_architecture_diagram.png)

## Step 2 : Use the right settings for ur driver

### Setting the DIP Switches on the TB6600

- Configure the DIP switches as follows:
  - **Microstepping**: Choose a desired resolution, such as  microsteps.
  - **Current Limit**: Set the current limit to **1 A**, the nominal current of the SY42STH47-1206A is 1.2A.
- Refer to the TB6600 manual for the correct DIP switch settings.
The following table shows the DIP switch settings for configuring the current limit on the TB6600 driver:

| Current (A) | S4  | S5  | S6  |
|-------------|------|------|------|
| 0.5         | ON   | ON   | ON   |
| **1.0**     | **ON**   | **OFF**  | **ON**   |
| 1.5         | ON   | ON   | OFF  |
| 2.0         | ON   | OFF  | OFF  |
| 2.5         | OFF  | ON   | ON   |
| 2.8         | OFF  | OFF  | ON   |
| 3.0         | OFF  | ON   | OFF  |
| 3.5         | OFF  | OFF  | OFF  |

| Micro Step | Pulse/Rev | S1   | S2   | S3   |
|------------|-----------|------|------|------|
| **1**           | **200**       | **ON**   | **ON**   | **OFF**  |
| 2/A        | 400       | ON   | OFF  | ON   |
| 2/B        | 400       | OFF  | ON   | ON   |
| 4          | 800       | ON   | OFF  | OFF  |
| 8          | 1600      | OFF  | ON   | OFF  |
| 16         | 3200      | OFF  | OFF  | ON   |
| 32    | 6400  | OFF  | OFF  | OFF  |

sources:
 https://www.makerguides.com/wp-content/uploads/2019/10/TB6600-Manual.pdf

## Step 3 : Software

### Setting Up Software on the ESP32

- Using the Arduino IDE.

Upload [this code](../../Code/StappenMotorCode/CodeStappenMotorDraaien.ino) to the ESP32 for basic rotation , 1 full rotation right then 1 full rotation left. This way u can varify if it works.

## Step 4 : Installation on serre
