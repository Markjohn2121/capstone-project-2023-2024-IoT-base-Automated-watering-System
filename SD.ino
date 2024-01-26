#include <Arduino.h>

void SD_INIT() {



  // if (!SD.begin()) {
  //   Serial.println("Card Mount Failed");
  //   return;
  // }
  // uint8_t cardType = SD.cardType();

  // if (cardType == CARD_NONE) {
  //   Serial.println("No SD card attached");
  //   return;
  // }

  // Serial.print("SD Card Type: ");
  // if (cardType == CARD_MMC) {
  //   Serial.println("MMC");
  // } else if (cardType == CARD_SD) {
  //   Serial.println("SDSC");
  // } else if (cardType == CARD_SDHC) {
  //   Serial.println("SDHC");
  // } else {
  //   Serial.println("UNKNOWN");
  // }

  // uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  // Serial.printf("SD Card Size: %lluMB\n", cardSize);







  // USING SPIFFS -------

  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }


  playMarioSound(SUPER_MARIO);

  Serial.println("Current Data : " + readFile(SPIFFS,"/" + ext + "/currentDate.txt"));
  Serial.println("Current Day : " + readFile(SPIFFS,"/" + ext +  "/currentDay.txt"));
  Serial.println("Current Liter : " + readFile(SPIFFS,"/" + ext + "/currentLiter.txt"));
  Serial.println("Current Liter : " + readFile(SPIFFS,"/" + ext + "/currentLiter2.txt"));
  Serial.println("Plant Data : " + readFile(SPIFFS,"/" + ext + "/" +"Plant.csv"));
  Serial.println("Data : " + readFile(SPIFFS,"/" + ext + "/" +  "_data.csv"));
  listDir(SPIFFS, "/", 0);

  // Serial.println("Water Consume Data : " + readFile(SPIFFS,"/" + ext + "/" + machineID + "/waterConsume.txt"));
}