#include <WiFi.h>                                               
#include <IOXhop_FirebaseESP32.h> 
#include <Servo.h>   
#include <Adafruit_AHTX0.h>  

//WIFI - FIREBASE - SETTINGS
  #define FIREBASE_HOST "https://teste-casa-4d9fb-default-rtdb.firebaseio.com/"                      
  #define FIREBASE_AUTH "AIzaSyBSY4AEXaK-DsLq3CMcKzZFxWkSMdcbMgw"                   
  #define WIFI_SSID "2GClaroAp202"                                         
  #define WIFI_PASSWORD "vlmrap202" 
  unsigned long lastReadTime = 0;
  unsigned long readInterval = 5000;  

//SERVO
  Servo s1;
  String door;  
  int pos = 0; 
  int x;

//AHTX10 - HUM - TEMP
  float temperature = 0.0;
  float humidity = 0.0;
  float aht_old_temp;
  float aht_old_humidity;

//FIREBASE - DATA
  String fireLed1 = "";                                                     
  int led1 = 26; 
  String fireLed2 = "";   
  int led2 = 25; 
  String fireLed3 = "";    
  int led3 = 33; 
  String fireLed4 = "";    
  int led4 = 32;                                                                

//AHT 10
  Adafruit_AHTX0 aht;
  Adafruit_Sensor *aht_humidity, *aht_temp;
void setup() 
{
  Serial.begin(115200);
  delay(500);

  //SERVO
    s1.attach(13);
    // Initially the servo must be stopped 
    s1.write(90);

  //OUTPUTS
    pinMode(26, OUTPUT); 
    pinMode(25, OUTPUT);
    pinMode(33, OUTPUT);
    pinMode(32, OUTPUT);    

  //LED'S
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);

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

  // Initialize AHT sensor
    if (!aht.begin()) {
      Serial.println("Failed to find AHT10/AHT20 chip");
      while (1) {
        delay(10);
      }
    }  
    Serial.println("AHT10/AHT20 Found!");
    aht_temp = aht.getTemperatureSensor();
    aht_humidity = aht.getHumiditySensor();  
    door = Firebase.getString("door"); 
}

void loop() 
{
  //CHECK TEMP HUM DATA
    sensors_event_t humidity;
    sensors_event_t temp;
    aht_humidity->getEvent(&humidity);
    aht_temp->getEvent(&temp);
    Serial.println("-Sensor terperature: " + String(temp.temperature));
    Serial.println("-Sensor humidity: " + String(humidity.relative_humidity));
    if (int(aht_old_temp) != int(temp.temperature)) {
      aht_temp->getEvent(&temp);
      Firebase.setString("temp", String(temp.temperature));
      Serial.println("-Updated temperature");
    }
    if (int(aht_old_humidity) != int(humidity.relative_humidity)) {
      aht_humidity->getEvent(&humidity);
      Firebase.setString("hum", String(humidity.relative_humidity));
      Serial.println("-Updated humidity");
    }
    aht_old_temp = temp.temperature;
    aht_old_humidity = humidity.relative_humidity;

    //CHECK FIREBASE    
      // Check if it's time to read from Firebase
      Serial.println("\n===ESP32-CICLE===\n-Firebase Request: ");
        //CHECK DOORS
        if (door != Firebase.getString("door")) {
          door = Firebase.getString("door");         
          if (door == "ON") {
            // Start turning clockwise
            s1.write(0);
            // Go on turning for the right duration
            delay(300);
            // Stop turnings
            s1.write(90);                                                      
          }
          if (door == "OFF") { 
            // Start turning clockwise
            s1.write(100);
            // Go on turning for the right duration
            delay(300);
            // Stop turning
            s1.write(90);                                                        
          }
        }

        //CHECK LIGHT
        lastReadTime = millis();
        if (fireLed1 != Firebase.getString("led1")) {
          fireLed1 = Firebase.getString("led1");
          if (fireLed1 == "ON") {                     
            Serial.print(" Led1-ON");                 
            digitalWrite(led1, HIGH);                                                        
          } else {             
            Serial.print(" Led1-OFF");
            digitalWrite(led1, LOW);                                                       
          }
        }
        if (fireLed2 != Firebase.getString("led2")) {
          fireLed2 = Firebase.getString("led2");
          if (fireLed2 == "ON") {                     
            Serial.print(" Led2-ON");                 
            digitalWrite(led2, HIGH);                                                        
          } else {             
            Serial.print(" Led2-OFF");
            digitalWrite(led2, LOW);                                                       
          }
        }
        if (fireLed3 != Firebase.getString("led3")) {
          fireLed3 = Firebase.getString("led3");
          if (fireLed3 == "ON") {                     
            Serial.print(" Led3-ON");                 
            digitalWrite(led3, HIGH);                                                        
          } else {             
            Serial.print(" Led3-OFF");
            digitalWrite(led3, LOW);                                                       
          }
        }
        if (fireLed4 != Firebase.getString("led4")) {
          fireLed4 = Firebase.getString("led4");
          if (fireLed4 == "ON") {                     
            Serial.println(" Led4-ON");                 
            digitalWrite(led4, HIGH);                                                        
          } else {             
            Serial.println(" Led4-OFF");
            digitalWrite(led4, LOW);                                                       
          }
        }  
}
