#include <Arduino.h>






// Insert Firebase project API Key
#define API_KEY "AIzaSyABOi1ik3cZwKJVntQF7t_kjqUOz91y-5I"
// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://iot-aws-a65d9-default-rtdb.asia-southeast1.firebasedatabase.app/"

#define USER_EMAIL machineID + "@gmail.com"
#define USER_PASSWORD machinePASSWORD
#define DATABASE_SECRET "rGczG00W1bb1ZJRiJhn2YnJbbZDIpGfSE3qwxn30"


void myfirebaseConfig() {
  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;


  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;  // see addons/TokenHelper.h

  // To refresh the token 5 minutes before expired
  config.signer.preRefreshSeconds = 2420;
  config.max_token_generation_retry = 5;

  config.signer.lastReqMillis = 0;


  // //  /* Sign up */
  //  if (Firebase.(&config, &auth, machineID+"@gmail.com", machinePASSWORD)){
  //    Serial.println("ok");
  //    signupOK = true;
  //  }
  //  else{
  //    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  //  }

  /* Assign the callback function for the long running token generation task */
  // config.token_status_callback = tokenStatusCallback;  //see addons/TokenHelper.h
  Serial.println("ok");
  signupOK = true;
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  setTime("online");
  getLocalTime();
  Firebase.begin(&config, &auth);
  config.fcs.upload_buffer_size = 1000;
  // Firebase.reconnectWiFi(false);
  Firebase.reconnectNetwork(true);

  fbdo.setBSSLBufferSize(10048 /* Rx buffer size in bytes from 512 - 16384 */, 10048 /* Tx buffer size in bytes from 512 - 16384 */);
  fboStream.setBSSLBufferSize(10048 /* Rx buffer size in bytes from 512 - 16384 */, 5048 /* Tx buffer size in bytes from 512 - 16384 */);
  RxData.setBSSLBufferSize(10048 /* Rx buffer size in bytes from 512 - 16384 */, 10048 /* Tx buffer size in bytes from 512 - 16384 */);
  RxData.setResponseSize(1000);



  fboStream.keepAlive(30, 40, 10);
  RxData.keepAlive(40, 15, 5);
  fbdo.keepAlive(30, 5, 3);







  // STREAM
  if (!Firebase.RTDB.beginMultiPathStream(&fboStream, NodeRoot))
    Serial.printf("sream begin error, %s\n\n", fboStream.errorReason().c_str());

  Firebase.RTDB.setMultiPathStreamCallback(&fboStream, streamCallbackPlant_1, streamTimeoutCallback);



  //  /** Timeout options, below is default config.

  //Network reconnect timeout (interval) in ms (10 sec - 5 min) when network or WiFi disconnected.
  config.timeout.networkReconnect = 300 * 1000;

  //Socket begin connection timeout (ESP32) or data transfer timeout (ESP8266) in ms (1 sec - 1 min).
  config.timeout.socketConnection = 60 * 1000;

  //ESP32 SSL handshake in ms (1 sec - 2 min). This option doesn't allow in ESP8266 core library.
  config.timeout.sslHandshake = 2 * 60 * 1000;

  //Server response read timeout in ms (1 sec - 1 min).
  config.timeout.serverResponse = 60 * 1000;

  //RTDB Stream keep-alive timeout in ms (20 sec - 2 min) when no server's keep-alive event data received.
  config.timeout.rtdbKeepAlive = 60 * 1000;

  //RTDB Stream reconnect timeout (interval) in ms (1 sec - 1 min) when RTDB Stream closed and want to resume.
  config.timeout.rtdbStreamReconnect = 5 * 1000;

  //RTDB Stream error notification timeout (interval) in ms (3 sec - 30 sec). It determines how often the readStream
  //will return false (error) when it called repeatedly in loop.
  config.timeout.rtdbStreamError = 3 * 1000;


  //Socket begin connection timeout (ESP32) or data transfer timeout (ESP8266) in ms (1 sec - 1 min).
  // config.timeout.socketConnection = 50 * 1000;

  initialInternetStat = true;
}










