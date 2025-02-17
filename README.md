# LoRa_Quectel_L80R


## Project Overview

This repository contains two main Arduino sketches for LoRaWAN communication using the Heltec board:
- **LoRaWan-Main.ino**: Implements real LoRaWAN communication. This file configures network parameters (OTAA/ABP), channel masks, regional settings, duty cycle, and other transmission parameters. It prepares and sends a simple payload over the LoRaWAN network.
- **LoRaWan-Main_mock_data.ino**: Similar to the main file but uses mock data. Instead of reading sensor input, it simulates a fixed-point latitude and longitude (e.g., representing San Francisco) for testing purposes. This allows verification of the communication flow without live sensor data.

## Detailed Explanation

### LoRaWan-Main.ino
- **Activation Modes**: Supports both OTAA and ABP, configurable via parameters.
- **Transmission Settings**: Sets application data transmission duty cycle, confirmed/unconfirmed message options, and retries for sending.
- **Payload Preparation**: Prepares a simple payload (set as a sequence of bytes) before sending.
- **State Machine**: Utilizes a state machine (INIT, JOIN, SEND, CYCLE, SLEEP) to manage communication operations.

### LoRaWan-Main_mock_data.ino
- **Mock Data Generation**: Constructs a payload by converting pre-defined latitude and longitude values into fixed-point representation (multiplied by 1e5). The coordinates used are:
  - Latitude: 37.7749 → 3777490
  - Longitude: -122.4194 → -12241940
- **Payload Structure**: The payload includes 4 bytes for latitude and 4 bytes for longitude.
- **Purpose**: Enables testing of the LoRaWAN communication without real sensor input, ensuring the transmission logic is working as expected.

## Usage

1. Open Arduino IDE and load either `LoRaWan-Main.ino` or `LoRaWan-Main_mock_data.ino` from this repository.
2. Adjust network parameters if necessary (e.g., device EUI, application EUI, keys, channels, etc.).
3. Compile and upload the sketch to the Heltec board.
4. Monitor the serial output for debugging and confirmation of successful data transmission.

