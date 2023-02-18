#include "ESP32_Spectrum_Analyzer_IoT_Functions.h"

void setup() {
  Serial.begin(115200);
  startMemory();
  Serial.println("Mem done");
  startWM();
  Serial.println("wm done");
  fastLEDSetup();
  Serial.println("led done");
  customColourStart();
  Serial.println("clr done");
  setupScrollText();
  Serial.println("txt done");
  AdafruitIOStart();
  Serial.println("aio done");
  // Connect to Wi-Fi
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    NULL,      /* Task handle to keep track of created task */
                    0);
}

void loop() {
  checkWM();
  //Serial.println("T1");
  //Serial.println(millis());
  selectMode();
  //SampleAndOutput();
  //Serial.println("T2");
  //Serial.println(millis());
  
  //Serial.println("T3");
  //Serial.println(millis());
}
void Task1code( void * parameter) {
  for(;;) {
      UpdateDataIO();
      vTaskDelay(10);
  }
}
