void feedBackIsPlanting(bool state) {
  PauseStream();
  RxData.pauseFirebase(true);
  RxData.clear();
  fbdo.clear();



  if (Firebase.RTDB.setBool(&fbdo, uid + "/Data/RxData/Controls/isPlanting", state)) {
    Serial.println("PASSED isPlanting");


    if (state) {
      getLocalTime();
      if (full_dateNow != "") {
        // writeFile(SPIFFS,"/" + ext + "/" + id + "/currentDate.txt", String(full_dateNow));
      }
      StartPantingExcuted = true;
    } else {
      StartPantingExcuted = false;
      // DateStarted = " ";
      uploadfileToFirebase(id, "/" +  ext +  "/waterConsump1.txt", "waterConsump1.txt");
      uploadfileToFirebase(id, "/" +  ext +  "/waterConsump2.txt", "waterConsump2.txt");
    }

    //  //Serial.println("PATH: " + fbdo.dataPath());
    //  //Serial.println("TYPE: " + fbdo.dataType());
  } else {
    Serial.println("FAILED isPlanting");
    Serial.println("REASON: " + fbdo.errorReason() + " code" + fbdo.errorCode());
    fbdo.clear();
    RxData.clear();

    // resolve();
    // StartPantingExcuted = false;
  }



  resumeFB();
}

void feedBackIsCalibrating(bool state) {
  PauseStream();
  RxData.pauseFirebase(true);
  RxData.clear();
  Serial.println("feedBackIsCalibrating");
  if (Firebase.RTDB.setBool(&fbdo, uid + "/Data/RxData/Calibration/Status", state)) {
    Serial.println("PASSED feedBackIsCalibrating");


    if(state){
      isCalibratingStart = "true";
    }else{
      isCalibratingStart = "false";
    }
    //  //Serial.println("PATH: " + fbdo.dataPath());
    //  //Serial.println("TYPE: " + fbdo.dataType());
  } else {
    Serial.println("FAILED feedBackIsCalibrating");
    Serial.println("REASON: " + fbdo.errorReason() + " code" + fbdo.errorCode());
    // resolve();



  }
  resumeFB();
}



int currenrCounterP1;
int currenrCounterP2;


void watering_counter(int Plant, int count) {



  if (Plant == 1) {
    currenrCounterP1 = readFile(SPIFFS,"/" + ext + "/counterP1.txt").toInt();
    writeFile(SPIFFS,"/" + ext +"/counterP1.txt", String((currenrCounterP1 + count)));
  }

  if (Plant == 2) {
    currenrCounterP2 = readFile(SPIFFS,"/" + ext + "/counterP2.txt").toInt();
    writeFile(SPIFFS,"/" + ext + "/counterP2.txt", String((currenrCounterP2 + count)));
  }



  if (isConnectedToInternet()) {
    if (Plant == 1) {

      currenrCounterP1 = readFile(SPIFFS,"/" + ext + "/counterP1.txt").toInt();

      if (Firebase.RTDB.setInt(&fbdo, uid + "/Data/Plant-1/Status/Counter", currenrCounterP1)) {
        Serial.println("PASSED counter");
        //  //Serial.println("PATH: " + fbdo.dataPath());
        //  //Serial.println("TYPE: " + fbdo.dataType());
      } else {
        Serial.println("FAILED TO PASS COUNTER");
        Serial.println("REASON: " + fbdo.errorReason());
        // resolve();
      }


    } else if (Plant == 2) {

      currenrCounterP2 = readFile(SPIFFS,"/" + ext +  "/counterP2.txt").toInt();
      // String Count = String((Plant2_watering_counter + 1));
      if (Firebase.RTDB.setInt(&fbdo, uid + "/Data/Plant-2/Status/Counter", currenrCounterP2)) {
        //Serial.println("PASSED");
        //  //Serial.println("PATH: " + fbdo.dataPath());
        //  //Serial.println("TYPE: " + fbdo.dataType());
      } else {
        //Serial.println("FAILED TO PASS COUNTER");
        //Serial.println("REASON: " + fbdo.errorReason());
      }
    } else {
      if (Firebase.RTDB.setString(&fbdo, uid + "/Data/Plant-2/Status/Counter", "else")) {
        //Serial.println("PASSED");
        //  //Serial.println("PATH: " + fbdo.dataPath());
        //  //Serial.println("TYPE: " + fbdo.dataType());
      } else {
        //Serial.println("FAILED");
        //Serial.println("REASON: " + fbdo.errorReason());
      }
    }
  }
}


