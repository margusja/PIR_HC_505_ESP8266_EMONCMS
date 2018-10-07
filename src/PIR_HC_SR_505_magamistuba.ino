#include <ESP8266WiFi.h>
#include <Base64.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMulti.h>

#define INPUT_PIN 2

// EMONCMS
#define HOST "http://192.168.1.248/emoncms"
#define NODE "PIR1"
#define APIKEY "101673a46ef99df676d3660a3b30ea39"

//AP definitions
#define AP_SSID "varmar"
#define AP_PASSWORD "xxxxxxx"

ESP8266WiFiMulti WiFiMulti;

int oldInputState;

void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to WiFi network
  wifiConnect();

  pinMode(INPUT_PIN, INPUT);
  digitalWrite(INPUT_PIN, LOW); // turns on pulldown. At least in arduino-uno board
}

void loop() {
  int inputState = digitalRead(INPUT_PIN);
  //Serial.println(inputState);

  if (inputState != oldInputState) {
    Serial.println(inputState);
    sendInputState(inputState);
    oldInputState = inputState;
  }

}

void sendInputState(int inputState)
{

  if((WiFiMulti.run() == WL_CONNECTED)) {
   HTTPClient http;

  String url;

  Serial.print("Send to emon: ");
  Serial.println(inputState);

  url = String(HOST) + "/input/post.json?node="+ String(NODE) +"&csv={movement:"+ String(inputState) +"}&apikey="+String(APIKEY);

  http.begin(url);
  int httpCode = http.GET();
        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                Serial.println(payload);
            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

    http.end();
    delay(10000);
  }
}


void wifiConnect()
{
    Serial.print("Connecting to AP");
    WiFi.begin(AP_SSID, AP_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
}
