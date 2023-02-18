#include "AdafruitIO_WiFi.h"
#include "FastLEDStuffs.h"

#include <FastLED.h>
#include <LEDMatrix.h>
#include <LEDText.h>
#include <Font12x16.h>

cLEDText ScrollingMsg;
uint16_t Options;
//int stringLength = 0;
char TxtDemo[82];

void setupScrollText(){
  ScrollingMsg.SetFont(Font12x16Data);
  ScrollingMsg.Init(&leds, leds.Width(), ScrollingMsg.FontHeight(), 0, 0);
  ScrollingMsg.SetScrollDirection(SCROLL_LEFT);
  ScrollingMsg.SetFrameRate(1);
  ScrollingMsg.SetText((unsigned char *)TxtDemo, stringLength);
  //ScrollingMsg.SetTextColrOptions(COLR_RGB | COLR_SINGLE, 0xff, 0x00, 0xff);
  ScrollingMsg.SetTextColrOptions(COLR_HSV | COLR_GRAD_AH, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff);
  Options = INSTANT_OPTIONS_MODE;
  ScrollingMsg.SetOptionsChangeMode(Options);
}
