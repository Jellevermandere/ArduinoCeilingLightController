#include "FastLED.h"
#include <SPI.h>
#include <WiFiNINA.h>
#include "secrets.h" 
#include "webpage.h"

// RGBLLED stuff
bool useRGB = true;
bool previewRGB = false;
#define DATA_PIN 9        // Serial data pin
#define COLOR_ORDER GRB   // It's GRB for WS2812B and BGR for APA102
#define LED_TYPE WS2812B  // What kind of strip are you using (APA102, WS2801 or WS2812B)?
#define NUM_LEDS 280
uint8_t max_bright = 64;  
struct CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;


//WhiteLED stuff
bool useWhite = true;
#define WARM_WHITE_PIN 10
#define COOL_WHITE_PIN 11
int currentBrightness = 0;
int currentTemperature = 0;

//server stuff
bool useServer = true;
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(80);

//Serial Stuff
bool useSerial = true;

#define TEMP_ANALOG_IN A1
#define BRIGHT_ANALOG_IN A2

void setup() {
  Serial.begin(115200);
  
  if (useWhite) SetUpWhite();
  if (useRGB) SetUpRGB(); 
  if (useServer) SetUpServer();

  Serial.println("<Arduino is ready>");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(useSerial) ControlWithSerial();
  if(useServer) LoopServer();
}
