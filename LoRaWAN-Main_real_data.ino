#include <TinyGPS++.h>
#include "LoRaWan_APP.h"

// ----------- GPS Setup --------------
HardwareSerial gpsSerial(1);  // Using UART1 for GPS
TinyGPSPlus gps;

// Define the RX and TX pins for the GPS module (adjust as needed)
#define GPS_RX_PIN 3   // Connect L80R TX to ESP32 GPIO3
#define GPS_TX_PIN 1   // Connect L80R RX to ESP32 GPIO1

// ----------- LoRaWAN Setup --------------
// OTAA parameters
uint8_t devEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x06, 0x22, 0xAF };
uint8_t appEui[] = { 0x0F, 0x20, 0x30, 0x04, 0x50, 0xCC, 0x09, 0x60 };
uint8_t appKey[] = { 0x21, 0x01, 0xD5, 0x49, 0x34, 0xA8, 0xD8, 0xEE,
                     0xA1, 0xCA, 0xC4, 0x70, 0xA3, 0x3A, 0x3F, 0x23 };

// ABP parameters (if using ABP)
//uint8_t nwkSKey[] = { ... };
//uint8_t appSKey[] = { ... };
//uint32_t devAddr = (uint32_t)0x007e6ae1;

// LoraWan channels mask, region, class, etc.
uint16_t userChannelsMask[6] = { 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;
DeviceClass_t  loraWanClass = CLASS_A;

// Application settings
uint32_t appTxDutyCycle = 15000; // ms between uplinks
bool overTheAirActivation = true;
bool loraWanAdr = true;
bool isTxConfirmed = true;
uint8_t appPort = 2;
uint8_t confirmedNbTrials = 4;

// ----------- Prepare Payload --------------
// Instead of sending fixed mock coordinates, we pack the current GPS reading.
// We use a fixed-point representation (latitude and longitude multiplied by 1e5).
static void prepareTxFrame( uint8_t port )
{
  // If a valid GPS location is available, use it; otherwise, use 0.0 as a fallback.
  double lat = gps.location.isValid() ? gps.location.lat() : 0.0;
  double lng = gps.location.isValid() ? gps.location.lng() : 0.0;
  
  int32_t latitude  = (int32_t)(lat * 1e5);
  int32_t longitude = (int32_t)(lng * 1e5);

  // Debug: Print what will be sent.
  Serial.print("Preparing payload: Latitude=");
  Serial.print(lat, 6);
  Serial.print(", Longitude=");
  Serial.println(lng, 6);

  // Prepare an 8-byte payload: 4 bytes for latitude, 4 bytes for longitude.
  appDataSize = 8;
  appData[0] = (latitude >> 24) & 0xFF;
  appData[1] = (latitude >> 16) & 0xFF;
  appData[2] = (latitude >> 8)  & 0xFF;
  appData[3] = latitude & 0xFF;

  appData[4] = (longitude >> 24) & 0xFF;
  appData[5] = (longitude >> 16) & 0xFF;
  appData[6] = (longitude >> 8)  & 0xFF;
  appData[7] = longitude & 0xFF;
}

// ----------- Setup Function --------------
void setup() {
  // Debug Serial
  Serial.begin(115200);
  delay(1000); // Allow time for the Serial Monitor to open
  Serial.println("System starting...");

  // Initialize GPS Hardware Serial
  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  Serial.println("GPS hardware serial started");

  // Initialize LoRaWAN (Heltec board specifics)
  Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);
}

// ----------- Main Loop --------------
void loop()
{
  // ---- Read GPS Data ----
  // Continuously check for new data from the GPS module.
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    gps.encode(c);
  }

  // Optionally, print updated GPS coordinates for debugging.
  if (gps.location.isUpdated()) {
    Serial.print("GPS Update -> Latitude: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print("  Longitude: ");
    Serial.println(gps.location.lng(), 6);
  }

  // ---- LoRaWAN State Machine ----
  switch( deviceState )
  {
    case DEVICE_STATE_INIT:
    {
#if(LORAWAN_DEVEUI_AUTO)
      LoRaWAN.generateDeveuiByChipID();
#endif
      LoRaWAN.init(loraWanClass, loraWanRegion);
      // Set default data rate
      LoRaWAN.setDefaultDR(3);
      break;
    }
    case DEVICE_STATE_JOIN:
    {
      LoRaWAN.join();
      break;
    }
    case DEVICE_STATE_SEND:
    {
      // Build the uplink payload using real GPS data.
      prepareTxFrame( appPort );
      LoRaWAN.send();
      deviceState = DEVICE_STATE_CYCLE;
      break;
    }
    case DEVICE_STATE_CYCLE:
    {
      // Schedule the next uplink transmission.
      txDutyCycleTime = appTxDutyCycle + randr( -APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND );
      LoRaWAN.cycle(txDutyCycleTime);
      deviceState = DEVICE_STATE_SLEEP;
      break;
    }
    case DEVICE_STATE_SLEEP:
    {
      LoRaWAN.sleep(loraWanClass);
      break;
    }
    default:
    {
      deviceState = DEVICE_STATE_INIT;
      break;
    }
  }
}
