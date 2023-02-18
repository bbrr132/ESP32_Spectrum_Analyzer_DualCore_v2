#include <WiFiManager.h>
#include "memory.h"
#define TRIGGER_PIN 32
int timeout = 60; // seconds to run for
WiFiManager wm;

void startWM(){
  //startMemory();
  Serial.println("a");
  Serial.println(apN);
  Serial.println("b");
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP  
  // put your setup code here, to run once:
  
  Serial.println("\n Starting");
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  
  WiFiManager wm;
  WiFiManagerParameter adafruitIOUser("adafruitIOUser", "Enter User", user.c_str(), 50);
  wm.addParameter(&adafruitIOUser);
  WiFiManagerParameter adafruitIOKey("adafruitIOKey", "Enter Key", key.c_str(), 50);
  wm.addParameter(&adafruitIOKey);
  WiFiManagerParameter adafruitIOGroup("adafruitIOGroup", "Enter Group Name", groupName.c_str(), 50);
  wm.addParameter(&adafruitIOGroup);
  WiFiManagerParameter accessPointName("accessPointName", "Enter WiFi Name", apN.c_str(), 50);
  wm.addParameter(&accessPointName);
  
  //wm.resetSettings();
  bool res;
  res = wm.autoConnect(apName);
  if(!res) {
     Serial.println("Failed to connect");
     // ESP.restart();
  } 
    user=String(adafruitIOUser.getValue());
    preferences.putString("user", user);
    
    key=String(adafruitIOKey.getValue());
    preferences.putString("key", key);
    
    groupName=String(adafruitIOGroup.getValue());
    preferences.putString("groupname", groupName);
    
    apN=String(accessPointName.getValue());
    preferences.putString("apn", apN);
    
    apName = apN.c_str();
    IO_USERNAME = user.c_str();
    IO_KEY = key.c_str();
    FEED_GROUP_NAME = groupName.c_str();
    
    AdafruitIO_WiFi io(adafruitIOUser.getValue(),adafruitIOKey.getValue(),"","");
    //AdafruitIO_WiFi io(IO_USERNAME,IO_KEY,"","");
    AdafruitIO_Group *group = io.group(FEED_GROUP_NAME);
    io.connect();
  
}

void checkWM(){
  if ( digitalRead(TRIGGER_PIN) == LOW) {  
  char userString[50];
  char keyString[50];
  char groupString[50];
  char ssidString[50];
  strcpy(ssidString, apN.c_str());
  strcpy(userString, user.c_str());
  strcpy(groupString, groupName.c_str());
  strcpy(keyString, key.c_str()); 
  
  WiFiManager wm;
  WiFiManagerParameter adafruitIOUser("adafruitIOUser", "Enter User", user.c_str(), 50);
  wm.addParameter(&adafruitIOUser);
  WiFiManagerParameter adafruitIOKey("adafruitIOKey", "Enter Key", key.c_str(), 50);
  wm.addParameter(&adafruitIOKey);
  WiFiManagerParameter adafruitIOGroup("adafruitIOGroup", "Enter Group Name", groupName.c_str(), 50);
  wm.addParameter(&adafruitIOGroup);
  WiFiManagerParameter accessPointName("accessPointName", "Enter WiFi Name", apN.c_str(), 50);
  wm.addParameter(&accessPointName);
  
  /*
    

   */
    //reset settings - for testing
    //wm.resetSettings();
  
    // set configportal timeout
    wm.setConfigPortalTimeout(timeout);
    
    if (!wm.startConfigPortal(apName)) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
      ESP.restart();
      delay(5000);
    }
    Serial.println("connected...yeey :)");
    Serial.println(accessPointName.getValue());
    
    user=String(adafruitIOUser.getValue());
    preferences.putString("user", user);
    
    key=String(adafruitIOKey.getValue());
    preferences.putString("key", key);
    
    groupName=String(adafruitIOGroup.getValue());
    preferences.putString("groupname", groupName);
    
    apN=String(accessPointName.getValue());
    preferences.putString("apn", apN);
    
    apName = apN.c_str();
    IO_USERNAME = user.c_str();
    IO_KEY = key.c_str();
    FEED_GROUP_NAME = groupName.c_str();
    
    AdafruitIO_WiFi io(adafruitIOUser.getValue(),adafruitIOKey.getValue(),"","");
    //AdafruitIO_WiFi io(IO_USERNAME,IO_KEY,"","");
    AdafruitIO_Group *group = io.group(FEED_GROUP_NAME);
    io.connect();
    Serial.println("Settings Changed");

  }
}
