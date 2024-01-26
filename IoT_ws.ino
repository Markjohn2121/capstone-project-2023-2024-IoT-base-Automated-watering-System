#include <ESP32Time.h>







// The wifi manager library and the wm instance is in Firebase addons/Tokenhelper.h

// Libraries ======

#include <Arduino.h>
#include <stdlib.h>
#if defined(ESP32)
#include <WiFi.h>
// #include <WiFiManager.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <Adafruit_Sensor.h>
#include <DHT.h>
// #include <NewPing.h>
#include <DistanceSensor.h>
#include "time.h"


#include <Firebase_ESP_Client.h>


//Provide the token generation process info.
#include "addons/TokenHelper.h"
#include <addons/SDHelper.h>
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"
#include <Adafruit_ADS1X15.h>
// #include <DatabaseOnSD.h>
#include "FS.h"
#include "SPIFFS.h"
// #include "SD.h"
#include "SPI.h"
#include <ESP32Ping.h>



#define FORMAT_SPIFFS_IF_FAILED true

// =============== USER CREDENTIALS ============

String machineID = "iotws2023";
String machinePASSWORD = "iotws2023";
String Device = "Devices";
String Monitor = "Monitor";
String id = "mH5f4kNEvaXgo2AANxPypPPSQXj2";  // "mH5f4kNEvaXgo2AANxPypPPSQXj2";  "7t0p9XjTVvWTyFSVuVqpJ1GsqOj1";
String uid = Device + "/" + id;
String uidMonitor = Monitor + "/" + id;
#define STORAGE_BUCKET_ID "iot-aws-a65d9.appspot.com"
// =============== USER CREDENTIALS ============
String ext = "Data";


bool initialInternetStat = false;
bool isInternet = false;

Adafruit_ADS1115 ads; /* Use this for the 16-bit version */

//========= DEFINE PINS ==========
// #define DHTPIN 16
// #define DHTTYPE DHT11

#define TRIGGER_PIN 13              //
#define ECHO_PIN 12                 //
const float speedOfSound = 0.0343;  // Speed of sound in cm/microsecond


const char* remote_host = "www.google.com";

// #define MOISTURE_SENSOR_PIN_1 35  //
#define MOISTURE_SENSOR_TRIG_1 33

// #define MOISTURE_SENSOR_PIN_2 34  //
#define MOISTURE_SENSOR_TRIG_2 32

#define ACCTUATOR_FORWARD 17
#define ACCTUATOR_BACKWARD 14
#define ACCTUATOR_TRIG 19

// #define MOTOR_PIN_FORWARD_2 21
// #define MOTOR_PIN_BACKWARD_2 19

#define BUZZER_PIN 27

#define PUMP_RELAY 26

//#define LED_WIFI 27
#define LED_INTERNET 18
#define LED_STATUS 5

#define Start_position 15
#define Plant_1_Position 16  //
#define Plant_2_Position 4   //
// #define SWITCH_PIN_4 25      //



//=========
// NewPing sonar(TRIGGER_PIN, ECHO_PIN);

DistanceSensor sensor(TRIGGER_PIN, ECHO_PIN);
// DHT dht(DHTPIN, DHTTYPE);






// WiFiManager wm;
// MyTable PlantData(id + "/" + id + "Plant.csv");
// MyTable Data(id + "/" + id + "_data.csv");



#define LEDC_CHANNEL 0


//Define Firebase Data object
unsigned long sendDataPrevMillis = 0;
bool signupOK = false;


unsigned long currerntMillis = millis();
unsigned long prevMillisINternetON = 0;
unsigned long prevMillisINternetOFF = 0;
unsigned long prevMillisWIFION = 0;
unsigned long prevMillisWIFIOFF = 0;


// CALLIBRATION VARIABLES ======
int moisture_1_Min = 0;     // Minimum sensor reading (dry soil)
int moisture_1_Max = 4095;  // Maximum sensor reading (wet soil)

int moisture_2_Min = 0;     // Minimum sensor reading (dry soil)
int moisture_2_Max = 3800;  // Maximum sensor reading (wet soil)
String isCalibrating = "false";
String isCalibratingStart = "false";
int Add1 = 0;
int Sub1 = 0;
int Add2 = 0;
int Sub2 = 0;


