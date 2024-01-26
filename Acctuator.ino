#include <Arduino.h>
unsigned long prevAcctuatorSoundMillisOFF = 0;
unsigned long prevAcctuatorSoundMillisON = 0;
void move_to_plant1(String direction) {  // MOVE TO PLANT 1 POSITION
  digitalWrite(ACCTUATOR_TRIG, HIGH);

  if (direction == "rev") {
    prevAcctuatorSoundMillisOFF = 0;
    prevAcctuatorSoundMillisON = 0;
    while (digitalRead(Plant_1_Position) == 1) {  // IF ACCTUATOR PLANT1 IS NOT IN POSITION YET
                                                  // Move forward ===

      acctuatorSound();
      if (digitalRead(Plant_1_Position) == 0) {
        digitalWrite(ACCTUATOR_FORWARD, HIGH);
        digitalWrite(ACCTUATOR_BACKWARD, HIGH);
      } else {
        digitalWrite(ACCTUATOR_FORWARD, LOW);
        digitalWrite(ACCTUATOR_BACKWARD, HIGH);
      }


      if (isInternet) {

        Firebase.ready();
      }



      if (digitalRead(Plant_1_Position) == 0) {
        digitalWrite(ACCTUATOR_FORWARD, HIGH);
        digitalWrite(ACCTUATOR_BACKWARD, HIGH);
      }

      // sendSensorToDatabase();
      Serial.println("Move to Plant 1 REV");
    }
  } else if (direction == "fwrd") {

    prevAcctuatorSoundMillisOFF = 0;
    prevAcctuatorSoundMillisON = 0;
    while (digitalRead(Plant_1_Position) == 1) {  // IF ACCTUATOR PLANT1 IS NOT IN POSITION YET
      // Move forward ===
      acctuatorSound();

      if (digitalRead(Plant_1_Position) == 0) {
        digitalWrite(ACCTUATOR_FORWARD, HIGH);
        digitalWrite(ACCTUATOR_BACKWARD, HIGH);
      } else {
        digitalWrite(ACCTUATOR_FORWARD, HIGH);
        digitalWrite(ACCTUATOR_BACKWARD, LOW);
      }



      if (isInternet) {

        Firebase.ready();
      }


      if (digitalRead(Plant_1_Position) == 0) {
        digitalWrite(ACCTUATOR_FORWARD, HIGH);
        digitalWrite(ACCTUATOR_BACKWARD, HIGH);
      }

      // sendSensorToDatabase();
      Serial.println("Move to Plant 1 FWRD");
    }
  }
digitalWrite(ACCTUATOR_FORWARD, LOW);
      digitalWrite(ACCTUATOR_BACKWARD, HIGH);
  digitalWrite(ACCTUATOR_FORWARD, HIGH);
  digitalWrite(ACCTUATOR_BACKWARD, HIGH);
  delay(1000);
  digitalWrite(ACCTUATOR_FORWARD, LOW);
  digitalWrite(ACCTUATOR_BACKWARD, LOW);

  ledcWrite(LEDC_CHANNEL, 0);
  digitalWrite(ACCTUATOR_TRIG, LOW);
}

