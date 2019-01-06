////
///  BlynkIO.ino - Blynk I/O
//


BLYNK_WRITE(YellowSliderPin)
{
  int pinValue = param.asInt();
  if (yellowLevel == pinValue) return;

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

  whiteLevel = pinValue;
  if (powerState == On) {
    setBrightness(whiteLevel, WhiteChannel);
    cancelAlarm();
  }
}

BLYNK_WRITE(NightButtonPin)
{
  int pinValue = param.asInt();
  if (pinValue != BunttonTapState) return;

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
  if (pinValue != BunttonTapState) return;
  
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
  if (pinValue != BunttonTapState) return;
  
  powerState = Off;
  nightLedOn(false);
  Blynk.virtualWrite(OnButtonPin, 0);
  Blynk.virtualWrite(NightButtonPin, 0);
  setButtonsColor();
  ledsOff();
  cancelAlarm();
}

BLYNK_WRITE(SegmentedButtonPin)
{
 int pinValue = param.asInt();

  switch(pinValue) {
    case 1:
      powerState = Off;
      nightLedOn(false);
      ledsOff();
      cancelAlarm();
      break;
    case 2:
      powerState = On;
      nightLedOn(false);
      ledsRestore();
      break;
    case 3:
      powerState = Night;
      nightLedOn(true);
      ledsOff();
      cancelAlarm();
      break;
  }  
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