// Sensor One
float S1_CoeficientA = -0.0389;
float S1_CoeficientB = 2.098;
float S1_CoeficientC = 6.2819;

// Sensor Two
float S2_CoeficientA = -0.0443;
float S2_CoeficientB = 2.6503;
float S2_CoeficientC = 1.6619;


// WATER VARIABLES ======
float CONTAINER_HEIGHT = 100.0;  //in cm
String type = "tank";
float liter;


int P1MoistureVal = 0;
int P2MoistureVal = 0;
int waterLevel = 0;



// CONTROLS ====
String StartPlanting = "false";
String StartPlant1 = "false";
String StartPlant2 = "false";
String DateStarted = "00/00/0000";
String Reset = "false";

// Variable to check the control status
bool StartPantingExcuted = false;
bool StartPant1Excuted = false;
bool StartPant2Excuted = false;


// PLANT 1 PARAMETERS =======
float Plant1_PWP = 0.0,        // Permanent welting point in Percent
  Plant1_FCapacity = 0.0,      //Field in percentage
  Plant1_MAD = 0.0;            // Management allowable deficit in percent
int Plant1_CriticalPoint = 0;  // critical soil moisture content to trigger watering
int Plant1_watering_counter = 0;
float WaterConsume = 0.00;


// PLANT 2 PARAMETERS =======
float Plant2_PWP = 0.0,        // Permanent welting point in Percent
  Plant2_FCapacity = 0.0,      //Field in percentage
  Plant2_MAD = 0.0;            //Management allowable deficit in percent
int Plant2_CriticalPoint = 0;  // critical soil moisture content to trigger watering
int Plant2_watering_counter = 0;




FirebaseData fbdo;
FirebaseData RxData;


// ========= STREAM ========
FirebaseData fboStream;







// FBOSTREAM data =======
String NodeRoot = uid + "/Data";
String NodeChildren[35] = {


  "/Plant-1/Plant-1/FCapacity",
  "/Plant-1/Plant-1/CriticalPoint",
  "/Plant-1/Plant-1/PWP",
  "/Plant-1/Plant-1/MAD",
  "/Plant-1/Status/Counter",

  "/Plant-2/Plant-2/FCapacity",
  "/Plant-2/Plant-2/CriticalPoint",
  "/Plant-2/Plant-2/PWP",
  "/Plant-2/Plant-2/MAD",
  "/Plant-2/Status/Counter",


  "/Calibration/SoilMoisture-1/Dry",
  "/Calibration/SoilMoisture-1/Wet",
  "/Calibration/SoilMoisture-1/Add",
  "/Calibration/SoilMoisture-1/Sub",
  "/Calibration/SoilMoisture-1/CoeficientA",
  "/Calibration/SoilMoisture-1/CoeficientB",
  "/Calibration/SoilMoisture-1/CoeficientC",


  "/Calibration/SoilMoisture-2/Dry",
  "/Calibration/SoilMoisture-2/Wet",
  "/Calibration/SoilMoisture-2/Add",
  "/Calibration/SoilMoisture-2/Sub",
  "/Calibration/SoilMoisture-2/CoeficientA",
  "/Calibration/SoilMoisture-2/CoeficientB",
  "/Calibration/SoilMoisture-2/CoeficientC",



  "/Calibration/Status/IsCalibrating",

  "/Controls/Controls/StartPlanting",
  "/Controls/Controls/StartPlant1",
  "/Controls/Controls/StartPlant2",
  "/Controls/Controls/DateStarted",
  "/Controls/Controls/Reset",

  "/Water/Liters/Consume",
  "/Water/Water/Height",
  "/Water/Water/Type"

};


String currentStartPlantingStatus = "false";  // Initial status
String previousisPlantingStatus = "false";
bool isPlantingExecuted = false;  // Flag to track if the condition has been executed



String currentisCalibratingStatus = "false";  // Initial status
String previousisCalibratingStatus = "false";
bool isCalibratingExecuted = false;  // Flag to track if the condition has been executed


