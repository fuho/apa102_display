#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <APA102.h>

const char* ssid = "N16FU";
const char* password = "masdostfest";
const uint8_t clockPin = D5;
const uint8_t dataPin = D7;

const uint8_t displayWidth = 18;
const uint8_t displayHeight = 8;
const uint16_t ledCount = displayWidth * displayHeight;
rgb_color brushColor = (rgb_color){0,0,0}; // Default brush color is black
rgb_color colors[ledCount];
const uint8_t brightness = 1;

ESP8266WebServer server(80);
APA102<dataPin, clockPin> ledStrip;
int counter = 0; // Server access count



// Converts a color from HSV to RGB.
// h is hue, as a number between 0 and 360.
// s is the saturation, as a number between 0 and 255.
// v is the value, as a number between 0 and 255.
rgb_color hsvToRgb(uint16_t h, uint8_t s, uint8_t v)
{
    uint8_t f = (h % 60) * 255 / 60;
    uint8_t p = (255 - s) * (uint16_t)v / 255;
    uint8_t q = (255 - f * (uint16_t)s / 255) * (uint16_t)v / 255;
    uint8_t t = (255 - (255 - f) * (uint16_t)s / 255) * (uint16_t)v / 255;
    uint8_t r = 0, g = 0, b = 0;
    switch((h / 60) % 6){
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
    }
    return (rgb_color){r, g, b};
}

void clearDisplay(){
    for(int i=0;i<ledCount;i++){
        colors[i]={0,0,0};
    }
}

void randomizeDisplay(){
    for(int i=0;i<ledCount;i++){
        colors[i]={rand() % 255, rand() % 255, rand() % 255};
    }
}

void setBrushColor(rgb_color color){
 brushColor = color;
}

void setBrushColor(uint8_t r,uint8_t g,uint8_t b){
 brushColor = {r,g,b};
}

void drawPixel(uint8_t x, uint8_t y){
    if(x >= displayWidth || y >= displayHeight) return; // Drawing outside disp.
    colors[y * displayWidth + x] = brushColor;
}

void updateDisplay(){
      ledStrip.write(colors, ledCount, brightness);
}

void drawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height){
    uint8_t maxX = x + width;
    uint8_t maxY = y + height;
    for(int y = 0; y <= maxY; y++){
        if(y == 0 || y == maxY ){ // only fill if top or bottom or req
            for(int x = 0; x <= maxX; x++){
                drawPixel(x,y);
            }
        } else {
            drawPixel(x,y);
            drawPixel(maxX,y);
        }
    }
}

void drawSolidRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height){
    uint8_t maxX = x + width;
    uint8_t maxY = y + height;
    for(int y = 0; y <= maxY; y++){
        for(int x = 0; x <= maxX; x++){
            drawPixel(x,y);
        }
    }
}

void setup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  ArduinoOTA.setPort(8266);
  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("esp8266fu");
  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");
  ArduinoOTA.begin();
  
  server.on("/", [](){
    server.send(200, "text/plain", "Accessed " + String(++counter) + " times.");
  });
  server.begin();
}


void loop() {
  ArduinoOTA.handle();
  server.handleClient();
  clearDisplay();
  setBrushColor(255,255,255);
  drawRect(1,1,16,6);
  //setBrushColor(187,255,19);
  //drawSolidRect(0,5,7,10);
  //randomizeDisplay();
  updateDisplay();
  delay(30);
}