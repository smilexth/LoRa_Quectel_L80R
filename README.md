# LoRa_Quectel_L80R

## Project Overview

This repository contains three Arduino sketches for LoRaWAN communication using the Heltec board:
- **LoRaWan-Main.ino**: Implements real LoRaWAN communication with standard payloads using either OTAA or ABP.
- **LoRaWan-Main_mock_data.ino**: Uses simulated fixed-point latitude and longitude (e.g., representing San Francisco) for testing without sensor input.
- **LoRaWan-Main_real_data.ino**: Integrates a GPS module to capture real location data and transmit it over LoRaWAN; if GPS data is unavailable, zeros are sent.

## Detailed Explanation

### LoRaWan-Main.ino
- **Activation Modes**: Supports OTAA and ABP.
- **Transmission Settings**: Configures duty cycle, confirmed/unconfirmed messages, and retries.
- **Payload Preparation**: Sends a predefined sequence of bytes.
- **State Machine**: Manages states from INIT to SLEEP.

### LoRaWan-Main_mock_data.ino
- **Mock Data Generation**: Creates payloads from fixed-point latitude (37.7749) and longitude (-122.4194).
- **Purpose**: Validates transmission logic without live sensor data.

### LoRaWan-Main_real_data.ino
- **GPS Integration**: Uses a GPS module to capture real latitude and longitude.
- **Fallback Behavior**: Sends zeros if GPS data is invalid.
- **Debug Information**: Prints GPS coordinates to the serial monitor for verification.

## Usage

1. Open Arduino IDE and select one of the sketches:
   - Use `LoRaWan-Main.ino` for standard communication tests.
   - Use `LoRaWan-Main_mock_data.ino` for simulated data testing.
   - Use `LoRaWan-Main_real_data.ino` for real GPS-based data.
2. Adjust network parameters (device EUI, application keys, channel masks, etc.) if needed.
3. Compile and upload the chosen sketch to your Heltec board.
4. Monitor the serial output for debugging and confirmation of successful data transmission.