String currentisPlant1Start = "false";
String previousisPlant1Start = "false";
bool isPlant1StartExecuted = false;


String currentisPlant2Start = "false";
String previousisPlant2Start = "false";
bool isPlant2StartExecuted = false;



FirebaseAuth auth;
FirebaseConfig config;


//========== LOCAL TIME VARIABLES =========
const char* ntpServer = "asia.pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 28800;  //GMT+8:30
ESP32Time rtc(daylightOffset_sec);     //GMT+8:30


String dayMonth = "";
String amPm = "";
String monthNow = "";
String hourNow = "";
String yearNow = "";
String full_dateNow = "";
String loggedDay = "";



unsigned long prevWatering = 0;


enum MarioSound {
  SUPER_MARIO,
  POWER_UP,
  GAME_OVER,
  FLAGPOLE,
  STOMP,
  COIN,
  JUMP,
  BRICK_BREAK,
  PIPE_ENTER,
  POWER_DOWN,
  ERROR
};




void setup() {
  Serial.begin(115200);



  pinMode(MOISTURE_SENSOR_TRIG_1, OUTPUT);
  pinMode(MOISTURE_SENSOR_TRIG_2, OUTPUT);

  pinMode(ACCTUATOR_FORWARD, OUTPUT);
  pinMode(ACCTUATOR_BACKWARD, OUTPUT);
  pinMode(ACCTUATOR_TRIG, OUTPUT);
  digitalWrite(ACCTUATOR_FORWARD, LOW);
  digitalWrite(ACCTUATOR_BACKWARD, LOW);
  digitalWrite(ACCTUATOR_TRIG, LOW);

  // pinMode(MOTOR_PIN_FORWARD_2, OUTPUT);
  // pinMode(MOTOR_PIN_BACKWARD_2, OUTPUT);

  // pinMode(BUZZER_PIN, OUTPUT);
  pinMode(PUMP_RELAY, OUTPUT);

  // pinMode(LED_WIFI, OUTPUT);
  pinMode(LED_INTERNET, OUTPUT);
  pinMode(LED_STATUS, OUTPUT);

  pinMode(Start_position, INPUT_PULLUP);    //Start Position
  pinMode(Plant_1_Position, INPUT_PULLUP);  // Plant 1 Position
  pinMode(Plant_2_Position, INPUT_PULLUP);  //Plant 2 Position
  // pinMode(SWITCH_PIN_4, INPUT_PULLUP);

  // SET DEFAULT STATE ===

  // digitalWrite(LED_WIFI, LOW);

  // digitalWrite(LED_INTERNET, LOW);


  digitalWrite(MOISTURE_SENSOR_TRIG_1, LOW);
  digitalWrite(MOISTURE_SENSOR_TRIG_2, LOW);




  digitalWrite(PUMP_RELAY, HIGH);  // HIGH == DISABLE LOW == ENABLE
  ledcSetup(LEDC_CHANNEL, 5000, 8);
  ledcAttachPin(BUZZER_PIN, LEDC_CHANNEL);





  ads.setGain(GAIN_TWOTHIRDS);  // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1)
      ;
  }




  SD_INIT();




  // wm.setConfigPortalBlocking(false);

  settingWifi();
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);




  if (Ping.ping(remote_host, 1)) {  // CHECK FOR INTERNET CONNECTION


    // MAKE TONE SOUND
    playMarioSound(POWER_UP);
    digitalWrite(LED_INTERNET, HIGH);
    digitalWrite(LED_STATUS, HIGH);




    // Init and get the time


    ledcWriteTone(LEDC_CHANNEL, 987 + 1000);
    ledcWrite(LEDC_CHANNEL, 255);
    myfirebaseConfig();  // initialize Firebase
    playMarioSound(POWER_DOWN);
    initialInternetStat = true;
  } else {
    initialInternetStat = false;



    Serial.printf("Local data started");
    playMarioSound(STOMP);
    digitalWrite(LED_INTERNET, LOW);
    setTime("offline");
    getLocalTime();



    String csvDataPlant = readFile(SPIFFS, "/" + ext + "/" + "Plant.csv");
    String csvData = readFile(SPIFFS, "/" + ext + "/" + "_data.csv");
    Plant1_PWP = readCSVValue(csvDataPlant, 1, 1).toFloat();
    Plant1_FCapacity = readCSVValue(csvDataPlant, 1, 2).toFloat();
    Plant1_MAD = readCSVValue(csvDataPlant, 1, 3).toFloat();
    Plant1_CriticalPoint = readCSVValue(csvDataPlant, 1, 4).toInt();
    Plant1_watering_counter = readCSVValue(csvDataPlant, 1, 5).toInt();
    moisture_1_Min = readCSVValue(csvDataPlant, 1, 6).toInt();
    moisture_1_Max = readCSVValue(csvDataPlant, 1, 7).toInt();

    S1_CoeficientA = readCSVValue(csvDataPlant, 1, 8).toFloat();
    S1_CoeficientB = readCSVValue(csvDataPlant, 1, 9).toFloat();
    S1_CoeficientC = readCSVValue(csvDataPlant, 1, 10).toFloat();


    Plant2_PWP = readCSVValue(csvDataPlant, 2, 1).toFloat();
    Plant2_FCapacity = readCSVValue(csvDataPlant, 2, 2).toFloat();
    Plant2_MAD = readCSVValue(csvDataPlant, 2, 3).toFloat();
    Plant2_CriticalPoint = readCSVValue(csvDataPlant, 2, 4).toInt();
    Plant2_watering_counter = readCSVValue(csvDataPlant, 2, 5).toInt();
    moisture_2_Min = readCSVValue(csvDataPlant, 2, 6).toInt();
    moisture_2_Max = readCSVValue(csvDataPlant, 2, 7).toInt();

    S2_CoeficientA = readCSVValue(csvDataPlant, 1, 8).toFloat();
    S2_CoeficientB = readCSVValue(csvDataPlant, 1, 9).toFloat();
    S2_CoeficientC = readCSVValue(csvDataPlant, 1, 10).toFloat();



    Serial.println(Plant1_PWP);
    Serial.println(Plant1_FCapacity);
    Serial.println(Plant1_MAD);
    Serial.println(Plant1_CriticalPoint);
    Serial.println(Plant1_watering_counter);

    Serial.println("  ");

    Serial.println(Plant2_PWP);
    Serial.println(Plant2_FCapacity);
    Serial.println(Plant2_MAD);
    Serial.println(Plant2_CriticalPoint);
    Serial.println(Plant2_watering_counter);

    Serial.println(" MIN MAX\n ");

    Serial.println(moisture_1_Min);
    Serial.println(moisture_1_Max);
    Serial.println(moisture_2_Min);
    Serial.println(moisture_2_Max);
    String lastState = readCSVValue(csvData, 1, 1);
    CONTAINER_HEIGHT = readCSVValue(csvData, 2, 1).toInt();

    Add1 = readCSVValue(csvData, 3, 1).toInt();
    Sub1 = readCSVValue(csvData, 4, 1).toInt();
    Add2 = readCSVValue(csvData, 5, 1).toInt();
    Sub2 = readCSVValue(csvData, 6, 1).toInt();


    Serial.println("ADD SUB");
    Serial.println(Add1);
    Serial.println(Sub1);
    Serial.println(Add2);
    Serial.println(Sub2);


    Serial.println(" ");
    Serial.println(lastState);
    lastState.trim();
    if (lastState == "true") {
      StartPantingExcuted = true;

    } else if (lastState == "false") {
      StartPantingExcuted = false;
    }


    playMarioSound(STOMP);
    digitalWrite(LED_INTERNET, LOW);
  }






  // dht.begin();












  move_to_start();
}




