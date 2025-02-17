#include <TinyGPS++.h>

// Create a HardwareSerial instance for the GPS (using UART1)
HardwareSerial gpsSerial(1);  // You can also use 2 if desired
TinyGPSPlus gps;

// Define the RX and TX pins for UART1 (adjust these to your wiring)
#define GPS_RX_PIN 16  // Connect L80R TX to ESP32 GPIO16
#define GPS_TX_PIN 17  // Connect L80R RX to ESP32 GPIO17

void setup() {
  // Start the primary Serial for debugging
  Serial.begin(115200);
  delay(1000); // Allow time for Serial Monitor to open

  // Initialize the hardware serial port for the GPS module
  // Syntax: begin(baud rate, config, rxPin, txPin)
  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  Serial.println("GPS hardware serial started");
}

void loop() {
  // Read from the GPS hardware serial and pass data to TinyGPS++
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    gps.encode(c);
  }

  // If the GPS has an updated location, print it out
  if (gps.location.isUpdated()) {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
  }
}
