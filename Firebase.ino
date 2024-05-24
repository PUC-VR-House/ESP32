#include <WiFi.h>
#include <FirebaseESP32.h>
#include <ArduinoJson.h>
#include <Adafruit_AHTX0.h>  
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>   

// WIFI - FIREBASE - SETTINGS
#define FIREBASE_HOST "https://teste-casa-4d9fb-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "AIzaSyBSY4AEXaK-DsLq3CMcKzZFxWkSMdcbMgw"
#define WIFI_SSID "motorafael"
#define WIFI_PASSWORD "1234567890"

// SCREEN
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//AHTX10 - HUM - TEMP
Adafruit_AHTX0 aht;
Adafruit_Sensor *aht_humidity, *aht_temp;
float temperature = 0.0;
float humidity = 0.0;
float aht_old_temp;
float aht_old_humidity;

// Firebase Data
FirebaseData firebaseData;
FirebaseConfig config;
FirebaseAuth auth;

//SERVO
Servo s1;
String door;  
int pos = 0; 
int x;

// LEDs
bool L1 = false;
bool L2 = false;
bool L3 = false;
bool L4 = false;
bool L5 = false;
bool L6 = false;
bool DOOR = false;

void setup()
{
  Serial.begin(115200);
  delay(500);

  // OUTPUTS
    pinMode(27, OUTPUT);
    pinMode(14, OUTPUT);
    pinMode(26, OUTPUT);
    pinMode(25, OUTPUT);
    pinMode(33, OUTPUT);
    pinMode(32, OUTPUT);

  //SERVO
    s1.attach(15);
    // Initially the servo must be stopped 
    s1.write(90);

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

  // SETING DISPLAY
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    { // Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.print("Hello, world!");
    delay(100);
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("PUC MINAS:");
    delay(500);
    display.println("ENG.COMP - JOGOS.DIGT");
    delay(1000);
    display.display();

  // WIFI
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.println("Connecting to ");
    display.println("Connecting to" + String(WIFI_SSID));
    delay(1000);
    display.display();
    Serial.println(WIFI_SSID);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        display.print(".");
        display.display();
        delay(500);
    }
    Serial.println();
    display.print("Wifi - Connected");
    display.display();
    Serial.println("Connected to ");
    Serial.println(WIFI_SSID);
    Serial.println("IP Address is : ");
    Serial.println(WiFi.localIP());
    delay(500);

  // FIREBASE
    config.host = FIREBASE_HOST;
    config.signer.tokens.legacy_token = FIREBASE_AUTH;
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
}

void loop()
{
    display.clearDisplay();
    display.setCursor(0, 10);
    display.setTextSize(2);
    // Lê o JSON do Firebase
      if (Firebase.getJSON(firebaseData, "/ATUADORES"))
      {
          if (firebaseData.dataType() == "json")
          {
              // Obter a string JSON
              String jsonString;
              firebaseData.jsonObject().toString(jsonString, true);
              FirebaseJson &json = firebaseData.jsonObject();
              // Printar a string JSON
              Serial.println("JSON recebido:");
              Serial.println(jsonString);

              // Decodificar o JSON e acessar os valores das chaves como booleanos
              StaticJsonDocument<256> doc;
              DeserializationError error = deserializeJson(doc, jsonString);
              if (error)
              {
                  Serial.print("Falha ao decodificar JSON: ");
                  Serial.println(error.c_str());
                  return;
              }

              L1 = doc["L1"];
              L2 = doc["L2"];
              L3 = doc["L3"];
              L4 = doc["L4"];
              L5 = doc["L5"];
              L6 = doc["L6"];
              DOOR = doc["DOOR"];
              Serial.println("Valores das chaves:");
              Serial.println(" L1:" + String(L1) + " L2:" + String(L2) + " L3:" + String(L3) + " L4:" + String(L4) + " L5:" + String(L5) + " L6:" + String(L6) + " DOOR:" + DOOR);
          }
      }
      else
      {
          Serial.print("Erro ao obter JSON: ");
          Serial.println(firebaseData.errorReason());
      }
    //DOOR CONTROL 
       if (door != String(DOOR)) {
          door = DOOR;  
          Serial.println("Spin");       
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

    //LED CONTROL 
      if(L1 == true) {
        digitalWrite(14, HIGH); 
      } else {
        digitalWrite(14, LOW); 
      }
      if(L2 == true) {
        digitalWrite(27, HIGH); 
      } else {
        digitalWrite(27, LOW); 
      }
      if(L3 == true) {
        digitalWrite(26, HIGH); 
      } else {
        digitalWrite(26, LOW); 
      }
      if(L4 == true) {
        digitalWrite(25, HIGH); 
      } else {
        digitalWrite(25, LOW); 
      }
      if(L5 == true) {
        digitalWrite(33, HIGH); 
      } else {
        digitalWrite(33, LOW); 
      }
      if(L6 == true) {
        digitalWrite(32, HIGH); 
      } else {
        digitalWrite(32, LOW); 
      }

    
    //CHECK TEMP HUM DATA
      sensors_event_t humidity;
      sensors_event_t temp;
      aht_humidity->getEvent(&humidity);
      aht_temp->getEvent(&temp);
      Serial.println("-Sensor terperature: " + String(temp.temperature));
      Serial.println("-Sensor humidity: " + String(humidity.relative_humidity));

      if (int(aht_old_temp) != int(temp.temperature) || (int(aht_old_humidity)/10) != (int(humidity.relative_humidity)/10)) {
          StaticJsonDocument<200> doc;
          doc["temp"] = String(temp.temperature); // exemplo de valor de temperatura como string
          doc["hum"] = String(humidity.relative_humidity);  // exemplo de valor de umidade como string

          FirebaseJson json;
          String jsonString;
          serializeJson(doc, jsonString);
          json.setJsonData(jsonString);

          // Enviar o JSON para o Firebase
          if (Firebase.setJSON(firebaseData, "/INPUTS", json)) {
              Serial.println("JSON enviado com sucesso!");
          } else {
              Serial.print("Erro ao enviar JSON: ");
              Serial.println(firebaseData.errorReason());
          }

          Serial.println("-Updated temperature and humidity");
      }

    aht_old_temp = temp.temperature;
    aht_old_humidity = humidity.relative_humidity;
    display.println("TEMP" + String(int(temp.temperature)) + "C");
    display.println("HUM" + String(int(humidity.relative_humidity)) + "%");
    display.display();
    delay(1000);
}
