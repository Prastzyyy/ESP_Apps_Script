//Program ESP_Spreadsheet Iklim Mikro

//Link Google SpreadSheet : 
//https://docs.google.com/spreadsheets/d/1IKQU8h4dVo9KKv69Chr1sNzfcQOmlh4LeF0Fe2pe_4k/edit?gid=0#gid=0


#include <ESP8266WiFi.h>  //esp8266
//#include "WiFi.h"  //esp32
#include <ESP8266HTTPClient.h>  //esp8266
//#include <HTTPClient.h>  //esp32
#include <WiFiClientSecure.h>
#include <Ticker.h>

//SSID and PASSWORD of your WiFi network.
const char* ssid = "Prastzy.net"; 
const char* password = "123456781"; 
//const char* ssid = "bebas";  
//const char* password = "akunulisaja"; 

// Google script Web_App_URL.
String Web_App_URL = "https://script.google.com/macros/s/AKfycbx04KLLZtxDsp9RVjLuC-3c5A7zPKfCoaw2g-ar50Yl3OOasGsMmxxlw2cKV1JIXo6C/exec";


//Data Monitoring
int ldrValue;
float temperature, humidity;
String kipas, sprayer;
void setup_wifi();

Ticker task1;
void Data();
//Ticker task2;
void spreadsheet();

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  Serial.println();
  delay(1000);
  setup_wifi();
  delay(1000);
  
  task1.attach(2, Data);
  //task2.attach(5, spreadsheet);
}

void loop() {
  spreadsheet();
  delay(5000);
}

void spreadsheet(){
  // This condition is the condition for sending or writing data to Google Sheets.
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);

    // Create a URL for sending or writing data to Google Sheets.
    String Send_Data_URL = Web_App_URL + "?sts=write";
    Send_Data_URL += "&ldrValue=" + String (ldrValue);
    Send_Data_URL += "&temperature=" + String(temperature);
    Send_Data_URL += "&humidity=" + String(humidity);
    Send_Data_URL += "&kipas=" + kipas;
    Send_Data_URL += "&sprayer=" + sprayer;

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
}

void Data(){
  ldrValue = random(0, 1024);
  temperature = random(20, 41);
  humidity = random(60, 91);

  if (temperature > 30){
    kipas = "Menyala";
  }else{
    kipas = "Padam";
  }
  if (humidity < 75){
    sprayer = "Menyala";
  }else{
    sprayer = "Padam";
  }
}

void setup_wifi(){
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
}
