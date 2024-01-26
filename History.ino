#include <Arduino.h>


String generateCSVString(float fcapacity1, float pwp1, float mad1, int criticalPoint1, int counter1,
                         int moisture1Min, int moisture1Max,
                         float fcapacity2, float pwp2, float mad2, int criticalPoint2, int counter2,
                         int moisture2Min, int moisture2Max,float S1_CoeficientA,float S1_CoeficientB,float S1_CoeficientC,float S2_CoeficientA,float S2_CoeficientB,float S2_CoeficientC) {
  // Create the CSV string
  String csvString = "";

  // Row 1
  csvString += String(fcapacity1, 2) + ",";
  csvString += String(pwp1, 2) + ",";
  csvString += String(mad1, 2) + ",";
  csvString += String(criticalPoint1) + ",";
  csvString += String(counter1) + ",";
  csvString += String(moisture1Min) + ",";
  csvString += String(moisture1Max) + ",";
  csvString += String(S1_CoeficientA) + ",";
  csvString += String(S1_CoeficientB) + ",";
  csvString += String(S1_CoeficientC) + "\n";
  

  // Row 2
  csvString += String(fcapacity2, 2) + ",";
  csvString += String(pwp2, 2) + ",";
  csvString += String(mad2, 2) + ",";
  csvString += String(criticalPoint2) + ",";
  csvString += String(counter2) + ",";
  csvString += String(moisture2Min) + ",";
  csvString += String(moisture2Max) + ",";
  csvString += String(S2_CoeficientA) + ",";
  csvString += String(S2_CoeficientB) + ",";
  csvString += String(S2_CoeficientC) + "\n";

  return csvString;
}




String readCSVValue(String csvData, int row, int col) {
  String currentLine;
  int currentRow = 1;
  int currentCol = 1;
  int dataIndex = 0;
  char currentChar;

  // Iterate through each character in the CSV data
  while (currentChar = csvData[dataIndex]) {
    // Check for a new line character
    if (currentChar == '\n') {
      // Move to the next row and reset the column
      currentRow++;
      currentCol = 1;
    }
    // Check for a comma, which separates columns
    else if (currentChar == ',') {
      // Move to the next column
      currentCol++;
    }
    // Check if we have reached the desired row and column
    else if (currentRow == row && currentCol == col) {
      // Read the value until the next comma or newline
      while (currentChar = csvData[dataIndex]) {
        if (currentChar == ',' || currentChar == '\n') {
          break;
        }
        currentLine += currentChar;
        dataIndex++;
      }
      // Return the value found
      return currentLine;
    }

    // Move to the next character in the CSV data
    dataIndex++;
  }

  // Return an empty string if the specified row or column is not found
  return "";
}








void resetHistory() {

  playMarioSound(POWER_DOWN);
  PauseStream();
  // rESET THE FILES TO DEFAULT VALUE
  writeFile(SPIFFS,"/" + ext +  "/currentLiter.txt", "0.00");
  writeFile(SPIFFS,"/" + ext +  "/currentLiter2.txt", "0.00");
  writeFile(SPIFFS,"/" + ext +  "/currentDay.txt", "0");
  writeFile(SPIFFS,"/" + ext +  "/currentDate.txt", "11/1/2023");
  writeFile(SPIFFS,"/" + ext +  "/waterConsump1.txt", "day=0 liter=0.00\n");
  writeFile(SPIFFS,"/" + ext +  "/waterConsump2.txt", "day=0 liter=0.00\n");


  writeFile(SPIFFS,"/" + ext +  "/counterP1.txt", "0");
  writeFile(SPIFFS,"/" + ext +  "/counterP2.txt", "0");
  writeFile(SPIFFS,"/" + ext +  "/" +"Plant.csv", generateCSVString(0.00, 0.00, 0.00, 0, 0, 0, 4095, 0.00, 0.00, 0.00, 0, 0, 0, 4095,0.000,0.000,0.000,0.000,0.000,0.000));
  writeFile(SPIFFS,"/" + ext +  "/" +"_data.csv", "false , \n" + String(CONTAINER_HEIGHT) + " ,\n" + String(Add1) + " ,\n" + String(Sub1) + " ,\n" + String(Add2) + " ,\n" + String(Sub2) + " ,\n");

  playMarioSound(POWER_DOWN);




  if (isInternet) {

    if (Firebase.RTDB.setString(&fbdo, uid + "/Data/Water/Liters/Consume", "0.00")) {
      Serial.println("PASSED Liter");
      //  //Serial.println("PATH: " + fbdo.dataPath());
      //  //Serial.println("TYPE: " + fbdo.dataType());
      playMarioSound(FLAGPOLE);
    } else {
      Serial.println("FAILED TO PASS Liters");
      Serial.println("REASON: " + fbdo.errorReason());
      // resolve();
    }


    if (Firebase.RTDB.setString(&fbdo, uid + "/Data/Plant-1/Status/Counter", "0")) {
      Serial.println("PASSED counter");
      //  //Serial.println("PATH: " + fbdo.dataPath());
      //  //Serial.println("TYPE: " + fbdo.dataType());
    } else {
      Serial.println("FAILED TO PASS COUNTER");
      Serial.println("REASON: " + fbdo.errorReason());
      // resolve();
    }


    if (Firebase.RTDB.setString(&fbdo, uid + "/Data/Plant-2/Status/Counter", "0")) {
      Serial.println("PASSED");
      //  //Serial.println("PATH: " + fbdo.dataPath());
      //  //Serial.println("TYPE: " + fbdo.dataType());
    } else {
      Serial.println("FAILED TO PASS COUNTER");
      Serial.println("REASON: " + fbdo.errorReason());
    }



    playMarioSound(POWER_DOWN);

    uploadfileToFirebase(id, "/" +  ext +  "/waterConsump1.txt", "waterConsump1.txt");
    uploadfileToFirebase(id, "/" +  ext +  "/waterConsump2.txt", "waterConsump2.txt");

    if (Firebase.RTDB.setString(&fbdo, uid + "/Data/Controls/Controls/Reset", "false")) {
      Serial.println("PASSED Reset");
      playMarioSound(FLAGPOLE);
    } else {
      Serial.println("FAILED Reset");
      Serial.println("REASON: " + fbdo.errorReason() + " code" + fbdo.errorCode());
    }
  }

  playMarioSound(POWER_DOWN);

  wm.disconnect();
  wm.erase();

  playMarioSound(FLAGPOLE);

  ESP.restart();
}
