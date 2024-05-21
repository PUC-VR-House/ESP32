#include <WiFi.h>
#include <FirebaseESP32.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// WIFI - FIREBASE - SETTINGS
#define FIREBASE_HOST "https://teste-casa-4d9fb-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "AIzaSyBSY4AEXaK-DsLq3CMcKzZFxWkSMdcbMgw"
#define WIFI_SSID "2GClaroAp202"
#define WIFI_PASSWORD "vlmrap202"

// SCREEN
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// DHT Sensor
#define DHT_SENSOR_PIN 12
#define DHT_SENSOR_TYPE DHT11
DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

// Firebase Data
FirebaseData firebaseData;
FirebaseConfig config;
FirebaseAuth auth;

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

    // DHT Sensor
    dht_sensor.begin();

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

    // read humidity
    int humi = trunc(dht_sensor.readHumidity());
    // read temperature in Celsius
    int tempC = trunc(dht_sensor.readTemperature());
    // read temperature in Fahrenheit
    int tempF = trunc(dht_sensor.readTemperature(true));
    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.print("%");
    Serial.print("  |  ");
    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C  ~  ");
    Serial.print(tempF);
    Serial.println("°F");
    display.println("TEMP " + String(tempC) + "C");
    display.println("HUM " + String(humi) + "%");
    display.display();
    delay(1000);
}