// CHECK IF PLANTING STARTED =======
void isPlantingStarted() {
  // Check for changes in status
  if (currentStartPlantingStatus != previousisPlantingStatus) {

    // Execute your condition if not already executed
    if (currentStartPlantingStatus == "true" && !isPlantingExecuted) {


      isPlantingExecuted = true;  // Set the flag
      feedBackIsPlanting(true);


    } else if (currentStartPlantingStatus == "false" && !isPlantingExecuted) {
      isPlantingExecuted = true;  // Set the flag
      feedBackIsPlanting(false);

      State(1, "PAUSED");
      State(2, "PAUSED");
    }
    // Update previousisPlantingStatus
    previousisPlantingStatus = currentStartPlantingStatus;
  } else {
    // Reset the isPlantingExecuted flag when the status remains the same
    isPlantingExecuted = false;
  }
}



// CHECK IF CALIBRATION STARTED =======
void isCalibrateStarted() {
  // Check for changes in status
  if (currentisCalibratingStatus != previousisCalibratingStatus) {

    // Execute your condition if not already executed
    if (currentisCalibratingStatus == "true" && !isCalibratingExecuted) {

      isCalibratingExecuted = true;  // Set the flag
      feedBackIsCalibrating(true);
    } else if (currentisCalibratingStatus == "false" && !isCalibratingExecuted) {

      isCalibratingExecuted = true;  // Set the flag
      feedBackIsCalibrating(false);
    }
    // Update previousisPlantingStatus
    previousisCalibratingStatus = currentisCalibratingStatus;
  } else {
    // Reset the isPlantingExecuted flag when the status remains the same
    isCalibratingExecuted = false;
  }
}



