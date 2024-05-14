#include <WiFi.h>                                                // esp32 library
#include <IOXhop_FirebaseESP32.h>                                             // firebase library
#include <ArduinoJson.h>

#define FIREBASE_HOST "https://teste-casa-4d9fb-default-rtdb.firebaseio.com/"                         // the project name address from firebase id
#define FIREBASE_AUTH "AIzaSyBSY4AEXaK-DsLq3CMcKzZFxWkSMdcbMgw"                    // the secret key generated from firebase
#define WIFI_SSID "motorafael"                                          // input your home or public wifi name
#define WIFI_PASSWORD "1234567890"                                    //password of wifi ssid

String fireStatus = "";                                                     // led status received from firebase
int led = 13;                                                               

void setup() {

  Serial.begin(115200);
  delay(1000);
  pinMode(13, OUTPUT);                
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                      //try to connect with wifi
  Serial.println("Connecting to ");
  Serial.println(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.println("IP Address is : ");
  Serial.println(WiFi.localIP());                                                      //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                                       // connect to firebase
  //Firebase.setString("fir-d-led-bang-bang", "OFF");                                          //send initial string of led status

}

void loop() {
  
  fireStatus = Firebase.getString("fir-d-led-bang-bang");                     // get led status input from firebase
  
  if (fireStatus == "ON") {                         // compare the input of led status received from firebase
    Serial.println("Led Turned ON");                 
    digitalWrite(led, HIGH);                                                         // make output led ON
  }

  else if (fireStatus == "OFF") {              // compare the input of led status received from firebase
    Serial.println("Led Turned OFF");
    digitalWrite(led, LOW);                                                         // make output led OFF
  }

  else {
    Serial.println("Wrong Credential! Please send ON/OFF");
  }

}
