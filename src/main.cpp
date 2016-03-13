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
// URL Constants
const char* URL_ROOT = "/";
const char* URL_ARGS = "/args";
const char* URL_DISPLAY = "/display";
// URL Parameters
const char* PARAM_FRAME_BUFFER = "framebuffer";
const char* PARAM_DISPLAY_BRIGHTNESS = "brightness";

const uint8_t displayWidth = 18;
const uint8_t displayHeight = 8;
const uint16_t ledCount = displayWidth * displayHeight;
uint8_t displayBrightness = 3;
rgb_color colors[ledCount];
// Defaults
rgb_color brushColor = (rgb_color){0,0,0}; // Default brush color is black

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


uint8_t r(uint8_t exclusiveMax){
    return rand() % exclusiveMax;
}


void randomizeDisplay(){
    for(int i=0;i<ledCount;i++){
        colors[i]={r(255), r(255), r(255)};
    }
}

uint8_t hexToInt(String twoChars){
    return (uint8_t) strtoul(twoChars.c_str(),NULL,16);
}

String intToHex(uint8_t value){
    String result = "xX";
//    std::ostringstream stream;
//    stream << std::hex << value;
//    return stream.str();
    return result;
}

void setBrushColor(rgb_color color){
 brushColor = color;
}

void setBrushColor(uint8_t r,uint8_t g,uint8_t b){
 brushColor = {r,g,b};
}

void setBrushColor(String colorAsString){
    uint8_t r,g,b;
    r= hexToInt(colorAsString.substring(0,1));
    g= hexToInt(colorAsString.substring(2,3));
    b= hexToInt(colorAsString.substring(4,5));
    setBrushColor(r,g,b);
}


void setRandomBrushColor(){
    setBrushColor(r(255),r(255),r(255));
}

void drawPixel(uint8_t x, uint8_t y){
    if(x >= displayWidth || y >= displayHeight) return; // Drawing outside disp.
    if(y % 2 == 0){// Display is a zigzaged LED strip
    colors[y * displayWidth + x] = brushColor;
    } else {
    colors[(y+1) * displayWidth -1 - x] = brushColor;
    }
}

rgb_color readPixel(uint8_t x, uint8_t y){
    // If user reading outside of display
    if(x >= displayWidth || y >= displayHeight) return (rgb_color) {256,256,256};
    if(y % 2 == 0){// Display is a zigzaged LED strip
    return colors[y * displayWidth + x];
    } else {
    return colors[(y+1) * displayWidth -1 - x];
    }
}

String colorToString(rgb_color color){
    String result = "";
    result += intToHex(color.red);
    result += intToHex(color.green);
    result += intToHex(color.blue);
    return  result;
}

void updateDisplay(){
      ledStrip.write(colors, ledCount, displayBrightness);
}

void drawRect(uint8_t minX, uint8_t minY, uint8_t width, uint8_t height){
    uint8_t maxX = minX + width;
    uint8_t maxY = minY + height;
    for(int y = minY; y <= maxY; y++){
        if(y == minY || y == maxY ){ // only fill if top or bottom row
            for(int x = minX; x <= maxX; x++){
                drawPixel(x,y);
            }
        } else {
            drawPixel(minX,y);
            drawPixel(maxX,y);
        }
    }
}

void drawSolidRect(uint8_t minX, uint8_t minY, uint8_t width, uint8_t height){
    uint8_t maxX = minX + width;
    uint8_t maxY = minY + height;
    for(int y = minY; y <= maxY; y++){
        for(int x = minX; x <= maxX; x++){
            drawPixel(x,y);
        }
    }
}


String getArgsInfo(){
    String response = "";
    for(int i = 0; i < server.args(); i++){
        response += server.argName(i) + ": " + server.arg(i) + "\n";    
    }
    return response;
}

void setFrameBuffer(String framebuffer){
    const unsigned int pixelLength = 6; 
    unsigned int index;
    String colorStr = "";
    int strLen = framebuffer.length();
    for(int y = 0; y < displayHeight; y++){
        for(int x = 0; x < displayWidth; x++){
            index = pixelLength * (y * displayWidth + x) ; 
            if(index + pixelLength <= strLen){
                colorStr = framebuffer.substring(index,index + pixelLength);
                setBrushColor(colorStr);
                drawPixel(x,y);
            } else {
                setBrushColor(0,0,0);
                drawPixel(x,y);
            }
        }
    }
}

void setDisplayBrightness(String brightness){
    displayBrightness = atoi( brightness.c_str() );
    updateDisplay();
}

void handleDisplayRequest(){
    if(server.hasArg(PARAM_FRAME_BUFFER)){
        setFrameBuffer(server.arg(PARAM_FRAME_BUFFER));
    }
    if(server.hasArg(PARAM_DISPLAY_BRIGHTNESS)){
        setDisplayBrightness(server.arg(PARAM_DISPLAY_BRIGHTNESS));
    }
}

String getFrameBufferString(){
    String frameBufferString = "";
    for(int y = 0; y < displayHeight; y++){
        for(int x = 0; x < displayWidth; x++){
            frameBufferString += colorToString(readPixel(x,y));
        }
    }
    return frameBufferString;
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
  
  server.on(URL_ROOT, [](){
    server.send(200, "text/plain", "Accessed " + String(++counter) + " times.");
  });

  server.on(URL_ARGS, [](){
    server.send(200, "text/plain", getArgsInfo());
  });

  server.on(URL_DISPLAY, [](){
  server.send(200, "text/plain");
  handleDisplayRequest();    
  });
  server.begin();
}


void loop() {
  ArduinoOTA.handle();
  server.handleClient();
  //clearDisplay();
//  setRandomBrushColor();
//  if(r(100) > 80){
//    drawSolidRect(r(18),r(7),r(18),r(7));
//  } else {
//    drawRect(r(18),r(7),r(18),r(7));
//  }
//  setBrushColor(255,0,0);
//  drawRect(1,1,2,2);
//  setBrushColor(0,255,0);
//  drawSolidRect(5,4,3,2);
//  setBrushColor(45,88,230);
//  drawPixel(0,0);
//  drawPixel(17,0);
//  drawPixel(0,7);
//  drawPixel(17,7);
  //drawSolidRect(0,5,7,10);
  //randomizeDisplay();
  updateDisplay();
  //delay(10);
}