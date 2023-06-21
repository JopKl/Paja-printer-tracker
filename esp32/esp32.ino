#include <WiFi.h>

#include <Arduino_JSON.h>

//#include <GroveEncoder.h>

#include <HTTPClient.h>

// Set DEBUG to 0 when deploying
#define DEBUG 1

#if DEBUG
  // Define test parameters here
  #define ssid "Pidgeon"
  #define password "batteryneeded"
  #define serverName "http://82.130.26.178:3000/posts"
  #define DELAY 5000
  
  #define D_SerialBegin(...) Serial.begin(__VA_ARGS__)
  #define D_print(...)       Serial.print(__VA_ARGS__)
  #define D_println(...)     Serial.println(__VA_ARGS__)
#else
  #define ssid "aalto open"
  #define password ""
  #define serverName "http://vilhonpaja.aalto.fi/api/3Dprinters";
  #define DELAY 60000
  
  #define D_SerialBegin(bauds)
  #define D_print(...)
  #define D_println(...)
#endif

// Printer states
#define pr_fail 0
#define pr_done 1
#define pr_printing 2

// Button mapping
const int b_printing = 25;
const int b_done = 26;
const int b_fail = 27;

// Led mapping
const uint8_t l_red = 15;
const uint8_t l_yellow = 16;
const uint8_t l_green = 21;
const uint8_t l_blue = 22;

// Buffer to store Mac addresss
byte mac[6];

unsigned long lastTime = 0;
const  unsigned long timerDelay = DELAY;

int httpOk = 201;

String status_begin = "{\"printer id\":\"";
String status_end_ping ="\",\"status\":\"Ping\"}";
String status_end_fail ="\",\"status\":\"FAIL\"}";
String status_end_done ="\",\"status\":\"DONE\"}";
String status_end_printing="\",\"status\":\"PRINTING\"}";
String mac_addr;
char message[64];

//enum Printer_status{pr_fail, pr_done, pr_printing};


volatile uint8_t pr_status = pr_done;
volatile uint8_t new_status_ready = true;

void setup() {
    pinMode(b_fail, INPUT_PULLUP);
    pinMode(b_done, INPUT_PULLUP);
    pinMode(b_printing, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(b_fail),fail_pressed,CHANGE);
    attachInterrupt(digitalPinToInterrupt(b_done),done_pressed,CHANGE);
    attachInterrupt(digitalPinToInterrupt(b_printing),printing_pressed,CHANGE);

    pinMode(l_red, OUTPUT);
    pinMode(l_yellow, OUTPUT);
    pinMode(l_green, OUTPUT);
    pinMode(l_blue, OUTPUT);
    
    D_SerialBegin(115200);
     WiFi.begin(ssid, password);
    D_println("Connecting");
    while(WiFi.status() != WL_CONNECTED) {
      delay(500);
      D_print(".");
    }
    D_println("");
    D_print("Connected to WiFi network with IP Address: ");
    D_println(WiFi.localIP());
    WiFi.macAddress(mac);
    mac_addr = mac2String(mac);

    // Ping the server during boot
    WiFiClient client;
    HTTPClient http;
    http.begin(client, serverName);  
    http.addHeader("Content-Type", "application/json");
    String temp = status_begin + mac_addr + status_end_ping;
    temp.toCharArray(message, temp.length());
    D_println(temp);
    int httpResponseCode = http.POST(temp);

    //retry until a connection to the server is established
    while (httpResponseCode != httpOk){
      httpResponseCode = http.POST(temp);
      delay(3000);
    }

    // If connected to server, blink 3 times
    if (httpResponseCode = httpOk){
        blink_l_blue();
    } 
    done_pressed()
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
    
      http.begin(client, serverName);  
      http.addHeader("Content-Type", "application/json");
      String temp = status_begin + mac_addr + status_end_ping;
      temp.toCharArray(message, temp.length());
      D_println(temp);
      int httpResponseCode = http.POST(temp);
  
      D_print("HTTP Response code: ");
      D_println(httpResponseCode);
      D_print("state: ");
      D_println(pr_status);
//      if (new_status_ready){
      if (httpResponseCode == httpOk && new_status_ready){
        int statusResponse = send_status(http);
        
        if(statusResponse == httpOk){
         new_status_ready = false;
         D_println("setting send status to false");
       }
      D_print("HTTP Response code: ");
      D_println(statusResponse);
      }
      // Free resources
      http.end();
    }
    else {
      D_println("WiFi Disconnected");
    }
    lastTime = millis();
  }
  
}

// Blink 3 times
void blink_l_blue(){
  for (int i = 0; i<3; i++){
    digitalWrite(l_blue, HIGH);
    delay(500);
    digitalWrite(l_blue, LOW);
    delay(500);
  }
}

void led_on(const int led_num){
  switch (led_num){
    case l_red:
      digitalWrite(l_red, HIGH);
      digitalWrite(l_yellow, LOW);
      digitalWrite(l_green, LOW);
      break;
    case l_yellow:
      digitalWrite(l_yellow, HIGH);
      digitalWrite(l_red, LOW);
      digitalWrite(l_green, LOW);
      break;
    case l_green:
      digitalWrite(l_green, HIGH);
      digitalWrite(l_red, LOW);
      digitalWrite(l_yellow, LOW);
      break;
  }
}

String mac2String(byte ar[]){
  String s;
  for (byte i = 0; i < 6; ++i)
  {
    char buf[3];
    sprintf(buf, "%02X", ar[i]);
    s += buf;
    if (i < 5) s += ':';
  }
  return s;
}

int send_status(HTTPClient &http){
  String temp = status_begin + mac_addr;
  switch(pr_status){
    case pr_fail:
      temp += status_end_fail;
      break;
    case pr_done:
      temp += status_end_done;
      break;
    case pr_printing:
      temp += status_end_printing;
      break;
  }
  return http.POST(temp);
}

// Interrupt functions
void fail_pressed(){
  pr_status = pr_fail;
  new_status_ready = true;
  digitalWrite(l_red, HIGH);
  digitalWrite(l_yellow, LOW);
  digitalWrite(l_green, LOW);
}
void done_pressed(){
  pr_status = pr_done;
  new_status_ready = true;
  digitalWrite(l_yellow, HIGH);
  digitalWrite(l_red, LOW);
  digitalWrite(l_green, LOW);
}
void printing_pressed(){
  pr_status = pr_printing;
  new_status_ready = true;
  digitalWrite(l_green, HIGH);
  digitalWrite(l_red, LOW);
  digitalWrite(l_yellow, LOW);
}