void resolve() {



  if (!Firebase.authenticated()) {

    Serial.println("Not authenticated");
  }

  if (!fbdo.httpConnected() || !RxData.httpConnected()) {
    Serial.println("RESOLVING -- ");

    Firebase.reconnectNetwork(true);
  }

  //  if (fbdo.errorCode() == -120 || RxData.errorCode() == -120) {  // token expired or revoke
  //   Serial.println("RESOLVING -- " );
  //   pauseFB();
  //   Firebase.refreshToken(&config);
  //   resumeFB();
  // }

  if ((fbdo.errorCode() == -1 || fbdo.errorCode() == -4) || (RxData.errorCode() == -1 || RxData.errorCode() == -4)) {  // Connection refused

    Serial.println("RESOLVING -- ");
    pauseFB();
    Firebase.reconnectNetwork(true);
    resumeFB();
  }
}

void PauseStream() {
  if (isConnectedToInternet()) {


    fboStream.pauseFirebase(true);
    fboStream.clear();

    Serial.println(" Stream PUASED");
  }
}


void pauseFB() {
  ledcWrite(LEDC_CHANNEL, 0);

  if (isConnectedToInternet()) {

    fbdo.pauseFirebase(true);
    fbdo.clear();
    RxData.pauseFirebase(true);
    RxData.clear();
    fboStream.pauseFirebase(true);
    fboStream.clear();

    Serial.println("PUASED");
  }
}

void resumeFB() {

  if (isConnectedToInternet()) {



    if (Firebase.ready() && Firebase.authenticated() && Firebase.isTokenExpired()) {

      Firebase.refreshToken(&config);

      config.signer.lastReqMillis = 0;
    }
    config.signer.lastReqMillis = 0;
    fbdo.pauseFirebase(false);
    fbdo.clear();
    RxData.pauseFirebase(false);
    RxData.clear();
    fboStream.pauseFirebase(false);
    Firebase.RTDB.setMultiPathStreamCallback(&fboStream, streamCallbackPlant_1, streamTimeoutCallback);

    Serial.println("RESUMING");
  } else {
    Serial.println("RESUMING No internet");
  }
}






// The Firebase Storage upload callback function
void fcsUploadCallback(FCS_UploadStatusInfo info) {
  if (info.status == firebase_fcs_upload_status_init) {
    Serial.printf("Uploading file %s (%d) to %s\n", info.localFileName.c_str(), info.fileSize, info.remoteFileName.c_str());
  } else if (info.status == firebase_fcs_upload_status_upload) {
    Serial.printf("Uploaded %d%s, Elapsed time %d ms\n", (int)info.progress, "%", info.elapsedTime);
  } else if (info.status == firebase_fcs_upload_status_complete) {
    Serial.println("Upload completed\n");
    FileMetaInfo meta = fbdo.metaData();
    Serial.printf("Name: %s\n", meta.name.c_str());
    Serial.printf("Bucket: %s\n", meta.bucket.c_str());
    Serial.printf("contentType: %s\n", meta.contentType.c_str());
    Serial.printf("Size: %d\n", meta.size);
    Serial.printf("Generation: %lu\n", meta.generation);
    Serial.printf("Metageneration: %lu\n", meta.metageneration);
    Serial.printf("ETag: %s\n", meta.etag.c_str());
    Serial.printf("CRC32: %s\n", meta.crc32.c_str());
    Serial.printf("Tokens: %s\n", meta.downloadTokens.c_str());
    Serial.printf("Download URL: %s\n\n", fbdo.downloadURL().c_str());
  } else if (info.status == firebase_fcs_upload_status_error) {
    Serial.printf("Upload failed, %s\n", info.errorMsg.c_str());
  }
}
