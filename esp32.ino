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
#include <LiquidCrystal_I2C.h>

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  


//define sensor
#define DHTPIN1 19
#define DHTPIN2 15
#define sensorPower 34
#define RAIN_SENSOR 35
#define DHTTYPE DHT22
#define BUZZ 33
#define LED 18
#define BUTTON1 12
#define BUTTON2 13
#define RIGHT 2
#define LEFT 4
#define LED_BUTTON 27
#define SS1 32
#define SS2 26
#define BZ 33

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
int button4;

// Your WiFi Credentials.
// Set password to "" for open networks.
char ssid[] = "Revenge";
char pass[] = "123456789";

void setup(){
  Serial.begin(9600);
  dht1.begin();
  dht2.begin();
  digitalWrite(sensorPower, LOW);
  pinMode(BUZZ, OUTPUT);
  pinMode(LED, OUTPUT);
  Blynk.begin(auth, ssid, pass);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(LEFT, OUTPUT);
  pinMode(RIGHT, OUTPUT);
  pinMode(BZ, OUTPUT);
  pinMode(SS1, INPUT);
  pinMode(SS2, INPUT);
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
}

void loop(){
  if ((digitalRead(BUTTON1) == digitalRead(BUTTON2)) & (digitalRead(BUTTON1) == HIGH) ){
    digitalWrite(LEFT, LOW);
    digitalWrite(RIGHT, LOW);
  }
  if ((digitalRead(BUTTON1) == digitalRead(BUTTON2)) & (digitalRead(BUTTON1) == LOW) ){
    digitalWrite(LEFT, LOW);
    digitalWrite(RIGHT, LOW);
  }
  if ((digitalRead(BUTTON1) != digitalRead(BUTTON2)) & (digitalRead(BUTTON1) == HIGH) ){
    digitalWrite(LEFT, HIGH);
    digitalWrite(RIGHT, LOW);
  }
  if ((digitalRead(BUTTON1) != digitalRead(BUTTON2)) & (digitalRead(BUTTON2) == HIGH) ){
    digitalWrite(LEFT, LOW);
    digitalWrite(RIGHT, HIGH);
  }
  Blynk.run();
  delay(1000);
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

  Blynk.virtualWrite(VPIN_BUTTON_0, t1);
  Blynk.virtualWrite(VPIN_BUTTON_1, h1);
  lcd.setCursor(0, 0);
  lcd.print("Nhiet do:" + String(t2));
  lcd.setCursor(0,1);
  lcd.print("Do am:" + String(h2));
}
BLYNK_WRITE(VPIN_BUTTON_3) {
  button = param.asInt();
  if(button == 1) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
}
BLYNK_WRITE(VPIN_BUTTON_4) {
  button4 = param.asInt();
  if(button4 == 1){
    if (RAIN_SENSOR_VALUE == 0 )
    {
      Blynk.logEvent("rain", "Water Detected!");
      Blynk.virtualWrite(VPIN_BUTTON_2, "Water Detected!!");
    }
    else if (RAIN_SENSOR_VALUE == 1 )
    {
      Blynk.virtualWrite(VPIN_BUTTON_2, "No Water Detected.");
    }
    if(digitalRead(SS1) != 1){
      digitalWrite(LEFT, HIGH);
      digitalWrite(RIGHT, LOW);
      digitalWrite(BZ, HIGH);
    }
    else{
      digitalWrite(LEFT, LOW);
      digitalWrite(RIGHT, LOW);
      digitalWrite(BZ, LOW);
    }
  }
  else{
    if(digitalRead(SS2) != 1){
      digitalWrite(LEFT, LOW);
      digitalWrite(RIGHT, HIGH);
    }
    else{
      digitalWrite(LEFT, LOW);
      digitalWrite(RIGHT, LOW);
      digitalWrite(BZ, LOW);
    }
  }
}

