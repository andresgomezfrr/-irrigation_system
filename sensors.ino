int checkSoil() {
  int soilMoistureValue = 0;
  int soilmoisturepercent = 0;
  soilMoistureValue = analogRead(A0);
  soilmoisturepercent = map(soilMoistureValue, AIR_VALUE_REFERENCE, WATER_VALUE_REFERENCE, 0, 100);
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
  
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);      
  digitalWrite(TRIGGER_PIN, LOW);

  t = pulseIn(ECHO_PIN, HIGH); 
  d = t/59;                
  
  Serial.print("Distancia: ");
  Serial.print(d);
  Serial.print("cm");
  Serial.println();

  return d;
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
