// ---------------- Global Definitions ----------------
// LoRaWAN parameters (for ABP mode, if needed)
uint8_t nwkSKey[] = { 0x15, 0xb1, 0xd0, 0xef, 0xa4, 0x63, 0xdf, 0xbe, 0x3d, 0x11, 0x18, 0x1e, 0x1e, 0xc7, 0xda, 0x85 };
uint8_t appSKey[] = { 0xd7, 0x2c, 0x78, 0x75, 0x8c, 0xdc, 0xca, 0xbf, 0x55, 0xee, 0x4a, 0x77, 0x8d, 0x16, 0xef, 0x67 };
uint32_t devAddr = (uint32_t)0x007e6ae1;

// ---------------- Include Libraries ----------------
#include <HT_TinyGPS++.h>
#include "LoRaWan_APP.h"  // Heltec LoRaWAN library

// ---------------- GPS Setup ----------------
HardwareSerial gpsSerial(1);  // Using HardwareSerial1 for GPS
TinyGPSPlus gps;

#define GPS_RX_PIN 3  // Adjust if needed
#define GPS_TX_PIN 1  // Adjust if needed

// ---------------- LoRaWAN Settings ----------------
uint8_t devEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x06, 0x22, 0xAF };
uint8_t appEui[] = { 0x0F, 0x20, 0x30, 0x04, 0x50, 0xCC, 0x09, 0x60 };
uint8_t appKey[] = { 0x21, 0x01, 0xD5, 0x49, 0x34, 0xA8, 0xD8, 0xEE,
                     0xA1, 0xCA, 0xC4, 0x70, 0xA3, 0x3A, 0x3F, 0x23 };

uint16_t userChannelsMask[6] = { 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;
DeviceClass_t loraWanClass = CLASS_A;

uint32_t appTxDutyCycle = 15000;
bool overTheAirActivation = true;
bool loraWanAdr = true;
bool isTxConfirmed = true;
uint8_t appPort = 2;
uint8_t confirmedNbTrials = 4;

// ---------------- Payload Preparation ----------------
static void prepareTxFrame(uint8_t port)
{
  // Use current GPS location if available; otherwise send zeros.
  double lat = gps.location.isValid() ? gps.location.lat() : 0.0;
  double lng = gps.location.isValid() ? gps.location.lng() : 0.0;
  
  int32_t latitude = (int32_t)(lat * 1e5);
  int32_t longitude = (int32_t)(lng * 1e5);

  // Debug output
  Serial.print("Preparing payload: Lat=");
  Serial.print(lat, 6);
  Serial.print("  Lon=");
  Serial.println(lng, 6);

  // Pack 8 bytes: 4 bytes for latitude, 4 bytes for longitude.
  appDataSize = 8;
  appData[0] = (latitude >> 24) & 0xFF;
  appData[1] = (latitude >> 16) & 0xFF;
  appData[2] = (latitude >> 8) & 0xFF;
  appData[3] = latitude & 0xFF;
  appData[4] = (longitude >> 24) & 0xFF;
  appData[5] = (longitude >> 16) & 0xFF;
  appData[6] = (longitude >> 8) & 0xFF;
  appData[7] = longitude & 0xFF;
}

// ---------------- Setup ----------------
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("System Starting...");

  // Initialize GPS Hardware Serial
  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  Serial.println("GPS serial started");

  // Initialize LoRaWAN (Heltec board specifics)
  Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);
}

// ---------------- Main Loop ----------------
void loop() {
  // Read GPS data
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    gps.encode(c);
  }
  
  // Optionally print updated GPS coordinates for debugging
  if (gps.location.isUpdated()) {
    Serial.print("GPS: Lat=");
    Serial.print(gps.location.lat(), 6);
    Serial.print(" Lon=");
    Serial.println(gps.location.lng(), 6);
  }

  // LoRaWAN state machine
  switch (deviceState) {
    case DEVICE_STATE_INIT:
      #if(LORAWAN_DEVEUI_AUTO)
        LoRaWAN.generateDeveuiByChipID();
      #endif
      LoRaWAN.init(loraWanClass, loraWanRegion);
      LoRaWAN.setDefaultDR(3);
      break;

    case DEVICE_STATE_JOIN:
      LoRaWAN.join();
      break;

    case DEVICE_STATE_SEND:
      prepareTxFrame(appPort);
      LoRaWAN.send();
      deviceState = DEVICE_STATE_CYCLE;
      break;

    case DEVICE_STATE_CYCLE:
      txDutyCycleTime = appTxDutyCycle + randr(-APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND);
      LoRaWAN.cycle(txDutyCycleTime);
      deviceState = DEVICE_STATE_SLEEP;
      break;

    case DEVICE_STATE_SLEEP:
      LoRaWAN.sleep(loraWanClass);
      break;

    default:
      deviceState = DEVICE_STATE_INIT;
      break;
  }
}
