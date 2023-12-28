#define BLYNK_TEMPLATE_ID "TMPL63aM_ruoh"
#define BLYNK_TEMPLATE_NAME "test1"
#define BLYNK_AUTH_TOKEN "I9Xjh73bpGQZQ8mYB6-c5bsMpLe9Bt8J"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <string.h>

#include <DHT.h>
#include <Wire.h>
#include <string.h>

//define sensor
#define DHTPIN1 19
#define DHTPIN2 15
#define sensorPower 22
#define RAIN_SENSOR 23
#define DHTTYPE DHT22
#define GREEN_LED 21
#define LED 18

DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);


//define virture pin
#define VPIN_BUTTON_0    V0
#define VPIN_BUTTON_1    V1
#define VPIN_BUTTON_2    V2 
#define VPIN_BUTTON_3    V3
#define VPIN_BUTTON_4    V4 


int RAIN_SENSOR_VALUE;
char auth[] = BLYNK_AUTH_TOKEN;
int LED_VALUE;
int button;

// Your WiFi Credentials.
// Set password to "" for open networks.
char ssid[] = "Revenge";
char pass[] = "123456789";

void setup(){
  Serial.begin(9600);
  dht1.begin();
  dht2.begin();
  digitalWrite(sensorPower, LOW);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(LED, OUTPUT);
  Blynk.begin(auth, ssid, pass);
}

void loop(){
  Blynk.run();
  delay(2000);
  float h1 = dht1.readHumidity();
  float t1 = dht1.readTemperature();
  float f1 = dht1.readTemperature(true);

  if (isnan(h1) || isnan(t1) || isnan(f1)) {
    Serial.println(F("Failed to read from DHT sensor1!"));
    return;
  }
  float hif = dht1.computeHeatIndex(f1, h1);
  float hic = dht1.computeHeatIndex(t1, h1, false);

//OUTSIDE DHT
  float h2 = dht2.readHumidity();
  float t2 = dht2.readTemperature();
  float f2 = dht2.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h2) || isnan(t2) || isnan(f2)) {
    Serial.println(F("Failed to read from DHT sensor2!"));
    return;
  }

  float hif2 = dht2.computeHeatIndex(f2, h2);
  float hic2 = dht2.computeHeatIndex(t2, h2, false);

  RAIN_SENSOR_VALUE = digitalRead(RAIN_SENSOR);
  if (RAIN_SENSOR_VALUE == 0 ){
    digitalWrite(GREEN_LED, LOW);
  }
  else{
    digitalWrite(GREEN_LED, HIGH);
  }


  if (RAIN_SENSOR_VALUE == 0 )
  {
    Blynk.logEvent("rain", "Water Detected!");
    Blynk.virtualWrite(VPIN_BUTTON_2, "Water Detected!!");
  }
  else if (RAIN_SENSOR_VALUE == 1 )
  {
    Blynk.virtualWrite(VPIN_BUTTON_2, "No Water Detected.");
  }
  Blynk.virtualWrite(VPIN_BUTTON_0, "Temprature: " + String(t1) + " " + "Humidity: " + String(h1));
  Blynk.virtualWrite(VPIN_BUTTON_1, "Temprature: " + String(t2) + " " + "Humidity: " + String(h2));
}
BLYNK_WRITE(VPIN_BUTTON_3) {
  button = param.asInt();
  if(button == 1) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
}
