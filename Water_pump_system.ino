
#include <Arduino.h>

unsigned long prevSec = 0;
long currentSec = 0;

unsigned long currentMillis = 0;
unsigned long lastMillis = 0;
int reading = 0;
int prevMillisReading = 0;
void Start_Pumping(int Plant) {
  // Delay before starting
  
PauseStream();
  if (Plant == 1) {
    State(1, "ON GOING");
   
   
    reading = m1();
    prevMillisReading = millis();
    currentMillis = millis();
    while (P1MoistureVal <= Plant1_FCapacity) {
      digitalWrite(PUMP_RELAY, LOW);  // ENABLE PUMP

      // // Read sensor value every 4 sec
      if ((millis() - prevMillisReading) >= 3000) {

        reading = m1();
        prevMillisReading = millis();
          Serial.println(m1());
      }



    pumplogg();
      Serial.println("Pumping");
      // delay(10);
    
    }

    
  } else if (Plant == 2) {

    State(2, "ON GOING");
    
  
    reading = m2();
    prevMillisReading = millis();
     currentMillis = millis();
    while (P2MoistureVal <= Plant2_FCapacity) {
      digitalWrite(PUMP_RELAY, LOW);  // ENABLE PUMP


     


  // // Read sensor value every 4 sec
      if ((millis() - prevMillisReading) >= 3000) {

        reading = m2();
        prevMillisReading = millis();
         Serial.println(m2());
      }

pumplogg();
      Serial.println("Pumping");



     
     
    }

    
  }


  // Disable pump after pumping
 
  digitalWrite(PUMP_RELAY, HIGH);
  lastMillis = millis();
  currentSec = (lastMillis - currentMillis) / 1000;


  // Log water consumption and reset timer
  logg_Water_consume(currentSec ,Plant);
  currentSec = 0;
lastMillis =0;
  currentMillis =0;

  // Check moisture levels and move to appropriate positions
// Plant1CriticalPoint();
// Plant2CriticalPoint();
      pauseFB();
      
move_to_start();
  


}









void pumplogg(){
 isConnectedToInternet();

            if (isInternet) {

              Firebase.ready();
                 
            }else{

            }
sendSensorToDatabase();

           

    // if ((millis() - prevSec) >= 1000) {
    //       currentSec++;




    //         if (isInternet) {

    //           Firebase.ready();
    //              sendSensorToDatabase();
    //         }
    //     // isConnectedToInternet();
       


    //       prevSec = millis();  
    //     }




   


}









// #include <Arduino.h>

// unsigned long  prevSec = 0;
// unsigned long currentSec = 0;

// void Start_Pumping(int Plant) {


//   if (Plant == 1) {
//     delay(500);
//     State(1, "ON GOING");
//     delay(500);
//     prevSec = millis() ;
//     while (readMoisture_sensor_1() <= Plant1_FCapacity) {  //Start Water pumping

//       digitalWrite(PUMP_RELAY, LOW);  // ENABLE PUMP

//     if((millis()-prevSec) >= 1000){
//       currentSec +=1;
//        prevSec = millis() ;
//     }

//       sendSensorToDatabase();
//       Serial.println("Pumping");
//       delay(10);
//       Serial.println(readMoisture_sensor_1());
//     }

//   } else if (Plant == 2) {
//     delay(500);
//     State(2, "ON GOING");
//     delay(500);
//     prevSec = millis() ;
//     while (readMoisture_sensor_2() <= Plant2_FCapacity) {  //Start Water pumping

//       digitalWrite(PUMP_RELAY, LOW);  // ENABLE PUMP
//        if((millis()-prevSec) >= 1000){
//       currentSec +=1;
//        prevSec = millis() ;
//     }

//       sendSensorToDatabase();
//       Serial.println("Pumping");
//       delay(10);
//       Serial.println(readMoisture_sensor_2());
//     }
//   }

// logg_Water_consume(currentSec);
// currentSec = 0;



//   delay(500);
//   digitalWrite(PUMP_RELAY, HIGH);
//   delay(500);
//   if (readMoisture_sensor_1() <= Plant1_CriticalPoint) {
//     if (digitalRead(Start_position) == 0) {  // IF CURRENT POSITION IS IN START
//       move_to_plant1("fwrd");
//     } else if (digitalRead(Plant_2_Position) == 0) {  //IF CURRENT POSITION IS IN Position 2
//       move_to_plant1("rev");
//     } else {
//       move_to_start();
//       move_to_plant1("fwrd");
//     }

//   } else if (readMoisture_sensor_2() <= Plant2_CriticalPoint) {
//     if (digitalRead(Start_position) == 0) {  // IF CURRENT POSITION IS IN START
//       move_to_plant2("fwrd");
//     } else if (digitalRead(Plant_1_Position) == 0) {  //IF CURRENT POSITION IS IN Position1
//       move_to_plant2("fwrd");
//     } else {
//       move_to_start();
//       move_to_plant2("fwrd");
//     }
//   } else {
//     move_to_start();
//   }
// }