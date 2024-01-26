#include <Arduino.h>
// ======= PLANT-1 STREAM =======

int fboStreamCount = 0;
void streamCallbackPlant_1(MultiPathStream stream) {






  size_t numChild = sizeof(NodeChildren) / sizeof(NodeChildren[0]);

  for (size_t i = 0; i < numChild; i++) {
    // Serial.println(stream.dataPath);
    if (stream.get(NodeChildren[i])) {
      //      get the data ==
      //      Serial.printf("path: %s, event: %s, type: %s, value: %s%s", stream.dataPath.c_str(), stream.eventType.c_str(), stream.type.c_str(), stream.value.c_str(), i < numChild - 1 ? "\n" : "");

      // Serial.println(stream.dataPath);

      // Plant-1 ====
      if (stream.dataPath == "/Plant-1/Plant-1/PWP") {
        Plant1_PWP = atof(stream.value.c_str());
      }
      if (stream.dataPath == "/Plant-1/Plant-1/FCapacity") {
        Plant1_FCapacity = atof(stream.value.c_str());
      }
      if (stream.dataPath == "/Plant-1/Plant-1/MAD") {
        Plant1_MAD = atof(stream.value.c_str());
      }
      if (stream.dataPath == "/Plant-1/Plant-1/CriticalPoint") {
        Plant1_CriticalPoint = stream.value.toInt();
      }
      if (stream.dataPath == "/Plant-1/Status/Counter") {
        Plant1_watering_counter = stream.value.toInt();
      }



      // Plant-2 ====

      if (stream.dataPath == "/Plant-2/Plant-2/PWP") {
        Plant2_PWP = atof(stream.value.c_str());
      }

      if (stream.dataPath == "/Plant-2/Plant-2/FCapacity") {
        Plant2_FCapacity = atof(stream.value.c_str());
      }

      if (stream.dataPath == "/Plant-2/Plant-2/MAD") {
        Plant2_MAD = atof(stream.value.c_str());
      }

      if (stream.dataPath == "/Plant-2/Plant-2/CriticalPoint") {
        Plant2_CriticalPoint = stream.value.toInt();
      }
      if (stream.dataPath == "/Plant-2/Status/Counter") {
        Plant2_watering_counter = stream.value.toInt();
      }


      // Calibration
      if (stream.dataPath == "/Calibration/SoilMoisture-1/Dry") {
        moisture_1_Min = stream.value.toInt();
      }
      if (stream.dataPath == "/Calibration/SoilMoisture-1/Wet") {
        moisture_1_Max = stream.value.toInt();
      }
      if (stream.dataPath == "/Calibration/SoilMoisture-2/Dry") {
        moisture_2_Min = stream.value.toInt();
      }
      if (stream.dataPath == "/Calibration/SoilMoisture-2/Wet") {
        moisture_2_Max = stream.value.toInt();
      }
      if (stream.dataPath == "/Calibration/Status/IsCalibrating") {
        isCalibrating = stream.value.c_str();
      }

      if (stream.dataPath == "/Calibration/SoilMoisture-1/Add") {
        Add1 = stream.value.toInt();
      }

      if (stream.dataPath == "/Calibration/SoilMoisture-1/Sub") {
        Sub1 = stream.value.toInt();
      }

      if (stream.dataPath == "/Calibration/SoilMoisture-2/Add") {
        Add2 = stream.value.toInt();
      }

      if (stream.dataPath == "/Calibration/SoilMoisture-2/Sub") {
        Sub2 = stream.value.toInt();
      }



      if (stream.dataPath == "/Calibration/SoilMoisture-1/CoeficientA") {
        S1_CoeficientA = atof(stream.value.c_str());
      }
       if (stream.dataPath == "/Calibration/SoilMoisture-1/CoeficientB") {
        S1_CoeficientB = atof(stream.value.c_str());
      }
       if (stream.dataPath == "/Calibration/SoilMoisture-1/CoeficientC") {
        S1_CoeficientC = atof(stream.value.c_str());
      }


       if (stream.dataPath == "/Calibration/SoilMoisture-2/CoeficientA") {
        S2_CoeficientA = atof(stream.value.c_str());
      }
       if (stream.dataPath == "/Calibration/SoilMoisture-2/CoeficientB") {
        S2_CoeficientB = atof(stream.value.c_str());
      }
       if (stream.dataPath == "/Calibration/SoilMoisture-2/CoeficientC") {
        S2_CoeficientC = atof(stream.value.c_str());
      }










      // Water
      if (stream.dataPath == "/Water/Water/Height") {
        CONTAINER_HEIGHT = stream.value.toInt();
      }
      if (stream.dataPath == "/Water/Water/Type") {
        type = stream.value.c_str();
      }
      if (stream.dataPath == "/Water/Liters/Consume") {
        WaterConsume = atof(stream.value.c_str());
      }



      // Controls

      if (stream.dataPath == "/Controls/Controls/StartPlanting") {
        StartPlanting = stream.value.c_str();
      }

      if (stream.dataPath == "/Controls/Controls/StartPlant1") {
        StartPlant1 = stream.value.c_str();
      }

      if (stream.dataPath == "/Controls/Controls/StartPlant2") {
        StartPlant2 = stream.value.c_str();
      }

      if (stream.dataPath == "/Controls/Controls/DateStarted") {
        DateStarted = stream.value.c_str();
      }

      if (stream.dataPath == "/Controls/Controls/Reset") {
        Reset = stream.value.c_str();
      }



Serial.println(stream.dataPath);

    }else{

      Serial.println("NOT IN NODE");
    }
  }



Serial.println("PATH ");
Serial.println(stream.dataPath);
Serial.println();

  // RESET THE ESP 32 SAVED FILES/WIFI
  if (Reset == "true") {

    resetHistory();
  }

  Serial.println(isCalibrating);
   

  // This is the size of stream payload received (current and max value)
  // Max payload size is the payload size under the stream path since the stream connected
  // and read once and will not update until stream reconnection takes place.
  // This max value will be zero as no payload received in case of ESP8266 which
  // BearSSL reserved Rx buffer size is less than the actual stream payload.
  Serial.printf("Received stream payload size: %d (Max. %d)\n\n", stream.payloadLength(), stream.maxPayloadLength());

  Serial.println(Plant1_PWP);
  Serial.println(Plant1_FCapacity);
  Serial.println(Plant1_MAD);
  Serial.println(Plant1_CriticalPoint);





  Serial.println("  ");

  Serial.println(Plant2_PWP);
  Serial.println(Plant2_FCapacity);
  Serial.println(Plant2_MAD);
  Serial.println(Plant2_CriticalPoint);


  Serial.println("ADD SUB  ");

  Serial.println(Add1);
  Serial.println(Sub1);
  Serial.println(Add2);
  Serial.println(Sub2);




  Serial.println("  ");

  Serial.println(moisture_1_Min);
  Serial.println(moisture_1_Max);
  Serial.println(moisture_2_Min);
  Serial.println(moisture_2_Max);
  Serial.println(isCalibrating);
  Serial.println(DateStarted);



  Serial.println("  ");
  Serial.println(CONTAINER_HEIGHT);
  Serial.println(type);
  Serial.println(WaterConsume);

  Serial.println("  ");
  Serial.println(StartPlanting);
  Serial.println(Plant2_watering_counter);
  currentStartPlantingStatus = StartPlanting;
  currentisCalibratingStatus = isCalibrating;
  currentisPlant1Start = StartPlant1;
  currentisPlant2Start = StartPlant2;


Serial.println(" COEFICIENT == ");

 Serial.println(S1_CoeficientA);
 Serial.println(S1_CoeficientB);
 Serial.println(S1_CoeficientC);

 Serial.println(S2_CoeficientA);
 Serial.println(S2_CoeficientB);
 Serial.println(S2_CoeficientC);




  writeFile(SPIFFS,"/" + ext + "/" + "Plant.csv", generateCSVString(Plant1_FCapacity, Plant1_PWP, Plant1_MAD, Plant1_CriticalPoint, Plant1_watering_counter, moisture_1_Min, moisture_1_Max, Plant2_FCapacity, Plant2_PWP, Plant2_MAD, Plant2_CriticalPoint, Plant2_watering_counter, moisture_2_Min, moisture_2_Max,S1_CoeficientA,S1_CoeficientB,S1_CoeficientC,S2_CoeficientA,S2_CoeficientB,S2_CoeficientC));
  writeFile(SPIFFS,"/" + ext + "/" +  "_data.csv", StartPlanting + " , \n" + CONTAINER_HEIGHT + " ,\n" + String(Add1) + " ,\n" + String(Sub1) + " ,\n" + String(Add2) + " ,\n" + String(Sub2) + " ,\n");


  playMarioSound(JUMP);



  //  Firebase.setMultiPathStreamCallback(Plant_1, streamCallbackPlant_1, streamTimeoutCallback);

  // internet LED status
  // digitalWrite(LED_INTERNET,LOW);MY INTERNET NA


  fbdo.clear();  // close session and release memory
  RxData.clear();
  // isPlantingStarted();

  fboStreamCount = 0;
  //  RxData.clear();
}




// ======== STREAM CALLBACK ======
void streamTimeoutCallback(bool timeout) {



  if (fboStreamCount == 5) {
    wm.disconnect();
    delay(500);
    playMarioSound(POWER_DOWN);
    ESP.restart();
  } else {
    if (timeout) {
      Serial.println("stream timed out, resuming NAAA...\n");
      Serial.println(fboStream.errorReason().c_str());
      Serial.println(fboStream.errorCode());
      PauseStream();
      resumeFB();
      fboStreamCount++;
      // internet LED status
      // digitalWrite(LED_INTERNET,LOW);
    }

    if (!fboStream.httpConnected()) {
      Serial.printf("error code: %d, reason: %s\n\n", fboStream.httpCode(), fboStream.errorReason().c_str());
      // internet LED status
      // digitalWrite(LED_INTERNET,LOW);
      fboStreamCount++;
      Serial.println("No Internet Connection ");
      // internet LED status
      // digitalWrite(LED_INTERNET,LOW);
    }
  }
}
