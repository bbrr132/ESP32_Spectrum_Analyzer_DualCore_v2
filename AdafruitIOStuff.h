#include "text.h"

#define IO_LOOP_DELAY 500 
unsigned long lastUpdate;
String Modes[] = {"Rainbow","Purple","Pink","Center","Changing","Waterfall","Custom","Text","Retro","BlueWhite","Rainbows","Fairy","RGWhite","Party","RWhite","Snow","Xmas","Ice","Twinkle"};
int arrLen = sizeof Modes / sizeof Modes[0];
int previousSettingMode = 0;
TDynamicRGBGradientPalette_byte custom_gp[] = {
  0, 255, 255, 255,      
 86, 255, 255, 255,      
170, 255, 255, 255,    
255, 255, 255, 255,
};
AdafruitIO_WiFi io(IO_USERNAME,IO_KEY,"","");
AdafruitIO_Group *group = io.group(FEED_GROUP_NAME);

void set_custom_gradient_palette(int i,uint8_t r, uint8_t g, uint8_t b) {
  TRGBGradientPaletteEntryUnion* grad_entry = (TRGBGradientPaletteEntryUnion*) custom_gp;
  grad_entry[i].r = r;
  grad_entry[i].g = g;
  grad_entry[i].b = b;
  
}
CRGBPalette16 customPal=customPal.loadDynamicGradientPalette(custom_gp);

String selectedMode = "";

void handleBrightness(AdafruitIO_Data *data) {
  BRIGHTNESS=data->toInt();
  preferences.putInt("brightness", BRIGHTNESS);
}
void handleamplitude(AdafruitIO_Data *data) {
  AMPLITUDE=data->toInt();
  preferences.putInt("amp", AMPLITUDE);
}
void handleMode(AdafruitIO_Data *data) {
    //buttonPushCounter = 0;
    for (int i = 0; i < arrLen; i++) 
    {
        if (Modes[i] == data->toString()) 
        {
            buttonPushCounter = i;
            if(i !=7)
            {
              previousSettingMode=i;
              preferences.putInt("counter", buttonPushCounter);
            }
            if(buttonPushCounter>=8)
            {
              FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds2, NUM_LEDS).setCorrection(TypicalSMD5050);
            }
            else
            {
              FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds[0], leds.Size()).setCorrection(TypicalSMD5050);
            }
            break;
        }
    }
    Serial.println(buttonPushCounter);
}
void handlenoise_filter(AdafruitIO_Data *data) {
  NOISE=data->toInt();
  preferences.putInt("noise", NOISE);
}

void handleNumberOfBands(AdafruitIO_Data *data) {
  if(data->toInt()==24)
  {
    group->set("number-of-bands", 32);
    NUM_BANDS=32;
    group->save();
  }
  else
  {
    NUM_BANDS = data->toInt(); 
  }
  preferences.putInt("bands", NUM_BANDS);
}
void handlecolour1(AdafruitIO_Data *data) {
  uint8_t r = data->toRed();
  uint8_t g = data->toGreen();
  uint8_t b = data->toBlue();
  set_custom_gradient_palette(0, r, g, b);
  customPal.loadDynamicGradientPalette(custom_gp);
  c1r = preferences.putUChar("c1r", r);
  c1g = preferences.putUChar("c1g", g);
  c1b = preferences.putUChar("c1b", b);
}
void handlecolour2(AdafruitIO_Data *data) {
  uint8_t r = data->toRed();
  uint8_t g = data->toGreen();
  uint8_t b = data->toBlue();
  set_custom_gradient_palette(1, r, g, b);
  customPal.loadDynamicGradientPalette(custom_gp);
  c2r = preferences.putUChar("c2r", r);
  c2g = preferences.putUChar("c2g", g);
  c2b = preferences.putUChar("c2b", b);
}
void handlecolour3(AdafruitIO_Data *data) {
  uint8_t r = data->toRed();
  uint8_t g = data->toGreen();
  uint8_t b = data->toBlue();
  set_custom_gradient_palette(2, r, g, b);
  customPal.loadDynamicGradientPalette(custom_gp);
  c3r = preferences.putUChar("c3r", r);
  c3g = preferences.putUChar("c3g", g);
  c3b = preferences.putUChar("c3b", b);
}
void handlecolour4(AdafruitIO_Data *data) {
  uint8_t r = data->toRed();
  uint8_t g = data->toGreen();
  uint8_t b = data->toBlue();
  set_custom_gradient_palette(3, r, g, b);
  customPal.loadDynamicGradientPalette(custom_gp);
  c4r = preferences.putUChar("c4r", r);
  c4g = preferences.putUChar("c4g", g);
  c4b = preferences.putUChar("c4b", b);
}
void handletext(AdafruitIO_Data *data) {
  text = data->toString();
  stringLength=text.length();
  text.toCharArray(TxtDemo, stringLength+1);
  preferences.putInt("stringLength", stringLength);
  Serial.println(preferences.getInt("stringLength", 100));
  preferences.putString("text", text);
  Serial.println(preferences.getString("text", "Name"));
  ScrollingMsg.SetText((unsigned char *)TxtDemo, stringLength);
}
void AdafruitIOStart(){
  io.connect();
  group->onMessage("mode", handleMode);
  group->onMessage("led-brightness", handleBrightness);
  group->onMessage("number-of-bands", handleNumberOfBands);
  group->onMessage("noise-filter", handlenoise_filter);
  group->onMessage("amplitude", handleamplitude);
  group->onMessage("colour1", handlecolour1);
  group->onMessage("colour2", handlecolour2);
  group->onMessage("colour3", handlecolour3);
  group->onMessage("colour4", handlecolour4);
  group->onMessage("text", handletext);
  group->get();
}

void scrollText(){
  if (ScrollingMsg.UpdateText() == -1)
  {
    ScrollingMsg.SetText((unsigned char *)TxtDemo, sizeof(TxtDemo)-1);
    group->set("mode", Modes[previousSettingMode]);
    group->save();
    io.run();
    buttonPushCounter = previousSettingMode;
    //ScrollingMsg.SetText((unsigned char *)TxtDemo, stringLength);
    Options ^= INSTANT_OPTIONS_MODE;
    ScrollingMsg.SetOptionsChangeMode(Options);
  }
  else
    FastLED.show();
}

void customColourStart(){
  TRGBGradientPaletteEntryUnion* grad_entry = (TRGBGradientPaletteEntryUnion*) custom_gp;
  grad_entry[0].r = c1r;
  grad_entry[0].g = c1g;
  grad_entry[0].b = c1b;
  grad_entry[1].r = c2r;
  grad_entry[1].g = c2g;
  grad_entry[1].b = c2b; 
  grad_entry[2].r = c3r;
  grad_entry[2].g = c3g;
  grad_entry[2].b = c3b; 
  grad_entry[3].r = c4r;
  grad_entry[3].g = c4g;
  grad_entry[3].b = c4b;  
  customPal.loadDynamicGradientPalette(custom_gp);
}

void UpdateDataIO(){
  if (millis() > (lastUpdate + IO_LOOP_DELAY) && buttonPushCounter!= 7) {
    io.run();
    lastUpdate = millis();
  }
}
