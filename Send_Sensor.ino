#include <Arduino.h>
FirebaseJson json;

int RxDataErrorCount = 0;

void sendSensorToDatabase() {

  // (millis() - sendDataPrevMillis) >= 3000
  if (StartPantingExcuted) {


    P1MoistureVal = 0;
    P2MoistureVal = 0;
    for (int i = 1; i <= 2; i++) {



      digitalWrite(MOISTURE_SENSOR_TRIG_1, HIGH);
      delay(100);
      P1MoistureVal += m1();
      digitalWrite(MOISTURE_SENSOR_TRIG_1, LOW);
      delay(100);


      digitalWrite(MOISTURE_SENSOR_TRIG_2, HIGH);
      delay(100);
      P2MoistureVal += m2();
      digitalWrite(MOISTURE_SENSOR_TRIG_2, LOW);
    }

    P1MoistureVal = (P1MoistureVal / 2);
    P2MoistureVal = (P2MoistureVal / 2);

    Serial.println('Sensor Reading ' + P1MoistureVal + '  '  + P2MoistureVal);
 
    P1MoistureVal = actualValueOne(P1MoistureVal);
    P2MoistureVal = actualValueTwo(P2MoistureVal);



    

    waterLevel = readWaterLevel();




    if (isInternet && initialInternetStat) {

      // if (!Firebase.ready()) {
      //   Firebase.refreshToken(&config);
      // }
      fbdo.clear();
      // RxData.clear();



      // json.set("/Monitor/SoilMoisture/Plant-1", "60");

      // json.set("/Monitor/SoilMoisture/Plant-2", "80");


      // json.set("/Monitor/TempHumi/Temperature", "23");
      // json.set("/Monitor/TempHumi/Humidity", "56");

      // json.set("/Monitor/Water/Level", "50");


      json.set("/Monitor/SoilMoisture/Plant-1", P1MoistureVal);

      json.set("/Monitor/SoilMoisture/Plant-2", P2MoistureVal);


      json.set("/Monitor/TempHumi/Temperature", 0.00);
      json.set("/Monitor/TempHumi/Humidity", 0.00);

      json.set("/Monitor/Water/Level", waterLevel);



      // json.set("/Monitor/Calibration/Sensor-1",String(analogRead(MOISTURE_SENSOR_PIN_1)));
      // json.set("/Monitor/Calibration/Sensor-2",String(analogRead(MOISTURE_SENSOR_PIN_2)));


      // The response is ignored in this  function, it may return true as long as the connection is established.
      // The purpose for this  function is to set, push and update data instantly.

      // ledcWriteTone(LEDC_CHANNEL, 4500);

      // ledcWriteTone(LEDC_CHANNEL, 4500);
      //   ledcWrite(LEDC_CHANNEL, 255);
      if (Firebase.RTDB.updateNodeSilentAsync(&RxData, uidMonitor, &json)) {

        Serial.println("PASSED MONITOR");
        playMarioSound(COIN);
        RxDataErrorCount = 0;
        // json.clear();
      } else {
        playMarioSound(ERROR);

        if (RxDataErrorCount == 10) {
          wm.disconnect();
          delay(500);
          playMarioSound(POWER_DOWN);
          ESP.restart();

        } else {
          Serial.println("FAILED MONITOR");
          Serial.println("REASON: " + RxData.errorReason() + " code " + RxData.errorCode());

          RxData.pauseFirebase(true);
        }


        RxDataErrorCount++;
        pauseFB();
        resumeFB();
      }






      fbdo.clear();

    } else {

      playMarioSound(COIN);
    }



  } else {



    // Serial.println("sa JSON " + fbdo.errorReason());
  }
}



unsigned long prevcalibration = 0;


void Calibrate() {
  fbdo.clear();

  if (isCalibratingStart == "true") {

    if (isInternet && initialInternetStat) {

      if (!Firebase.ready()) {
        Firebase.refreshToken(&config);
      }







      digitalWrite(MOISTURE_SENSOR_TRIG_1, HIGH);
      delay(100);
      int16_t s1 = ads.readADC_SingleEnded(1);
      int volt1 = (ads.computeVolts(s1) * 1000);
      digitalWrite(MOISTURE_SENSOR_TRIG_1, LOW);
      delay(100);


      digitalWrite(MOISTURE_SENSOR_TRIG_2, HIGH);
      delay(100);
      int16_t s2 = ads.readADC_SingleEnded(2);
      int volt2 = (ads.computeVolts(s2) * 1000);
      digitalWrite(MOISTURE_SENSOR_TRIG_2, LOW);

      // int s1 = analogReadMilliVolts(MOISTURE_SENSOR_PIN_1);
      //  int s2 = analogReadMilliVolts(MOISTURE_SENSOR_PIN_2);
      if (Firebase.RTDB.setString(&fbdo, uidMonitor + "/Monitor/Calibration/Sensor-1", String(volt1))) {
        Serial.println("PASSED");
        playMarioSound(COIN);
        //  Serial.println("PATH: " + fbdo.dataPath());
        //  Serial.println("TYPE: " + fbdo.dataType());
      } else {

        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }



      //  ============= get ====

      Serial.printf("Get string... %s\n", Firebase.RTDB.getString(&fbdo, uidMonitor + "/Monitor/Calibration/Sensor-1") ? fbdo.to<const char *>() : fbdo.errorReason().c_str());


      if (Firebase.RTDB.setString(&fbdo, uidMonitor + "/Monitor/Calibration/Sensor-2", String(volt2))) {
        Serial.println("PASSED");
        playMarioSound(COIN);
        //  Serial.println("PATH: " + fbdo.dataPath());
        //  Serial.println("TYPE: " + fbdo.dataType());
      } else {

        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }


      //  ============= get ====
      Serial.printf("Get string... %s\n", Firebase.RTDB.getString(&fbdo, uidMonitor + "/Monitor/Calibration/Sensor-2") ? fbdo.to<const char *>() : fbdo.errorReason().c_str());
    }
  }
}
