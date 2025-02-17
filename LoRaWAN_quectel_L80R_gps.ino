#include <TinyGPS++.h>
#include <SoftwareSerial.h>  // Or use HardwareSerial if pins allow

TinyGPSPlus gps;
SoftwareSerial gpsSerial(2, TX_PIN); // (RX, TX) for the MCU

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600);  // Confirm L80R’s baud rate
}

void loop() {
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isUpdated()) {
    // Retrieve latitude and longitude
    double lat = gps.location.lat();
    double lng = gps.location.lng();

    // Print for debugging
    Serial.print("Lat: ");
    Serial.print(lat, 6);
    Serial.print("   Lon: ");
    Serial.println(lng, 6);

    // If using LoRaWAN, store lat/lng in a payload to send
    // e.g. convert them to a signed 32-bit int multiplied by 1e5
  }
}
