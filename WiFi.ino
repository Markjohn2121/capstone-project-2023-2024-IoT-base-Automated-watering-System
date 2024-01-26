#include <Arduino.h>
#include <WiFiClient.h>

bool currentWifiStat = false;
bool ONInternetStat = false;
bool NOInternetStat = false;
bool firebaseStat = false;

const char* ssid = machineID.c_str();
const char* password = machinePASSWORD.c_str();
bool res = false;

bool isConnectedToInternet() {

  if (!Ping.ping(remote_host, 3)) {  // iF NO INTERNET CONNECTION



    ONInternetStat = false;
    if (!NOInternetStat && initialInternetStat) {
      Serial.println("NO internet");



      fbdo.pauseFirebase(true);
      fbdo.clear();
      RxData.pauseFirebase(true);
      RxData.clear();
      fboStream.pauseFirebase(true);
      fboStream.clear();

      // fbdo.maxPayloadLength()
      playMarioSound(BRICK_BREAK);
      digitalWrite(LED_INTERNET, LOW);

      NOInternetStat = true;
    } else {
    }

    isInternet = false;
    return false;
  } else {





    // digitalWrite(LED_INTERNET,LOW);



    NOInternetStat = false;

    if (!initialInternetStat) {


      if (!firebaseStat) {
        wm.disconnect();
        ESP.restart();
        myfirebaseConfig();

        firebaseStat = true;
        initialInternetStat = true;
      }
    }





    if (!ONInternetStat && initialInternetStat) {



      Serial.println("On internet");



      if (Firebase.ready() && Firebase.authenticated() && Firebase.isTokenExpired()) {

        Firebase.refreshToken(&config);

        config.signer.lastReqMillis = 0;
      }


      // config.signer.tokens.expires =0;
      fbdo.pauseFirebase(false);
      RxData.pauseFirebase(false);
      fboStream.pauseFirebase(false);

      RxData.clear();
      fbdo.clear();

      fboStream.pauseFirebase(true);
      fboStream.clear();
      fboStream.pauseFirebase(false);
      Firebase.RTDB.setMultiPathStreamCallback(&fboStream, streamCallbackPlant_1, streamTimeoutCallback);
      playMarioSound(FLAGPOLE);
      digitalWrite(LED_INTERNET, HIGH);

      ONInternetStat = true;
    }



    isInternet = true;
    return true;
  }
}


bool checkWifiConnection() {

  if (WiFi.status() != WL_CONNECTED) {
    if ((millis() - prevMillisWIFION) >= 4000) {
      Serial.println("Wi-Fi disconnected! HIGH");
      // Your disconnection handling code here
      digitalWrite(LED_INTERNET, HIGH);
      prevMillisWIFION = millis();
    }
    if ((millis() - prevMillisWIFIOFF) >= 5500) {

      Serial.println("Wi-Fi disconnected! LOW");
      // Your disconnection handling code here
      digitalWrite(LED_INTERNET, LOW);
      prevMillisWIFIOFF = millis();
    }
    // if (res) {

    //   wm.setConfigPortalBlocking(false);
    // res = wm.autoConnect(ssid, password);
    //   }
    return false;
  } else {



    //digitalWrite(LED_WIFI,HIGH);

    if (!res) {

      wm.disconnect();
      ESP.restart();
    }

    return true;
  }
}



void settingWifi() {



  //For AP mod IPADDRESS==========
  // IPAddress apIP(192, 168, 4, 1);
  // IPAddress apGW(192, 168, 1, 1);
  // IPAddress apSM(255, 255, 0, 0);
  // IPAddress apDNS1(8, 8, 8, 8);
  // IPAddress apDNS2(8, 8, 4, 4);
  //char static_ip[16] = "10,0,1,78";
  //char static_gw[16] = "10,0,1,1";
  //char static_sm[16] = "255,255,255,0";
  //For AP mod IPADDRESS==========


  //if(digitalRead(onlineBtn) == HIGH){ // OFFLINE MODE IF BTN IF OFF
  //  delay(500);
  //    Serial.println("OfflineMode");
  //    digitalWrite(onlineLed,LOW);
  //     delay(500);
  //     WiFi.mode(WIFI_AP);
  //      WiFi.softAPConfig(apIP,apIP,IPAddress(255,255,255,0));
  //      WiFi.softAP(ssid,password);
  //
  //
  //  Serial.println("");
  //  Serial.print("IP Address: ");
  //  Serial.println(WiFi.softAPIP());



  //}else {

  //        digitalWrite(onlineLed,HIGH);
  //    if(!WiFi.config(apIP,apGW,apSM,apDNS1,apDNS2)){
  //      Serial.println("STA failed to config");
  //    }
  //
  //      Serial.println("STA connecting to YOUR_SSID ");
  //      WiFi.begin("YOUR_SSID","YOUR_PASSWORD");
  //      while(WiFi.status() != WL_CONNECTED){
  //        delay(500);
  //        Serial.print(".");
  //      }
  //  Serial.println("");
  //    Serial.println("IP Address: ");
  //      Serial.println(WiFi.localIP());

  //   Serial.println("OnlineMode");
  //    delay(500);
  //   //=============ONLINE MODE=======
  //    // WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  //    // it is a good practice to make sure your code sets wifi mode how you want it.
  //
  //    // put your setup code here, to run once:
  //    Serial.begin(115200);
  //
  //
  //
  //    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around



  //
  //    // reset settings - wipe stored credentials for testing
  //    // these are stored by the esp library
  // wm.resetSettings();
  //
  ////  IPAddress _ip,_gw,_sm ;
  ////  _ip.fromString(static_ip);
  ////    _gw.fromString(static_gw);
  ////    _sm.fromString(static_sm);
  //
  //
  ////  IPAddress _ip = IPAddress(10,0,1,78);
  ////  IPAddress _gw = IPAddress(10,0,1,1);
  ////  IPAddress _sm = IPAddress(255,255,255,0);
  //wm.setSTAStaticIPConfig(IPAddress(192,168,0,99),IPAddress(192,168,0,1),IPAddress(255,255,255,0));
  wm.setConfigPortalBlocking(false);

  wm.setWiFiAutoReconnect(true);
  wm.setCleanConnect(true);
  wm.setMinimumSignalQuality(-1);
  res = wm.autoConnect(ssid, password);  // password protected ap


  Serial.println(" CONNECTED");
  // WiFi.config(WiFi.localIP(), WiFi.gatewayIP(), WiFi.subnetMask(), WiFi.dnsIP(0), WiFi.dnsIP(1));
  //     WiFi.enableLongRange(true);
  if (!res) {
    Serial.println("Failed to connect");
    /// ESP.restart();
  } else {
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
    //WiFi.enableLongRange(true);



    //digitalWrite(LED_WIFI,HIGH);
  }
  //======================
  //  }
}


//
//#include <Arduino.h>
//
//
//#define WIFI_SSID "SHUPETA_MARK"
//#define WIFI_PASSWORD "YOUR_PASSWORD"
//
//
//
//void setMyWifi(){
//
//
//  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//  Serial.print("Connecting to Wi-Fi");
//  while (WiFi.status() != WL_CONNECTED){
//    Serial.print(".");
//    delay(300);
//  }
//  Serial.println();
//  Serial.print("Connected with IP: ");
//  Serial.println(WiFi.localIP());
//  Serial.println();
//
//
//
//
//}
