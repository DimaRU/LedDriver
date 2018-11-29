////
///  BlunkIO.ino - Blynk I/O
//


BLYNK_CONNECTED(){
  Serial.println("Blynk connected");
  Blynk.setProperty(YellowSlider, "min", SLIDER_MINVALUE);
  Blynk.setProperty(WhiteSlider, "min", SLIDER_MINVALUE);
  Blynk.setProperty(YellowSlider, "max", SLIDER_MAXVALUE);
  Blynk.setProperty(WhiteSlider, "max", SLIDER_MAXVALUE);
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
  if (pinValue != 1) return;

  powerState = Night;
  nightLedOn(true);
  Blynk.virtualWrite(OnButtonPin, 0);
  Blynk.virtualWrite(OffButtonPin, 0);
  setButtonsColor();
  ledsOff();
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
  setBrightness(yellowSlider, YellowChannel);
  setBrightness(whiteSlider, WhiteChannel);
}
