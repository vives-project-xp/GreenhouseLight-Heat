# GreenhouseLight-Heat

![Project Picture AI](./resources/images/ai_image.jpg)

# Introduction

This project automates the control of light and heat within a greenhouse to optimize plant growth year-round. Through a network of sensors and actuators, the system continuously monitors and adjusts environmental conditions to create the ideal environment for plants.

# The team

- Thibaut Schroyens (SCRUM Master)
- Sam De Wispelaere
- Joren Vandewalle

# features

- Light Control: Adjusts lighting with blue and red LEDs to stimulate plant growth and manages shading to prevent overexposure.
- Temperature Regulation: Monitors greenhouse temperature, controlling electric heaters to add warmth or opening windows for cooling.
- All LEDs, heaters, and actuators run on standard power (230V), making installation straightforward and accessible without specialized wiring.

# Hardware

- ledstrip: SK6812 RGBW LED Strip 5m
- esp32: ESP32-C3-DevKitC-02
- voedingen: Mean Well Rs-50-5 (5V-10A) + Mean Well UHP-200R-36 (36V-5.6A)
- stappenmotor: Joy-it Nema23-02 (2.5A - 1.2Nm)
- level shifter: Sparkfun Logic converter, bi-directional
- verwarmingselement: VONROC heater marsili compact -2000W
- Smart plug : Wiz Smart plug
- stappenmotordriver: Joy-it SBC-MD-TB6600
- Shade cloth
- Lamp construction
- Shade cloth roll-up construction

# 3D Prints

- Plate for lamps
 ![Plate for lamps](./resources/images/LampPlaat.png)
- Corner for attaching shade cloth (?)
- Housing for weight (weight = soil) (?)
  ![Plate for lamps](./resources/images/WeightForDirt.png)
- Electric motor attachments (?)

# Software

- The only software used is WLED for controlling our LED's.
- the rest of the software is provided by the other teams

# Bom list


# Step by step plan

For a step by step plan click [here](./Documentatie/README.md).
