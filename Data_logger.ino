#include <Arduino.h>

const float PUMP_RATE = 1.0 / 62.0;  // Liters per second (1 liter per 50 seconds)

void logg_Water_consume(int second, int plant) {

  if (plant == 1) {
    liter = 0.00;
    liter = PUMP_RATE * second;  // Compute water usage in liters
    getLocalTime();

    loggedDay = readFile(SPIFFS,"/" + ext +  "/currentDate.txt");  // default is 00/00/0000
    Serial.println(second);
    // Read current values from files
    int currentDay = readFile(SPIFFS,"/" + ext +  "/currentDay.txt").toInt();  // default is 0
    Serial.println(currentDay);
    float currentLiter = readFile(SPIFFS,"/" + ext + "/currentLiter.txt").toFloat();  //default is 0.00
    Serial.println(currentLiter);
    if (loggedDay == full_dateNow) {
      Serial.println("Same Day");
      // Append water consumption data to file
      currentLiter = currentLiter + liter;
      writeFile(SPIFFS,"/" + ext +  "/currentLiter.txt", String(currentLiter));
      appendFile(SPIFFS,"/" +  ext +  "/waterConsump1.txt", "day=" + String(currentDay) + " liter=" + String(currentLiter) + "\n");
    } else {
      Serial.println("Other Day");
      // Update current day and reset current liter
      currentDay++;
      currentLiter = liter;

      // Write current day and liter to files
      writeFile(SPIFFS,"/" + ext + "/currentDay.txt", String(currentDay));
      writeFile(SPIFFS,"/" + ext + "/currentLiter.txt", String(currentLiter));

      // Append water consumption data to file
      appendFile(SPIFFS,"/" +  ext +   "/waterConsump1.txt", "day=" + String(currentDay) + " liter=" + String(currentLiter) + "\n");

      writeFile(SPIFFS,"/" + ext +  "/currentDate.txt", String(full_dateNow));
    }




  } else if (plant == 2) {

    liter = 0.00;
    liter = PUMP_RATE * second;  // Compute water usage in liters
    getLocalTime();

    loggedDay = readFile(SPIFFS,"/" + ext +  "/currentDate.txt");  // default is 00/00/0000
    Serial.println(second);
    // Read current values from files
    int currentDay = readFile(SPIFFS,"/" + ext + "/currentDay.txt").toInt();  // default is 0
    Serial.println(currentDay);
    float currentLiter = readFile(SPIFFS,"/" + ext + "/currentLiter2.txt").toFloat();  //default is 0.00
    Serial.println(currentLiter);
    if (loggedDay == full_dateNow) {
      Serial.println("Same Day");
      // Append water consumption data to file
      currentLiter = currentLiter + liter;
      writeFile(SPIFFS,"/" + ext +  "/currentLiter2.txt", String(currentLiter));
      appendFile(SPIFFS,"/" +  ext +  "/waterConsump2.txt", "day=" + String(currentDay) + " liter=" + String(currentLiter) + "\n");
    } else {
      Serial.println("Other Day");
      // Update current day and reset current liter
      currentDay++;
      currentLiter = liter;

      // Write current day and liter to files
      writeFile(SPIFFS,"/" + ext +  "/currentDay.txt", String(currentDay));
      writeFile(SPIFFS,"/" + ext +  "/currentLiter2.txt", String(currentLiter));

      // Append water consumption data to file
      appendFile(SPIFFS,"/" +  ext +   "/waterConsump2.txt", "day=" + String(currentDay) + " liter=" + String(currentLiter) + "\n");

      writeFile(SPIFFS,"/" + ext +  "/currentDate.txt", String(full_dateNow));
    }
  }
}




void uploadfileToFirebase(String uid, String PATH, String NAME) {


  if (isInternet) {

    if (!Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID, PATH, mem_storage_type_flash, uid + "/" + NAME, "text/plain ", fcsUploadCallback)) {
      Serial.println(fbdo.errorReason());
    } else {
    }
  }
}