// check for manual watering is ON

void isPlant1Started() {

  if (currentisPlant1Start != previousisPlant1Start) {

    // Execute your condition if not already executed
    if (currentisPlant1Start == "true" && !isPlant1StartExecuted) {
      // feedback true here

      isPlant1StartExecuted = true;  // Set the flag
      P1_Manual(feedbackIsPlant1Started(true));
    } else if (currentisPlant1Start == "false" && !isPlant1StartExecuted) {
      // feedback false here

      isPlant1StartExecuted = true;  // Set the flag
      P1_Manual(feedbackIsPlant1Started(false));
    }
    // Update previousisPlantingStatus
    previousisPlant1Start = currentisPlant1Start;
  } else {
    // Reset the isPlantingExecuted flag when the status remains the same
    isPlant1StartExecuted = false;
  }
}


void isPlant2Started() {

  if (currentisPlant2Start != previousisPlant2Start) {

    // Execute your condition if not already executed
    if (currentisPlant2Start == "true" && !isPlant2StartExecuted) {
      // feedback true here

      isPlant2StartExecuted = true;  // Set the flag
      P2_Manual(feedbackIsPlant2Started(true));
    } else if (currentisPlant2Start == "false" && !isPlant2StartExecuted) {
      // feedback false here

      isPlant2StartExecuted = true;  // Set the flag
      P2_Manual(feedbackIsPlant2Started(false));
    }
    // Update previousisPlantingStatus
    previousisPlant2Start = currentisPlant2Start;
  } else {
    // Reset the isPlantingExecuted flag when the status remains the same
    isPlant2StartExecuted = false;
  }
}


void loop() {
  wm.process();
  // isConnectedToInternet();








  // Serial.println("Stream availablbe "+fboStream.streamAvailable());

  //  Serial.println("Monitor " + StartPlanting);

  isPlantingStarted();
  isCalibrateStarted();
  isPlant1Started();
  isPlant2Started();
  // sendSensorToDatabase();

  Serial.println(full_dateNow);
  if (isConnectedToInternet()) {

    if (Firebase.ready()) {
    }

    // Firebase.authenticated();
    Serial.println(fboStream.errorCode());
  } else {
  }
  sendSensorToDatabase();




  checkWifiConnection();


  Serial.println("START WATERING ============" + String(P1MoistureVal) + "  " + String(P2MoistureVal));
  if (StartPantingExcuted && (waterLevel > 10)) {
    digitalWrite(LED_STATUS, HIGH);
    // Serial.println("START WATERING ============" + String(P1MoistureVal) + "  " + String(P2MoistureVal));
    StartWaterting();

  } else {
    digitalWrite(LED_STATUS, LOW);
    // Serial.println(" NOT Start watering " + StartPantingExcuted);
  }

  Calibrate();



  delay(10);
}
