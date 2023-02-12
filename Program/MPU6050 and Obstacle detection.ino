
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Stepper.h>
#include <WiFi.h>
#include <PubSubClient.h>

// MQTT Server Parameters
const char* MQTT_CLIENT_ID = "arduino-weather-demo";
const char* MQTT_BROKER = "broker.mqttdashboard.com";
const char* MQTT_USER = "";
const char* MQTT_PASSWORD = "";
const char* MQTT_TOPIC = "Accelaration";
String Prev;
float A;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);


#define trigPin 4
#define echoPin 2
const int stepsPerRevolution = 200;

Stepper myStepper(stepsPerRevolution, 32, 33, 25,26);

Adafruit_MPU6050 mpu;

#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);


void setup(void) {
  Serial.begin(115200);

  while (!mpu.begin()) {
    Serial.println("MPU6050 not connected!");
    delay(1000);
  }
  Serial.println("MPU6050 ready!");
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
 
   // Connect to WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin("Wokwi-GUEST", "");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println(" Connected!");

  // Connect to MQTT broker
  Serial.print("Connecting to MQTT server... ");
  mqttClient.setServer(MQTT_BROKER, 1883);
  mqttClient.setCallback(callback);
  while (!mqttClient.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Connected!");
// Start the I2C communication
  Wire.begin();

  // Start the OLED display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  display.clearDisplay();
}

sensors_event_t event;

void loop() {
  mpu.getAccelerometerSensor()->getEvent(&event);
  A=event.acceleration.x;

  Serial.print("[");
  Serial.print(millis());
  Serial.print("] X: ");
  Serial.print(event.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(event.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(event.acceleration.z);
  Serial.println(" m/s^2");
  delay(500);




  // send ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // measure distance
  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;

   // print distance to serial monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  //OLED

 display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Acceleration:");
  display.setCursor(0, 10);
 display.print(event.acceleration.x);
display.setCursor(0, 20);
  display.print("Distance:");
  display.setCursor(0, 30);
  display.print(distance);
display.display();

if(distance<100){
  Serial.println("Obstacle detected");
  myStepper.setSpeed(30);
  myStepper.step(stepsPerRevolution);
}
else{
  Serial.println("Good to go");
  myStepper.setSpeed(90);
  myStepper.step(stepsPerRevolution);
}
String message = "{\"Accelaration\": " + String(A) + ", \"Distance\": " + String(distance) + "}";
  if (message != Prev) {
    Serial.println("Updated!");
    Serial.print("Reporting to MQTT topic ");
    Serial.print(MQTT_TOPIC);
    Serial.print(": ");
    Serial.println(message);
    mqttClient.publish(MQTT_TOPIC, message.c_str());
    Prev = message;
  } else {
    Serial.println("No change");
  }
  mqttClient.loop();
  delay(1000);


 
 
}



void callback(char* topic, byte* payload, unsigned int length) {
  // Handle MQTT messages received (if any)
}