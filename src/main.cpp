/*
  Rui Santos
  Complete project details at our blog.
    - ESP32: https://RandomNerdTutorials.com/esp32-firebase-realtime-database/
    - ESP8266: https://RandomNerdTutorials.com/esp8266-nodemcu-firebase-realtime-database/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  Based in the RTDB Basic Example by Firebase-ESP-Client library by mobizt
  https://github.com/mobizt/Firebase-ESP-Client/blob/main/examples/RTDB/Basic/Basic.ino
*/

#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "WhiteSky-Marshall"
#define WIFI_PASSWORD "x7jy8n9p"

#define USER_EMAIL "testemail@gmail.com"
#define USER_PASSWORD "MySUPERCoolPassword1"

// Insert Firebase project API Key
#define API_KEY "AIzaSyDDRdQUAOfqf1TpKn8eEc_opS5pm6iRuhQ"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://roommate-74f3d-default-rtdb.firebaseio.com/" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

uint8_t mic_data = 0;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

int currentScore = 0;
int newScore = 0;

const char* carissaStuff = "our_house/Joe/reps/";

void setup(){
  pinMode(25, INPUT);

  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Sign up */
  /*if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }*/

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Firebase.RTDB.setMaxRetry(&fbdo, 3);
}

void loop(){
  /*if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    // Write an Int number on the database path test/int
    if (Firebase.RTDB.setInt(&fbdo, "test/int", count)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    count++;
    
    // Write an Float number on the database path test/float
    if (Firebase.RTDB.setFloat(&fbdo, "test/float", 0.01 + random(0,100))){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }*/

  mic_data = digitalRead(25);

  if (mic_data) {
    if (Firebase.ready() && millis() - sendDataPrevMillis > 1000) {
      sendDataPrevMillis = millis();

      FirebaseJson carissaLoud;

      carissaLoud.set("dateAssigned", 777);
      carissaLoud.set("dateDone", 0);
      carissaLoud.set("description", "TOO LOUD!!!");
      carissaLoud.set("done", 0);
      carissaLoud.set("id", 99);
      carissaLoud.set("points", -7);

      int randomValue = random(10, 99);

      Serial.write("randomValue = ");
      printf("%d \n", randomValue);

      fflush(stdout);

      char addToCarissa[3];
      sprintf(addToCarissa, "%i", randomValue);

      fflush(stdout);

      char newCarissaString[40];
      strcpy(newCarissaString, carissaStuff);
      strcat(newCarissaString, addToCarissa);

      printf("PathString = %s", newCarissaString);

      fflush(stdout);

      Firebase.RTDB.setJSON(&fbdo, newCarissaString, &carissaLoud);
      //Firebase.RTDB.setJSON(&fbdo, "our_house/Joe/reps/8", &carissaLoud);

      //currentScore = Firebase.RTDB.getInt(&fbdo, "our_house/Carissa/score");

      newScore = currentScore - 7;

      //Firebase.RTDB.setIntAsync(&fbdo, "our_house/Carissa/score", newScore);
    }
  }
}