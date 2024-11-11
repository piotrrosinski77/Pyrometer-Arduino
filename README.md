# Pyrometer-Arduino
🌡️ A simple pyrometer (infrared thermometer) using an Arduino and MLX90614

This project involves creating a simple pyrometer (infrared thermometer) using an Arduino IDE. The device measures the temperature of objects without contact using an infrared sensor (e.g., MLX90614) and displays the result on an LCD screen or sends the data to a computer.

## Overview

This project utilizes the MLX90614 infrared temperature sensor to detect the surface temperature of objects. The sensor measures infrared radiation and converts it into a temperature reading, which can then be displayed on an LCD or transmitted to a computer via serial communication.

## Features:

- Contactless Temperature Measurement: Measure the surface temperature of objects without needing direct contact.
- Accurate Readings: Utilizes the MLX90614, known for its precision and reliability.
- Display Options: Output temperature data to an LCD screen or send it to a computer for logging and analysis.
- Arduino-Compatible: Fully programmable and customizable via the Arduino IDE.

## Components Required:
- Arduino Board (e.g., Uno, Nano)
- MLX90614 Infrared Temperature Sensor
- 16x2 LCD Screen (optional, for local display)
- I2C Module for LCD (optional, if using I2C communication)
- Jumper Wires
- Breadboard (optional)
