//#include <ESP8266WiFi.h>  //esp8266
#include "WiFi.h"  //esp32
//#include <ESP8266HTTPClient.h>  //esp8266
#include <HTTPClient.h>  //esp32
#include <WiFiClientSecure.h>

//SSID and PASSWORD of your WiFi network.
//const char* ssid = "Prastzy.net"; 
//const char* password = "123456781"; 
const char* ssid = "bebas";  
const char* password = "akunulisaja"; 

// Google script Web_App_URL.
String Web_App_URL = "https://script.google.com/macros/s/AKfycbxRqOrkjT4KhrZAiD2FpJUqVwve891eW8uHgDBFx05zkyYTsWXLalb1BriJ3ZqMKu77aw/exec";

int data1, data2;
String status1, status2;

void Data() {
  data1 = random(1, 101);
  data2 = random(1, 101);
  if (data1 > 50){
    status1 = "HIGH";
  }else{
    status1 = "LOW";
  }
  if (data2 > 50){
    status2 = "HIGH";
  }else{
    status2 = "LOW";
  }
}

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  Serial.println();
  delay(1000);

  //Set Wifi to STA mode
  Serial.println();
  Serial.println("-------------");
  Serial.println("WIFI mode : STA");
  WiFi.mode(WIFI_STA);
  Serial.println("-------------");

  //Connect to Wi-Fi (STA).
  Serial.println();
  Serial.println("------------");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  int connecting_process_timed_out = 20; //--> 20 = 20 seconds.
  connecting_process_timed_out = connecting_process_timed_out * 2;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    if (connecting_process_timed_out > 0) connecting_process_timed_out--;
    if (connecting_process_timed_out == 0) {
      delay(1000);
      ESP.restart();
    }
  }

  digitalWrite(LED_BUILTIN, HIGH);
  
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("------------");
  
  delay(1000);
}

void loop() {

  Data();
  
  // This condition is the condition for sending or writing data to Google Sheets.
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);

    // Create a URL for sending or writing data to Google Sheets.
    String Send_Data_URL = Web_App_URL + "?sts=write";
    Send_Data_URL += "&data1=" + String (data1);
    Send_Data_URL += "&data2=" + String(data2);
    Send_Data_URL += "&status1=" + status1;
    Send_Data_URL += "&status2=" + status2;

    Serial.println();
    Serial.println("-------------");
    Serial.println("Send data to Google Spreadsheet...");
    Serial.print("URL : ");
    Serial.println(Send_Data_URL);

      // Initialize HTTPClient as "http".
      WiFiClientSecure client;
      client.setInsecure(); // Nonaktifkan validasi SSL
      HTTPClient http;
  
      // HTTP GET Request using the new API
      http.begin(client, Send_Data_URL);  // Use client and URL
      http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  
      // Gets the HTTP status code.
      int httpCode = http.GET(); 
      Serial.print("HTTP Status Code : ");
      Serial.println(httpCode);
  
      // Getting response from google sheets.
      String payload;
      if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload : " + payload);    
      }
      
      http.end();
    
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("-------------");
  }

  delay(5000);
}
