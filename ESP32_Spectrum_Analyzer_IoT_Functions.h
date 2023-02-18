#include "AdafruitIOStuff.h"


uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  if(x % 2 == 0)
  {
    if(y>7)
    {
      i=((32-x)*8-1)-y+256+8;
    }
    else
    {
      i=((32-x)*8-1)-y;
    }
    
  }
  else
  {
    if(y>7)
    {
      i=((32-x)*8-1)-7+y+256-8;
    }
    else
    {
      i=((32-x)*8-1)-7+y;
    }
    
  }
  return i;
}
// PATTERNS BELOW //

void rainbowBars(int band, int barHeight) {
  int xStart = BAR_WIDTH * band;
  for (int x = xStart; x < xStart + BAR_WIDTH; x++)
  {
    for (int y = 0; y < barHeight; y++) {
      leds(XY(x,y)) = CHSV((x / BAR_WIDTH) * (255 / NUM_BANDS), 255, 255);
    }
  }
}

void purpleBars(int band, int barHeight) {
  int xStart = BAR_WIDTH * band;
  for (int x = xStart; x < xStart + BAR_WIDTH; x++) {
    for (int y = 0; y < barHeight; y++) {
      leds(XY(x,y)) = ColorFromPalette(purplePal, y * (255 / barHeight));
    }
  }
}

void customBars(int band, int barHeight) {
  int xStart = BAR_WIDTH * band;
  for (int x = xStart; x < xStart + BAR_WIDTH; x++) {
    for (int y = 0; y < barHeight; y++) {
      leds(XY(x,y)) = ColorFromPalette(customPal, y * (255 / barHeight));
    }
  }
}

void changingBars(int band, int barHeight) {
  int xStart = BAR_WIDTH * band;
  for (int x = xStart; x < xStart + BAR_WIDTH; x++) {
    for (int y = 0; y < barHeight; y++) {
      leds(XY(x,y)) = CHSV(y * (255 / kMatrixHeight) + colorTimer, 255, 255);
    }
  }
}

void centerBars(int band, int barHeight) {
  int xStart = BAR_WIDTH * band;
  for (int x = xStart; x < xStart + BAR_WIDTH; x++) {
    if (barHeight % 2 == 0) barHeight--;
    int yStart = ((kMatrixHeight - barHeight) / 2 );
    for (int y = yStart; y <= (yStart + barHeight); y++) {
      int colorIndex = constrain((y - yStart) * (255 / barHeight), 0, 255);
      leds(XY(x,y)) = ColorFromPalette(heatPal, colorIndex);
    }
  }
}

void whitePeak(int band) {
  int xStart = BAR_WIDTH * band;
  int peakHeight = peak[band];
  for (int x = xStart; x < xStart + BAR_WIDTH; x++) {
    leds(XY(x,peakHeight)) = CRGB::White;
  }
}

void outrunPeak(int band) {
  int xStart = BAR_WIDTH * band;
  int peakHeight = peak[band];
  for (int x = xStart; x < xStart + BAR_WIDTH; x++) {
    //leds[XY(x,peakHeight)] = CHSV(peakHeight * (255 / kMatrixHeight), 255, 255);
    leds(XY(x,peakHeight)) = ColorFromPalette(outrunPal, peakHeight * (255 / kMatrixHeight));
  }
}

void waterfall(int band) {
  int xStart = BAR_WIDTH * band;
  double highestBandValue = 60000;        // Set this to calibrate your waterfall

  // Draw bottom line
  for (int x = xStart; x < xStart + BAR_WIDTH; x++) {
    leds(XY(x,0)) = CHSV(constrain(map(bandValues[band],0,highestBandValue,160,0),0,160), 255, 255);
  }

  // Move screen up starting at 2nd row from top
  if (band == NUM_BANDS - 1){
    for (int y = kMatrixHeight - 2; y >= 0; y--) {
      for (int x = 0; x < kMatrixWidth; x++) {
        leds(XY(x,y+1)) = leds(XY(x,y));
      }
    }
  }
}
void fastLEDSetup(){
  if(buttonPushCounter >=8)
  {
    FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds2, NUM_LEDS).setCorrection(TypicalSMD5050);
  }
  else
  {
    FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds[0], leds.Size()).setCorrection(TypicalSMD5050);
  }
  FastLED.setMaxPowerInVoltsAndMilliamps(LED_VOLTS, MAX_MILLIAMPS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQ));
}


