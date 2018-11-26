////
/// BlynkSlider - main file.
//

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
//#include <BlynkSimpleEsp32_BLE.h>
//#include <BLEDevice.h>
//#include <BLEServer.h>
#include <ArduinoOTA.h>

#include "LedDriver.h"
#include "Credientals.h"

static uint32_t prefsSaveTick;
int yellowSlider = 0;
int whiteSlider = 0;
enum LedsPowerState powerState = Off;


BLYNK_CONNECTED(){
  Serial.println("Blynk connected");
  Blynk.setProperty(YellowSlider, "min", LEDC_MINVALUE);
  Blynk.setProperty(WhiteSlider, "min", LEDC_MINVALUE);
  Blynk.setProperty(YellowSlider, "max", LEDC_MAXVALUE);
  Blynk.setProperty(WhiteSlider, "max", LEDC_MAXVALUE);
  Blynk.setProperty(OffButtonPin, "offBackColor", "#000000");
  Blynk.setProperty(OffButtonPin, "onBackColor", "#ffffff");
}


BLYNK_WRITE(YellowSlider)
{
  int pinValue = param.asInt();
  if (yellowSlider == pinValue) return;

  Serial.printf("Yellow: %d->%d\r\n", yellowSlider, pinValue);
  yellowSlider = pinValue;
  if (powerState == On) {
    setBrightness(yellowSlider, YellowChannel);
  }
}

BLYNK_WRITE(WhiteSlider)
{
  int pinValue = param.asInt();
  if (whiteSlider == pinValue) return;

  Serial.printf("White: %d->%d\r\n", whiteSlider, pinValue);
  whiteSlider = pinValue;
  if (powerState == On) {
    setBrightness(whiteSlider, WhiteChannel);
  }
}

BLYNK_WRITE(NightButtonPin)
{
  int pinValue = param.asInt();

  Serial.print("NighButton: ");
  Serial.println(pinValue);

  if (pinValue == 1) {
    powerState = Night;
    digitalWrite(NightLedPin, HIGH);
    Blynk.virtualWrite(OnButtonPin, 0);
    Blynk.virtualWrite(OffButtonPin, 0);
    ledsOff();
  }
}

BLYNK_WRITE(OnButtonPin)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable

  Serial.print("OnButton: ");
  Serial.println(pinValue);

  if (pinValue == 1) {
    powerState = On;
    digitalWrite(NightLedPin, LOW);
    Blynk.virtualWrite(OffButtonPin, 0);
    Blynk.virtualWrite(NightButtonPin, 0);
    ledsRestore();
  }
}

BLYNK_WRITE(OffButtonPin)
{
 int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable

  Serial.print("OffButtonPin: ");
  Serial.println(pinValue);
  
  if (pinValue == 1) {
    powerState = Off;
    Blynk.virtualWrite(OnButtonPin, 0);
    Blynk.virtualWrite(NightButtonPin, 0);
    digitalWrite(NightLedPin, LOW);
    ledsOff();
  }
}


void setup()
{
  // Debug console
  Serial.begin(115200);

  ledSetup();

  restorePrefs();

  WiFi.mode(WIFI_STA);
  //  Blynk.setDeviceName("Blynk");

  // Blynk.begin(blynkAuth);
  // Blynk.begin(blynkAuth, ssid, password);
 
  Blynk.begin(blynkAuth, ssid, password, "userv.bdm", 8080);


  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);
  // Hostname defaults to esp3232-[MAC]
  ArduinoOTA.setHostname(ModuleHostname);

  // No authentication by default
  // ArduinoOTA.setPassword("admin");
  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
//      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Blynk.virtualWrite(YellowSlider, yellowSlider);
  Blynk.virtualWrite(WhiteSlider, whiteSlider);

  prefsSaveTick = millis() / PrefsSaveTimeout;
}

void loop()
{
  Blynk.run();
  ArduinoOTA.handle();
  uint32_t tick = millis() / PrefsSaveTimeout;
  if (tick != prefsSaveTick) {
    prefsSaveTick = tick;
    savePrefs();
  }
}