float totalWaterConsumep1() {
  // Your data string
  String data = readFile(SPIFFS,"/" + ext + "/waterConsumep1.txt");

  // Variables to store current day and total liters
  int currentDay = -1;
  float totalLiters = 0.0;
  float lastLiter = 0.0;

  // Tokenize the data by lines
  char *line = strtok((char *)data.c_str(), "\n");

  while (line != NULL) {
    // Parse day and liters from the line
    int day, bytesRead;
    float liters;
    bytesRead = sscanf(line, "day=%d liter=%f", &day, &liters);

    // Check if parsing was successful
    if (bytesRead == 2) {
      // Check if it's a new day
      if (day != currentDay) {
        // Add the last liter to the total for the previous day
        totalLiters += lastLiter;

        // Update current day and reset last liter
        currentDay = day;
        lastLiter = 0.0;
      }

      // Update the last liter for the current day
      lastLiter = liters;
    }

    // Move to the next line
    line = strtok(NULL, "\n");
  }

  // Add the last liter to the total for the last day
  totalLiters += lastLiter;

  // Output the total liters
  Serial.print("Total Liters = ");
  Serial.println(totalLiters);
  return totalLiters;
}




float totalWaterConsumep2() {
  // Your data string
  String data = readFile(SPIFFS,"/" + ext +  "/waterConsumep2.txt");

  // Variables to store current day and total liters
  int currentDay = -1;
  float totalLiters = 0.0;
  float lastLiter = 0.0;

  // Tokenize the data by lines
  char *line = strtok((char *)data.c_str(), "\n");

  while (line != NULL) {
    // Parse day and liters from the line
    int day, bytesRead;
    float liters;
    bytesRead = sscanf(line, "day=%d liter=%f", &day, &liters);

    // Check if parsing was successful
    if (bytesRead == 2) {
      // Check if it's a new day
      if (day != currentDay) {
        // Add the last liter to the total for the previous day
        totalLiters += lastLiter;

        // Update current day and reset last liter
        currentDay = day;
        lastLiter = 0.0;
      }

      // Update the last liter for the current day
      lastLiter = liters;
    }

    // Move to the next line
    line = strtok(NULL, "\n");
  }

  // Add the last liter to the total for the last day
  totalLiters += lastLiter;

  // Output the total liters
  Serial.print("Total Liters = ");
  Serial.println(totalLiters);
  return totalLiters;
}









void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\r\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println(" - not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.path(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}





String readFile(fs::FS &fs, String &path) {
  Serial.printf("Reading file: %s\n", path.c_str());
  String readFile = "";
  File file = fs.open(path, "r");
  if (file) {
    while (file.available()) {
      char c = file.read();
      readFile += c;
    }
    file.close();
    // Trim whitespace (Note: trim does not modify the original string)
  } else {
    Serial.println("Failed to open file for reading");
  }
  return readFile;
}

void writeFile(fs::FS &fs, const String &path, const String &message) {
  Serial.printf("Writing file: %s\n", path.c_str());
  File file = fs.open(path, FILE_WRITE);
  if (file) {
    if (file.print(message.c_str())) {
      Serial.println("File written");
    } else {
      Serial.println("Write failed");
    }
    file.close();
  } else {
    Serial.println("Failed to open file for writing");
  }
}

void appendFile(fs::FS &fs, String &path, const String &message) {
  Serial.printf("Appending to file: %s\n", path.c_str());
  File file = fs.open(path, FILE_APPEND);
  if (file) {
    if (file.print(message.c_str())) {
      Serial.println("Message appended");
    } else {
      Serial.println("Append failed");
    }
    file.close();
  } else {
    Serial.println("Failed to open file for appending");
  }
}

void deleteFile(fs::FS &fs, const char *path) {
  Serial.printf("Deleting file: %s\n", path);
  if (fs.remove(path)) {
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}
