



void getLocalTime() {

  // char dayBuffer[3];
  // char amPmBuffer[3];
  // char monthBuffer[3];
  // char hourBuffer[3];
  // char yearBuffer[5];





  // strftime(dayBuffer, sizeof(dayBuffer), "%e", &timeinfo);
  // strftime(amPmBuffer, sizeof(amPmBuffer), "%p", &timeinfo);
  // strftime(monthBuffer, sizeof(monthBuffer), "%m", &timeinfo);
  // strftime(hourBuffer, sizeof(hourBuffer), "%I", &timeinfo);
  // strftime(yearBuffer, sizeof(yearBuffer), "%Y", &timeinfo);



  dayMonth = String(rtc.getDay());
  amPm = String(rtc.getAmPm());
  monthNow = String(rtc.getMonth());
  hourNow = String(rtc.getHour());
  yearNow = String(rtc.getYear());


  full_dateNow = String(monthNow.toInt() + 1) + "/" + dayMonth + "/" + yearNow;


  //  Serial.println( monthNow + " " + dayMonth+ " " + yearNow+ " " + hourNow+ " " + amPm);
}


void setTime(String status) {


  if (status == "offline") {

    // Input string containing date in the format "11/1/2023"
    String dateString = readFile(SPIFFS,"/" + ext + "/currentDate.txt");
    // String dateString = "11/1/2023";

    Serial.println(dateString);
    // Find the positions of slashes in the input string
    int firstSlashPos = dateString.indexOf('/');
    int secondSlashPos = dateString.lastIndexOf('/');

    // Extract month, day, and year as substrings
    String monthString = dateString.substring(0, firstSlashPos);
    String dayString = dateString.substring(firstSlashPos + 1, secondSlashPos);
    String yearString = dateString.substring(secondSlashPos + 1);

    monthString.trim();
    dayString.trim();
    yearString.trim();
    Serial.println(monthString);
    Serial.println(dayString);
    Serial.println(yearString);
    // Convert extracted substrings to integers
    int month = monthString.toInt();
    int day = dayString.toInt();
    int year = yearString.toInt();

    // Set your desired date and time
    rtc.setTime(30, 24, 8, day, month, year);


  } else if (status == "online") {



    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      rtc.setTimeStruct(timeinfo);
      // Firebase.setSystemTime(timeinfo);

    } else {
      Serial.println("Failed to obtain time");
      return;
    }
  }








  // struct tm timeinfo;
  // timeinfo.tm_year = year - 1900; // Year since 1900
  // timeinfo.tm_mon = month;             // Month (0 - 11, January is 0)
  // timeinfo.tm_mday = day;            // Day of the month (1 - 31)
  // timeinfo.tm_hour = 0;            // Hour (0 - 23)
  // timeinfo.tm_min = 0;             // Minute (0 - 59)
  // timeinfo.tm_sec = 0;             // Second (0 - 59)

  // // Set the system time
  // if (mktime(&timeinfo) == -1) {
  //   Serial.println("Failed to set time!");
  // } else {
  //   Serial.println("Time set successfully!");
  // }
}
