

#include <Arduino.h>
float temperature;

// Read Ultrasonic Sensor
int readWaterLevel() {
  // unsigned int distance = sonar.ping_cm();

  // Get distance in cm
  int distance = sensor.getCM(temperature);
  int waterLevelPercent = map(distance, 0, CONTAINER_HEIGHT, 0, 100);
  waterLevelPercent = constrain(waterLevelPercent, 0, 100);  // Make sure the percentage is within 0-100 range

  waterLevelPercent = 100 - waterLevelPercent;

  return waterLevelPercent;
}


// Read Temperature
// float readTemperature() {

//   temperature = dht.readTemperature();


//   // Check if any reads failed and exit early (to try again).
//   if (isnan(temperature)) {
//     Serial.println("Failed to read Temperature from DHT sensor!");
//     return 0.00;
//   }

//   return temperature;
// }

// // Read Humidity
// float readHumidity() {
//   float humidity = dht.readHumidity();

//   // Check if any reads failed and exit early (to try again).
//   if (isnan(humidity)) {
//     Serial.println("Failed to read Humidity from DHT sensor!");
//     return 0.00;
//   }


//   return humidity;
// }

// READ MOISTURE 1
int readMoisture_sensor_1() {




  int16_t adc0;
  int volt;


  // wet lower value ,Dry higher value

  // max is wet

  adc0 = ads.readADC_SingleEnded(1);
  volt = (ads.computeVolts(adc0) * 1000);
  // adc0 = analogReadMilliVolts(MOISTURE_SENSOR_PIN_1);


  //  int Plant1_moistureValue = analogRead(MOISTURE_SENSOR_PIN_1);
  int Plant1_moisturePercentage = map(volt, moisture_1_Max, moisture_1_Min, 0, 100);
  Plant1_moisturePercentage = 100 - Plant1_moisturePercentage;
  Plant1_moisturePercentage = constrain(Plant1_moisturePercentage, 0, 100);





  return Plant1_moisturePercentage;
}

// READ MOISTURE 2
int readMoisture_sensor_2() {


  int16_t adc1;
  int volt;


  adc1 = ads.readADC_SingleEnded(2);
  volt = (ads.computeVolts(adc1) * 1000);
  //  volt =  analogRead(MOISTURE_SENSOR_PIN_2) ;
  // adc1 = analogRead(MOISTURE_SENSOR_PIN_2);
  //  int Plant2_moistureValue = analogRead(MOISTURE_SENSOR_PIN_2);
  int Plant2_moisturePercentage = map(volt, moisture_2_Max, moisture_2_Min, 0, 100);
  Plant2_moisturePercentage = 100 - Plant2_moisturePercentage;
  Plant2_moisturePercentage = constrain(Plant2_moisturePercentage, 0, 100);

  Serial.println(volt);
  return Plant2_moisturePercentage;
}



int m1() {

  int m1 = 0;
  m1 = readMoisture_sensor_1();
  delay(500);
  m1 += readMoisture_sensor_1();
  delay(500);
  m1 += readMoisture_sensor_1();
  //  delay(500);
  // m1 += readMoisture_sensor_1();

  m1 = (m1 / 3);

  m1 = ((m1 + Add1) - Sub1);

  return m1;
}


int m2() {

  int m2 = 0;
  m2 = readMoisture_sensor_2();
  delay(500);
  m2 += readMoisture_sensor_2();
  delay(500);
  m2 += readMoisture_sensor_2();
  //   delay(500);
  // m2 += readMoisture_sensor_2();

  m2 = (m2 / 3);

  m2 = ((m2 + Add2) - Sub2);

  return m2;
}




// Calibration formula

// Sensor One
int actualValueOne(int x) {
  float y = 0;
  y = S1_CoeficientA * x * x + S1_CoeficientB * x + S1_CoeficientC;
  return round(y);
}

// Sensor One
int actualValueTwo(int x) {
  float y = 0;
  y = S2_CoeficientA * x * x + S2_CoeficientB * x + S2_CoeficientC;
  return round(y);
}
