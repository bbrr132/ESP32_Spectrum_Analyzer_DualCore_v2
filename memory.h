#include <Preferences.h>
int AMPLITUDE;                   // Depending on your audio source level, you may need to alter this value. Can be used as a 'sensitivity' control.
int BRIGHTNESS;                   // Brightness 0 - 255, but won't exceed current specified above
int NOISE;                         // Used as a crude noise filter, values below this are ignored
int NUM_BANDS;                     // To change this, you will need to change the bunch of if statements describing the mapping from bins to bands
int buttonPushCounter;
int TWINKLE_SPEED;
int TWINKLE_DENSITY;
int SECONDS_PER_PALETTE;
int stringLength;
const char *IO_USERNAME = "";
const char *IO_KEY = "";
const char *FEED_GROUP_NAME = "led-display";
const char *apName;

uint8_t c1r;
uint8_t c2r;
uint8_t c3r;
uint8_t c4r;
uint8_t c1g;
uint8_t c2g;
uint8_t c3g;
uint8_t c4g;
uint8_t c1b;
uint8_t c2b;
uint8_t c3b;
uint8_t c4b;
String text;
String key;
String groupName;
String user;
String apN;

Preferences preferences;

void startMemory() {
  preferences.begin("my-app", false);
  AMPLITUDE = preferences.getInt("amp", 10000); 
  BRIGHTNESS = preferences.getInt("brightness", 10); 
  NOISE = preferences.getInt("noise", 500); 
  NUM_BANDS = preferences.getInt("bands", 32);
  buttonPushCounter = preferences.getInt("counter", 0);
  text = preferences.getString("text", "name");
  key = preferences.getString("key", "aio_MQKa64I4LlrtOjCsqz0OnEOLiTCr");
  user = preferences.getString("user", "phyllisdisplaylights");
  groupName = preferences.getString("groupname", "led-display");
  apN = preferences.getString("apn", "ESP Wifi");
  stringLength = preferences.getInt("stringLength", 100);
  TWINKLE_SPEED = preferences.getInt("twinklespeed", 5);
  TWINKLE_DENSITY = preferences.getInt("twinkledensity", 8);
  SECONDS_PER_PALETTE = preferences.getInt("palettetimer", 5);
  c1r = preferences.getUChar("c1r", 255);
  c2r = preferences.getUChar("c2r", 255);
  c3r = preferences.getUChar("c3r", 255);
  c4r = preferences.getUChar("c4r", 255);
  c1g = preferences.getUChar("c1g", 255);
  c2g = preferences.getUChar("c2g", 255);
  c3g = preferences.getUChar("c3g", 255);
  c4g = preferences.getUChar("c4g", 255);
  c1b = preferences.getUChar("c1b", 255);
  c2b = preferences.getUChar("c2b", 255);
  c3b = preferences.getUChar("c3b", 255);
  c4b = preferences.getUChar("c4b", 255); 
}
