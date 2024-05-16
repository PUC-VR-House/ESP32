#include <WiFi.h>                                               
#include <IOXhop_FirebaseESP32.h>                                            

//WIFI - FIREBASE - SETTINGS
  #define FIREBASE_HOST "https://teste-casa-4d9fb-default-rtdb.firebaseio.com/"                      
  #define FIREBASE_AUTH "AIzaSyBSY4AEXaK-DsLq3CMcKzZFxWkSMdcbMgw"                   
  #define WIFI_SSID "2GClaroAp202"                                         
  #define WIFI_PASSWORD "vlmrap202"  

//FIREBASE - DATA
  String fireLed1 = "";                                                     
  int led1 = 26; 
  String fireLed2 = "";   
  int led2 = 25; 
  String fireLed3 = "";    
  int led3 = 33; 
  String fireLed4 = "";    
  int led4 = 32;                                                                

void setup() {
  Serial.begin(115200);
  delay(500);

  //OUTPUTS
    pinMode(26, OUTPUT); 
    pinMode(25, OUTPUT);
    pinMode(33, OUTPUT);
    pinMode(32, OUTPUT);    

  //WIFI
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                   
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
    Serial.println(WiFi.localIP()); 

  //FIREBASE                                                  
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                                   
}

void loop() {

  //CHECK FIREBASE DATA     
    // Check Firebase data for LED 1
    fireLed1 = Firebase.getString("led1");
    if (fireLed1 == "ON") {                     
      Serial.println("LED 1 Turned ON");                 
      digitalWrite(led1, HIGH);                                                        
    } else {             
      Serial.println("LED 1 Turned OFF");
      digitalWrite(led1, LOW);                                                       
    }
    // Check Firebase data for LED 2
    fireLed2 = Firebase.getString("led2");
    if (fireLed2 == "ON") {                     
      Serial.println("LED 2 Turned ON");                 
      digitalWrite(led2, HIGH);                                                        
    } else {             
      Serial.println("LED 2 Turned OFF");
      digitalWrite(led2, LOW);                                                       
    }
    // Check Firebase data for LED 3
    fireLed3 = Firebase.getString("led3");
    if (fireLed3 == "ON") {                     
      Serial.println("LED 3 Turned ON");                 
      digitalWrite(led3, HIGH);                                                        
    } else {             
      Serial.println("LED 3 Turned OFF");
      digitalWrite(led3, LOW);                                                       
    }
    // Check Firebase data for LED 4
    fireLed4 = Firebase.getString("led4");
    if (fireLed4 == "ON") {                     
      Serial.println("LED 4 Turned ON");                 
      digitalWrite(led4, HIGH);                                                        
    } else {             
      Serial.println("LED 4 Turned OFF");
      digitalWrite(led4, LOW);                                                       
    }
}
