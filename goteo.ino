// Incluimos librería
#include <DHT.h>
 
#define DHTPIN 12
#define DHTTYPE DHT11
 
DHT dht(DHTPIN, DHTTYPE);

const int Trigger = 2; // Orange
const int Echo = 3;   // Green

const int AirValue = 581;   
const int WaterValue = 293;

const int maxWaterDistanceCm = 4;

const int AlertLedPin = 5;  // Orange
const int StatusLedPin = 6; // Yellow
const int WorkingLedPin = 7;  // Red
const int WaterEnginePin = 4; // Blue

const int ButtonPin = 13;

const long WorkIntervalMs = 30000;

void setup() {
  Serial.begin(9600);

  pinMode(AlertLedPin, OUTPUT); 
  pinMode(StatusLedPin, OUTPUT); 
  pinMode(WorkingLedPin, OUTPUT); 
  pinMode(WaterEnginePin, OUTPUT); 

  pinMode(ButtonPin, INPUT);


  pinMode(Trigger, OUTPUT); 
  pinMode(Echo, INPUT);  
  digitalWrite(Trigger, LOW);
  digitalWrite(WaterEnginePin, HIGH);

  dht.begin();
}

int workingMode = 0;

void loop() {
  workingLed(StatusLedPin, 5);
  int button = digitalRead(ButtonPin);
  Serial.print("Boton: ");
  Serial.print(button);
  Serial.print(" - WorkingMode: ");
  Serial.print(workingMode);
  Serial.println();

  if (button) workingMode = !workingMode;
  float temp = checkTemp();
  float humidity = checkHumidity();
  float realTemp = checkRealTemp(temp, humidity);

  if (workingMode) {
    int waterDistance = checkWaterDistance();
    int soilHumidity = checkSoil();
    
    if (waterDistance >= maxWaterDistanceCm) {
      digitalWrite(WaterEnginePin, HIGH);
      digitalWrite(AlertLedPin, HIGH);
    } else {
      digitalWrite(AlertLedPin, LOW);
      int soilHumidityGoal = checkSoilHumidityGoal();

      if (soilHumidity < soilHumidityGoal) {
        digitalWrite(WaterEnginePin, LOW);
        workingLed(WorkingLedPin, 5);
        digitalWrite(WaterEnginePin, HIGH);
      } else {
        digitalWrite(WorkingLedPin, LOW);
        digitalWrite(WaterEnginePin, HIGH);
      }
    }
  } else {
      workingLed(AlertLedPin, 5);
  }

  
  delay(WorkIntervalMs);
}

int checkSoilHumidityGoal() {  
  int val = analogRead(A1);
  int soilHumidityGoal = map(val, 0, 1023, 0, 100);

  Serial.print("Agua Tierra Objetivo: ");
  Serial.print(soilHumidityGoal);
  Serial.print(" %");
  Serial.println();

  return soilHumidityGoal;
}

void workingLed(int led, int times) {
  for(int i=0; i < times; i++) {
    digitalWrite(led, HIGH);
    delay(500);
    digitalWrite(led, LOW);
    delay(500);
  }
}

int checkSoil() {
  int soilMoistureValue = 0;
  int soilmoisturepercent = 0;
  soilMoistureValue = analogRead(A0);
  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
  if(soilmoisturepercent > 100)
  {
    soilmoisturepercent = 100;
  }
  else if(soilmoisturepercent <0)
  {
    soilmoisturepercent = 0;
  }

  Serial.print("Agua Tierra: ");
  Serial.print(soilmoisturepercent);
  Serial.println("%");

  return soilmoisturepercent;
}

int checkWaterDistance() {
  long t;
  long d;
  
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);      
  digitalWrite(Trigger, LOW);

  t = pulseIn(Echo, HIGH); 
  d = t/59;                
  
  Serial.print("Distancia: ");
  Serial.print(d);
  Serial.print("cm");
  Serial.println();

  return d;
}

float checkRealTemp(float temp, float humidity) {
  float hic = dht.computeHeatIndex(temp, humidity, false);
  Serial.print("Sensacion termica: ");
  Serial.print(hic);
  Serial.print(" ºC ");
  Serial.println();

  return hic;
}

float checkHumidity() {
  float h = dht.readHumidity();
 
  if (isnan(h)) {
    Serial.println("Error obteniendo los datos del sensor DHT11: Humidity");
    return 0.00;
  }
 
  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.print(" % ");
  Serial.println();

  return h;
}


float checkTemp() {
  float t = dht.readTemperature();
 
  // Comprobamos si ha habido algún error en la lectura
  if (isnan(t)) {
    Serial.println("Error obteniendo los datos del sensor DHT11: Temperature");
    return 0.00;
  }
    
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print(" ºC ");
  Serial.println();

  return t;
}
