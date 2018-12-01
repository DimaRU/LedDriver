////
/// LedDriver - main file.
//

#ifdef TEST_CONFIG
  #define BLYNK_PRINT Serial
#endif

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
//#include <BlynkSimpleEsp32_BLE.h>
//#include <BLEDevice.h>
//#include <BLEServer.h>
#include <ArduinoOTA.h>
#include "esp_system.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "apps/sntp/sntp.h"

#include "LedDriver.h"
#include "Credientals.h"


int yellowLevel = SLIDER_MAXVALUE /2;
int whiteLevel = SLIDER_MAXVALUE /2;
enum LedsPowerState powerState = On;

BlynkTimer timer;

static void WiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info);

BLYNK_CONNECTED(){
  Serial.println("Blynk connected");
  Blynk.setProperty(YellowSliderPin, "min", SLIDER_MINVALUE);
  Blynk.setProperty(YellowSliderPin, "max", SLIDER_MAXVALUE);
  Blynk.setProperty(WhiteSliderPin, "min", SLIDER_MINVALUE);
  Blynk.setProperty(WhiteSliderPin, "max", SLIDER_MAXVALUE);

  Blynk.setProperty(AlarmYellowSliderPin, "min", SLIDER_MINVALUE);
  Blynk.setProperty(AlarmYellowSliderPin, "max", SLIDER_MAXVALUE);
  Blynk.setProperty(AlarmWhiteSliderPin, "min", SLIDER_MINVALUE);
  Blynk.setProperty(AlarmWhiteSliderPin, "max", SLIDER_MAXVALUE);

  Blynk.setProperty(OffButtonPin, "offBackColor", "#000000");
  Blynk.setProperty(OffButtonPin, "onBackColor", "#ffffff");
}



void setup()
{
  // Debug console
  Serial.begin(115200);

  ledSetup();
  
  restorePrefs();

  WiFi.onEvent(WiFiEvent);
 
  //  Blynk.setDeviceName("Blynk");
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

  Blynk.virtualWrite(YellowSliderPin, yellowLevel);
  Blynk.virtualWrite(WhiteSliderPin, whiteLevel);
  setButtonsColor();
  initialize_sntp();
  timer.setInterval(PrefsSaveTimeout, savePrefs);
}

void loop()
{
  Blynk.run();
  timer.run();
  ArduinoOTA.handle();
}


static void WiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info)
{
  if (event == SYSTEM_EVENT_STA_DISCONNECTED) {
    if (info.disconnected.reason == 6) {
      Serial.println("NOT_AUTHED reconnect");
      WiFi.reconnect();
    }
  }
}
