/**

	Get COVID-19 Corona REST API data with the ESP32 microcontroller

	Code example; for more information see:
	https://oneguyoneblog.com/2020/03/10/covid-19-corona-tracker-esp32-arduino-ide/


 **/


#include <M5StickC.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* url = "https://services1.arcgis.com/0MSEUqKaxRlEPj5g/arcgis/rest/services/Coronavirus_2019_nCoV_Cases/FeatureServer/1/query?where=Country_Region%20like%20'%25NETHERLANDS%25'&outFields=Last_Update,Confirmed,Deaths,Recovered&returnGeometry=false&outSR=4326&f=json";

const char* ssid = "yourssid";
const char* password = "yourpassword";

const int redLED = 10;

void setup() {

  pinMode(redLED, OUTPUT);
  Serial.begin(115200);
  delay(2000);

  // initialize the M5StickC object
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);

  digitalWrite(redLED, LOW);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 10);
    M5.Lcd.printf("Connecting..");
  }

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 10);
  M5.Lcd.printf("Connected");
  digitalWrite(redLED, HIGH);
}

// the loop routine runs over and over again forever
void loop() {

  HTTPClient http;
  String data;

  digitalWrite(redLED, LOW);
  http.begin(url);
  digitalWrite(redLED, HIGH);

  int httpCode = http.GET();

  if (httpCode > 0) { //Check for the returning code

    String payload = http.getString();
char charBuf[500];
payload.toCharArray(charBuf, 500);
    
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 10);

    Serial.println(payload);

    const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(4) + JSON_OBJECT_SIZE(1) + 2 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + 3 * JSON_OBJECT_SIZE(6) + 2 * JSON_OBJECT_SIZE(7) + 690;
    DynamicJsonDocument doc(capacity);

    deserializeJson(doc, payload);

    JsonArray fields = doc["fields"];

    JsonObject features_0_attributes = doc["features"][0]["attributes"];

    long features_0_attributes_Last_Update = features_0_attributes["Last_Update"];
    int features_0_attributes_Confirmed = features_0_attributes["Confirmed"];
    int features_0_attributes_Deaths = features_0_attributes["Deaths"];
    int features_0_attributes_Recovered = features_0_attributes["Recovered"];

    M5.Lcd.setTextSize(4);
    M5.Lcd.setTextColor(ORANGE);
    M5.Lcd.println(features_0_attributes_Confirmed);

    M5.Lcd.setTextSize(3);
    M5.Lcd.setTextColor(RED);
    M5.Lcd.println(features_0_attributes_Deaths);
    
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.println(features_0_attributes_Recovered);
    
  } else {

    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 10);
    M5.Lcd.printf("Error on HTTP request");
  }

  http.end(); //Free the resources

  delay(60000);
}