void SampleAndOutput(){
  FastLED.setBrightness(BRIGHTNESS);
  // Don't clear screen if waterfall pattern, be sure to change this is you change the patterns / order
  if (buttonPushCounter != 5) FastLED.clear();
  // Reset bandValues[]
  for (int i = 0; i<NUM_BANDS; i++){
    bandValues[i] = 0;
  }
  // Sample the audio pin
  for (int i = 0; i < SAMPLES; i++) {
    newTime = micros();
    vReal[i] = analogRead(AUDIO_IN_PIN); // A conversion takes about 9.7uS on an ESP32
    vImag[i] = 0;
    while ((micros() - newTime) < sampling_period_us) { /* chill */ }
  }
  // Compute FFT
  FFT.DCRemoval();
  FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(FFT_FORWARD);
  FFT.ComplexToMagnitude();
  // Analyse FFT results
  int j=0;
  for (int i = 2; i < (SAMPLES/2); i++){       // Don't use sample 0 and only first SAMPLES/2 are usable. Each array element represents a frequency bin and its value the amplitude.
    if (vReal[i] > NOISE) {                    // Add a crude noise filter
      
    if(NUM_BANDS==8)
    {
      if (i<=3 )           bandValues[0]  += (int)vReal[i];
      if (i>3   && i<=6  ) bandValues[1]  += (int)vReal[i];
      if (i>6   && i<=13 ) bandValues[2]  += (int)vReal[i];
      if (i>13  && i<=27 ) bandValues[3]  += (int)vReal[i];
      if (i>27  && i<=55 ) bandValues[4]  += (int)vReal[i];
      if (i>55  && i<=112) bandValues[5]  += (int)vReal[i];
      if (i>112 && i<=229) bandValues[6]  += (int)vReal[i];
      if (i>229          ) bandValues[7]  += (int)vReal[i];
    }
    else if(NUM_BANDS==16)
    {
      if (i<=2 )           bandValues[0]  += (int)vReal[i];
      if (i>2   && i<=3  ) bandValues[1]  += (int)vReal[i];
      if (i>3   && i<=5  ) bandValues[2]  += (int)vReal[i];
      if (i>5   && i<=7  ) bandValues[3]  += (int)vReal[i];
      if (i>7   && i<=9  ) bandValues[4]  += (int)vReal[i];
      if (i>9   && i<=13 ) bandValues[5]  += (int)vReal[i];
      if (i>13  && i<=18 ) bandValues[6]  += (int)vReal[i];
      if (i>18  && i<=25 ) bandValues[7]  += (int)vReal[i];
      if (i>25  && i<=36 ) bandValues[8]  += (int)vReal[i];
      if (i>36  && i<=50 ) bandValues[9]  += (int)vReal[i];
      if (i>50  && i<=69 ) bandValues[10] += (int)vReal[i];
      if (i>69  && i<=97 ) bandValues[11] += (int)vReal[i];
      if (i>97  && i<=135) bandValues[12] += (int)vReal[i];
      if (i>135 && i<=189) bandValues[13] += (int)vReal[i];
      if (i>189 && i<=264) bandValues[14] += (int)vReal[i];
      if (i>264          ) bandValues[15] += (int)vReal[i];
      
    }
      else if(NUM_BANDS==32)
      {
        if (i<=2 )           bandValues[0]  += (int)vReal[i];
      if (i>2   && i<=3  ) bandValues[1]  += (int)vReal[i];
      if (i>3   && i<=4  ) bandValues[2]  += (int)vReal[i];
      if (i>3   && i<=4  ) bandValues[3]  += (int)vReal[i];
      if (i>4   && i<=5  ) bandValues[4]  += (int)vReal[i];
      if (i>4   && i<=5 ) bandValues[5]  += (int)vReal[i];
      if (i>5  && i<=6 ) bandValues[6]  += (int)vReal[i];
      if (i>6  && i<=7 ) bandValues[7]  += (int)vReal[i];
      if (i>7  && i<=9 ) bandValues[8]  += (int)vReal[i];
      if (i>9  && i<=10 ) bandValues[9]  += (int)vReal[i];
      if (i>10  && i<=12 ) bandValues[10] += (int)vReal[i];
      if (i>12  && i<=15 ) bandValues[11] += (int)vReal[i];
      if (i>15  && i<=17) bandValues[12] += (int)vReal[i];
      if (i>17 && i<=21) bandValues[13] += (int)vReal[i];
      if (i>21 && i<=25) bandValues[14] += (int)vReal[i];
      if (i>25 && i<=29) bandValues[15] += (int)vReal[i];
      if (i>29 && i<=34) bandValues[16] += (int)vReal[i];
      if (i>34 && i<=41) bandValues[17] += (int)vReal[i];
      if (i>41 && i<=49) bandValues[18] += (int)vReal[i];
      if (i>49 && i<=58) bandValues[19] += (int)vReal[i];
      if (i>58 && i<=68) bandValues[20] += (int)vReal[i];
      if (i>68 && i<=81) bandValues[21] += (int)vReal[i];
      if (i>81 && i<=96) bandValues[22] += (int)vReal[i];
      if (i>96 && i<=114) bandValues[23] += (int)vReal[i];
      if (i>114 && i<=135) bandValues[24] += (int)vReal[i];
      if (i>135 && i<=161) bandValues[25] += (int)vReal[i];
      if (i>161 && i<=191) bandValues[26] += (int)vReal[i];
      if (i>191 && i<=226) bandValues[27] += (int)vReal[i];
      if (i>226 && i<=268) bandValues[28] += (int)vReal[i];
      if (i>268 && i<=318) bandValues[29] += (int)vReal[i];
      if (i>318 && i<=377) bandValues[30] += (int)vReal[i];
      if (i>377 && i<=448) bandValues[31] += (int)vReal[i];
      if (i>448          ) bandValues[32] += (int)vReal[i];
      }
    }
  }

  // Process the FFT data into bar heights
  for (byte band = 0; band < NUM_BANDS; band++) {
    // Scale the bars for the display
    int barHeight = bandValues[band] / AMPLITUDE;
    if (barHeight > TOP) barHeight = TOP;

    // Small amount of averaging between frames
    barHeight = ((oldBarHeights[band] * 1) + barHeight) / 2;

    // Move peak up
    if (barHeight > peak[band]) {
      peak[band] = min(TOP, barHeight);
    }

    // Draw bars
    switch (buttonPushCounter) {
      case 0:
        rainbowBars(band, barHeight);
        break;
      case 1:
        // No bars on this one
        break;
      case 2:
        purpleBars(band, barHeight);
        break;
      case 3:
        centerBars(band, barHeight);
        break;
      case 4:
        changingBars(band, barHeight);
        break;
      case 5:
        waterfall(band);
        break;
      case 6:
        customBars(band, barHeight);
        break;
    }

    // Draw peaks
    switch (buttonPushCounter) {
      case 0:
        whitePeak(band);
        break;
      case 1:
        outrunPeak(band);
        break;
      case 2:
        whitePeak(band);
        break;
      case 3:
        // No peaks
        break;
      case 4:
        // No peaks
        break;
      case 5:
        // No peaks
        break;
      case 6:
        whitePeak(band);
        break;
    }

    // Save oldBarHeights for averaging later
    oldBarHeights[band] = barHeight;
  }
 
  // Decay peak
  EVERY_N_MILLISECONDS(5) {
    for (byte band = 0; band < NUM_BANDS; band++)
      if (peak[band] > 0) peak[band] -= 1;
    colorTimer++;
  }

  // Used in some of the patterns
  EVERY_N_MILLISECONDS(5) {
    colorTimer++;
  }
 
  FastLED.show();
}

void selectMode()
{
  if (buttonPushCounter<7)
  {
    SampleAndOutput();
  }
  else if (buttonPushCounter==7)
  {
    scrollText();
  }
  else if (buttonPushCounter>=8)
  {
    twinkleLEDS();
  }
}