void State(int Plant, String state) {
  if (Plant == 1) {
    if (Firebase.RTDB.setString(&fbdo, uid + "/Data/Plant-1/Status/State", state)) {
      Serial.println("PASSED state");
      //  //Serial.println("PATH: " + fbdo.dataPath());
      //  //Serial.println("TYPE: " + fbdo.dataType());
    } else {
      Serial.println("FAILED TO PASS State");
      Serial.println("REASON: " + fbdo.errorReason());

      // resolve();
    }
  } else if (Plant == 2) {
    if (Firebase.RTDB.setString(&fbdo, uid + "/Data/Plant-2/Status/State", state)) {
      Serial.println("PASSED state");
      //  //Serial.println("PATH: " + fbdo.dataPath());
      //  //Serial.println("TYPE: " + fbdo.dataType());
    } else {
      Serial.println("FAILED TO PASS State");
      Serial.println("REASON: " + fbdo.errorReason());
      // resolve();
    }
  }
}

void ConsumedWater() {
  // WaterConsume + Liter;


  if (Firebase.RTDB.setString(&fbdo, uid + "/Data/Water/Liters/Consume", String((totalWaterConsumep1() + totalWaterConsumep2())))) {
    Serial.println("PASSED Liter");
    //  //Serial.println("PATH: " + fbdo.dataPath());
    //  //Serial.println("TYPE: " + fbdo.dataType());
  } else {
    Serial.println("FAILED TO PASS Liters");
    Serial.println("REASON: " + fbdo.errorReason());
    // resolve();
  }
}








String feedbackIsPlant1Started(bool state) {
  PauseStream();
  RxData.pauseFirebase(true);
  Serial.println("feedbackIsPlant1Started");
  if (state) {

    // feedback
    if (Firebase.RTDB.setBool(&fbdo, uid + "/Data/RxData/Controls/isStartPlant1", state)) {
      Serial.println("PASSED p1 Started");
      // Start P1_manual function

      // isPlant1 = true;
      RxData.pauseFirebase(false);
      return "start";
      // P1_Manual("start");
    } else {

      Serial.println("FAILED p1 started");
      Serial.println("REASON: " + fbdo.errorReason());
      RxData.pauseFirebase(false);
      return "stop";
    }

  } else if (!state) {
    if (Firebase.RTDB.setBool(&fbdo, uid + "/Data/RxData/Controls/isStartPlant1", state)) {
      //Serial.println("PASSED");
      // stop P1_manual function
      //  isPlant1 = false;
      // P1_Manual("stop");
      RxData.pauseFirebase(false);
      return "stop";
    } else {
      RxData.pauseFirebase(false);
      return "stop";
      //Serial.println("FAILED");
      //Serial.println("REASON: " + fbdo.errorReason());
    }
  }
}


String feedbackIsPlant2Started(bool state) {
  Serial.println("feedbackIsPlant2Started");
  PauseStream();
  RxData.pauseFirebase(true);
  if (state) {

    // feedback
    if (Firebase.RTDB.setBool(&fbdo, uid + "/Data/RxData/Controls/isStartPlant2", state)) {
      //Serial.println("PASSED");
      // Start P2_manual function
      // isPlant2 = true;
      RxData.pauseFirebase(false);
      return "start";
      // P2_Manual("start");
    } else {
      RxData.pauseFirebase(false);
      return "stop";
      //Serial.println("FAILED");
      //Serial.println("REASON: " + fbdo.errorReason());
    }

  } else if (!state) {
    if (Firebase.RTDB.setBool(&fbdo, uid + "/Data/RxData/Controls/isStartPlant2", state)) {
      //Serial.println("PASSED");
      // stop P2_manual function
      // isPlant2 = false;
      RxData.pauseFirebase(false);
      return "stop";
      // P2_Manual("stop");
    } else {
      RxData.pauseFirebase(false);
      return "stop";
      //Serial.println("FAILED");
      //Serial.println("REASON: " + fbdo.errorReason());
    }
  }
}
