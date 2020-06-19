// Incluimos librería
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DHTTYPE DHT11
#define OLED_RESET 4
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define DHTPIN 12
#define ALERT_LED_PIN 5      // Orange
#define STATUS_LED_PIN 6     // Yellow
#define WORKING_LED_PIN 7    // Red
#define WATER_ENGINE_PIN 4   // Blue
#define BUTTON_PIN 11        // Blue
#define TRIGGER_PIN 2        // Orange
#define ECHO_PIN 3           // Green

#define AIR_VALUE_REFERENCE 581
#define WATER_VALUE_REFERENCE 293
#define MAX_WATER_DISTANCE_CM 4
#define WORKING_INTERVAL_MS 600000
#define WORKING_ENGINE_INTERVAL_MS 3600000

Adafruit_SSD1306 display(OLED_RESET);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);

  pinMode(ALERT_LED_PIN, OUTPUT); 
  pinMode(STATUS_LED_PIN, OUTPUT); 
  pinMode(WORKING_LED_PIN, OUTPUT); 
  pinMode(WATER_ENGINE_PIN, OUTPUT); 
  pinMode(BUTTON_PIN, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT); 
  pinMode(ECHO_PIN, INPUT);  
  
  digitalWrite(TRIGGER_PIN, LOW);
  digitalWrite(WATER_ENGINE_PIN, HIGH);

  dht.begin();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);
  display.clearDisplay();
}

int workingMode = 0;

unsigned long currentMillis = 0;
unsigned long previousEngineIntervalMillis = 0;

void loop() {
 
  currentMillis = millis();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  
  workingLed(STATUS_LED_PIN, 3);
  int button = checkButton();  
  float temp = checkTemp();
  float humidity = checkHumidity();
  float realTemp = checkRealTemp(temp, humidity);
  int waterDistance = checkWaterDistance();
  int soilHumidity = checkSoil();
  int soilHumidityGoal = checkSoilHumidityGoal();

  if (button) workingMode = !workingMode;
  
  display.clearDisplay();
  display.print("Mode : ");
  display.println(workingMode); 
  display.display();

  display.print("Temp : ");
  display.println(realTemp); 
  display.display();
  
  display.print("Humd : ");
  display.println(humidity); 
  display.display();

  display.print("Water: ");
  display.println(waterDistance);     
  display.display();

  display.print("Soil : ");
  display.print(soilHumidity);
  display.print(" --> ");
  display.println(soilHumidityGoal);
  display.display();
  
  if (workingMode) {
    if (waterDistance >= MAX_WATER_DISTANCE_CM) {
      digitalWrite(WATER_ENGINE_PIN, HIGH);
      digitalWrite(ALERT_LED_PIN, HIGH);
    } else {
      digitalWrite(ALERT_LED_PIN, LOW);
      engineCheckAndWork(soilHumidity, soilHumidityGoal);
    }
  } else {
      workingLed(ALERT_LED_PIN, 2);
  }

  delay(WORKING_INTERVAL_MS);
}

void engineCheckAndWork(int soilHumidity, int soilHumidityGoal) {
  unsigned long delta = printEngineBar();

  if (delta >= WORKING_ENGINE_INTERVAL_MS) {
    if (soilHumidity < soilHumidityGoal) {
        digitalWrite(WATER_ENGINE_PIN, LOW);
        workingLed(WORKING_LED_PIN, 30    );
        digitalWrite(WATER_ENGINE_PIN, HIGH);
        previousEngineIntervalMillis = millis();
    }
  }
}