void move_to_plant2(String direction) {  // MOVE TO PLANT 2 POSITION

  digitalWrite(ACCTUATOR_TRIG, HIGH);
  if (direction == "rev") {
    prevAcctuatorSoundMillisOFF = 0;
    prevAcctuatorSoundMillisON = 0;

    while (digitalRead(Plant_2_Position) == 1) {  // IF ACCTUATOR PLANT1 IS NOT IN POSITION YET


      acctuatorSound();
      if (digitalRead(Plant_2_Position) == 0) {
        digitalWrite(ACCTUATOR_FORWARD, HIGH);
        digitalWrite(ACCTUATOR_BACKWARD, HIGH);
      } else {
        // Move forward ===
        digitalWrite(ACCTUATOR_FORWARD, LOW);
        digitalWrite(ACCTUATOR_BACKWARD, HIGH);
      }


      if (isInternet) {

        Firebase.ready();
      }


      if (digitalRead(Plant_2_Position) == 0) {
        digitalWrite(ACCTUATOR_FORWARD, HIGH);
        digitalWrite(ACCTUATOR_BACKWARD, HIGH);
      }
      // sendSensorToDatabase();
      Serial.println("Move to Plant 2  REV");
    }


  } else if (direction == "fwrd") {
    prevAcctuatorSoundMillisOFF = 0;
    prevAcctuatorSoundMillisON = 0;

    while (digitalRead(Plant_2_Position) == 1) {  // IF ACCTUATOR PLANT1 IS NOT IN POSITION YET

      acctuatorSound();
      if (digitalRead(Plant_2_Position) == 0) {
        digitalWrite(ACCTUATOR_FORWARD, HIGH);
        digitalWrite(ACCTUATOR_BACKWARD, HIGH);
      } else {
        // Move forward ===
        digitalWrite(ACCTUATOR_FORWARD, HIGH);
        digitalWrite(ACCTUATOR_BACKWARD, LOW);
      }
      if (isInternet) {

        Firebase.ready();
      }



      if (digitalRead(Plant_2_Position) == 0) {
        digitalWrite(ACCTUATOR_FORWARD, HIGH);
        digitalWrite(ACCTUATOR_BACKWARD, HIGH);
      }
      // sendSensorToDatabase();
      Serial.println("Move to Plant 2 FWRD");
    }
  }

digitalWrite(ACCTUATOR_FORWARD, LOW);
      digitalWrite(ACCTUATOR_BACKWARD, HIGH);
  digitalWrite(ACCTUATOR_FORWARD, HIGH);
  digitalWrite(ACCTUATOR_BACKWARD, HIGH);
  delay(1000);
  digitalWrite(ACCTUATOR_FORWARD, LOW);
  digitalWrite(ACCTUATOR_BACKWARD, LOW);
  ledcWrite(LEDC_CHANNEL, 0);

  digitalWrite(ACCTUATOR_TRIG, LOW);
}




void move_to_start() {  // MOVE TO STARTING POSITION
  digitalWrite(ACCTUATOR_TRIG, HIGH);
  prevAcctuatorSoundMillisOFF = 0;
  prevAcctuatorSoundMillisON = 0;

  while (digitalRead(Start_position) == 1) {  // IF ACCTUATOR Start  IS NOT IN POSITION YET
    // Move BACKWARD ===
    acctuatorSound();
    if (digitalRead(Start_position) == 0) {
      digitalWrite(ACCTUATOR_FORWARD, HIGH);
      digitalWrite(ACCTUATOR_BACKWARD, HIGH);
    } else {
      digitalWrite(ACCTUATOR_FORWARD, LOW);
      digitalWrite(ACCTUATOR_BACKWARD, HIGH);
    }



    if (isInternet && initialInternetStat) {

      Firebase.ready();
    }

    if (digitalRead(Start_position) == 0) {
      digitalWrite(ACCTUATOR_FORWARD, HIGH);
      digitalWrite(ACCTUATOR_BACKWARD, HIGH);
    }

    // sendSensorToDatabase();
    Serial.println("Move to Start");
  }


digitalWrite(ACCTUATOR_FORWARD, HIGH);
      digitalWrite(ACCTUATOR_BACKWARD, LOW);

  digitalWrite(ACCTUATOR_FORWARD, HIGH);
  digitalWrite(ACCTUATOR_BACKWARD, HIGH);
  delay(1000);
  digitalWrite(ACCTUATOR_FORWARD, LOW);
  digitalWrite(ACCTUATOR_BACKWARD, LOW);
  ledcWrite(LEDC_CHANNEL, 0);
  digitalWrite(ACCTUATOR_TRIG, LOW);
}



void acctuatorSound() {

  if ((millis() - prevAcctuatorSoundMillisON) >= 1000) {

    ledcWriteTone(LEDC_CHANNEL, 4880);
    ledcWrite(LEDC_CHANNEL, 255);
    prevAcctuatorSoundMillisON = millis();
  }
  if ((millis() - prevAcctuatorSoundMillisOFF) >= 2000) {

    ledcWrite(LEDC_CHANNEL, 0);
    prevAcctuatorSoundMillisOFF = millis();
  }
}