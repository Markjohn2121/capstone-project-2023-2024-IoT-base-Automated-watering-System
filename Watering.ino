#include <Arduino.h>



// Make this 1-2 Sec delay
void StartWaterting() {

  Plant1CriticalPoint();
  Plant2CriticalPoint();
}




void Plant1CriticalPoint() {

  if (P1MoistureVal <= Plant1_CriticalPoint) {
    // digitalWrite(LED_INTERNET,LOW);
    pauseFB();



    // If PLANT 1 IS IN CRITICAL POINT
    if (digitalRead(Start_position) == 0) {  // IF CURRENT POSITION IS IN START
      move_to_plant1("fwrd");

    } else if (digitalRead(Plant_2_Position) == 0) {  //IF CURRENT POSITION IS IN Position 2
      move_to_plant1("rev");

    } else if (digitalRead(Plant_1_Position) == 0) {


    } else {

      move_to_start();
      move_to_plant1("fwrd");
    }



    resumeFB();
    Start_Pumping(1);
    resumeFB();

    if (isInternet) {


      State(1, "PAUSED");
      // ConsumedWater();
    }
    watering_counter(1, 1);
    resumeFB();
  }
}


void Plant2CriticalPoint() {



  if (P2MoistureVal <= Plant2_CriticalPoint) {  // If PLANT 2 IS IN CRITICAL POINT
                                                //sendSensorToDatabase();
    pauseFB();
    // digitalWrite(LED_INTERNET,LOW);
    if (digitalRead(Start_position) == 0) {  // IF CURRENT POSITION IS IN START
      move_to_plant2("fwrd");

    } else if (digitalRead(Plant_1_Position) == 0) {  //IF CURRENT POSITION IS IN Position1

      move_to_plant2("fwrd");

    } else if (digitalRead(Plant_2_Position) == 0) {


    } else {

      move_to_start();
      move_to_plant2("fwrd");
    }





    resumeFB();
    Start_Pumping(2);
    resumeFB();


    if (isInternet) {


      State(2, "PAUSED");
      // ConsumedWater();
    }
    watering_counter(2, 1);
    resumeFB();
  }
}








// Manual Mode
void P1_Manual(String state) {

  if (state == "start") {
    pauseFB();

    if (digitalRead(Start_position) == 0) {  // IF CURRENT POSITION IS IN START
      // delay(500);
      move_to_plant1("fwrd");
    } else if (digitalRead(Plant_2_Position) == 0) {  //IF CURRENT POSITION IS IN Position 2
      // delay(500);
      move_to_plant1("rev");
    } else {
      // delay(500);
      move_to_start();
      // delay(500);
      move_to_plant1("fwrd");
    }
    // delay(500);
    digitalWrite(PUMP_RELAY, LOW);  // ENABLE PUMP

    resumeFB();
  } else if (state == "stop") {
    // fbdo.pauseFirebase(true);
    // fbdo.clear();
    // RxData.pauseFirebase(true);
    // RxData.clear();
    // fboStream.pauseFirebase(true);
    // fboStream.clear();
    pauseFB();
    digitalWrite(PUMP_RELAY, HIGH);  // DISABLE PUMP
    // delay(500);
    move_to_start();
    resumeFB();
    // fbdo.pauseFirebase(false);
    // fbdo.clear();
    // RxData.pauseFirebase(false);
    // RxData.clear();
    // fboStream.pauseFirebase(false);
    // Firebase.RTDB.setMultiPathStreamCallback(&fboStream, streamCallbackPlant_1, streamTimeoutCallback);
  }
}



void P2_Manual(String state) {

  if (state == "start") {

    pauseFB();

    if (digitalRead(Start_position) == 0) {  // IF CURRENT POSITION IS IN START
      // delay(500);
      move_to_plant2("fwrd");
    } else if (digitalRead(Plant_1_Position) == 0) {  //IF CURRENT POSITION IS IN Position 2
      // delay(500);
      move_to_plant2("fwrd");
    } else {
      // delay(500);
      move_to_start();
      // delay(500);
      move_to_plant2("fwrd");
    }
    // delay(500);
    digitalWrite(PUMP_RELAY, LOW);  // ENABLE PUMP

    resumeFB();
  } else if (state == "stop") {
    pauseFB();
    digitalWrite(PUMP_RELAY, HIGH);  // DISABLE PUMP
    // ydelay(500);
    move_to_start();
    // fbdo.pauseFirebase(false);
    resumeFB();
  }
}
