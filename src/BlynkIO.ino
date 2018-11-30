////
///  BlynkIO.ino - Blynk I/O
//


BLYNK_WRITE(YellowSliderPin)
{
  int pinValue = param.asInt();
  if (yellowLevel == pinValue) return;

  Serial.printf("Yellow: %d->%d\r\n", yellowLevel, pinValue);
  yellowLevel = pinValue;
  if (powerState == On) {
    setBrightness(yellowLevel, YellowChannel);
    cancelAlarm();
  }
}

BLYNK_WRITE(WhiteSliderPin)
{
  int pinValue = param.asInt();
  if (whiteLevel == pinValue) return;

  Serial.printf("White: %d->%d\r\n", whiteLevel, pinValue);
  whiteLevel = pinValue;
  if (powerState == On) {
    setBrightness(whiteLevel, WhiteChannel);
    cancelAlarm();
  }
}

BLYNK_WRITE(NightButtonPin)
{
  int pinValue = param.asInt();
  Serial.print("NighButton: ");
  Serial.println(pinValue);
  if (pinValue != 1) return;

  powerState = Night;
  nightLedOn(true);
  Blynk.virtualWrite(OnButtonPin, 0);
  Blynk.virtualWrite(OffButtonPin, 0);
  setButtonsColor();
  ledsOff();
  cancelAlarm();
}

BLYNK_WRITE(OnButtonPin)
{
  int pinValue = param.asInt();
  Serial.print("OnButton: ");
  Serial.println(pinValue);
  if (pinValue != 1) return;
  
  powerState = On;
  nightLedOn(false);
  Blynk.virtualWrite(OffButtonPin, 0);
  Blynk.virtualWrite(NightButtonPin, 0);
  setButtonsColor();
  ledsRestore();
}

BLYNK_WRITE(OffButtonPin)
{
 int pinValue = param.asInt();
  Serial.print("OffButtonPin: ");
  Serial.println(pinValue);
  if (pinValue != 1) return;
  
  powerState = Off;
  nightLedOn(false);
  Blynk.virtualWrite(OnButtonPin, 0);
  Blynk.virtualWrite(NightButtonPin, 0);
  setButtonsColor();
  ledsOff();
  cancelAlarm();
}

void setButtonsColor()
{
  switch(powerState) {
    case On:
      Blynk.setProperty(OnButtonPin, "offBackColor", "#ffffff");
      Blynk.setProperty(OffButtonPin, "offBackColor", "#787878");
      Blynk.setProperty(NightButtonPin, "offBackColor", "#787878");
      break;
    case Off:
      Blynk.setProperty(OnButtonPin, "offBackColor", "#787878");
      Blynk.setProperty(OffButtonPin, "offBackColor", "#ffffff");
      Blynk.setProperty(NightButtonPin, "offBackColor", "#787878");
      break;
    case Night:
      Blynk.setProperty(OnButtonPin, "offBackColor", "#787878");
      Blynk.setProperty(OffButtonPin, "offBackColor", "#787878");
      Blynk.setProperty(NightButtonPin, "offBackColor", "#ffffff");
      break;
  }
}

void ledsOff() {
  setBrightness(0, YellowChannel);
  setBrightness(0, WhiteChannel);
}

void ledsRestore() {
  setBrightness(yellowLevel, YellowChannel);
  setBrightness(whiteLevel, WhiteChannel);
}
