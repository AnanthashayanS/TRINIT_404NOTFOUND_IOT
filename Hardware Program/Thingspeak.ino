#include <WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>

// Replace with your network credentials
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Replace with your ThingSpeak API key
const char* apiKey = "X4D924ZEWGCO7H1C";

// Replace with your ThingSpeak channel ID
unsigned long channelID = 17566200;

// Initialize WiFi client
WiFiClient client;

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize ThingSpeak client
  ThingSpeak.begin(client);
}

void loop() {
  // Read data from sensors or other sources
  int value1 = 5;
  float value2 = 1.45;
  String value3 = "Need For Speed";
  float value4 = 2.5;

  // Send data to ThingSpeak
  ThingSpeak.setField(1, value1);
  ThingSpeak.setField(2, value2);
  ThingSpeak.setField(3, value3);
   ThingSpeak.setField(4, value4);

  int response = ThingSpeak.writeFields(channelID, apiKey);

  if (response == 200) {
    Serial.println("Data sent to ThingSpeak successfully");
  } else {
    Serial.print("Error sending data to ThingSpeak. HTTP error code: ");
    Serial.println(response);
  }

  // Wait for 15 seconds before sending the next data
  delay(15000);
}
