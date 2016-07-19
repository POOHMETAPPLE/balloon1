// Test code for Adafruit GPS modules using MTK3329/MTK3339 driver
//
// This code shows how to listen to the GPS module in an interrupt
// which allows the program to have more 'freedom' - just parse
// when a new NMEA sentence is available! Then access data when
// desired.
//
// Tested and works great with the Adafruit Ultimate GPS module
// using MTK33x9 chipset
//    ------> http://www.adafruit.com/products/746
// Pick one up today at the Adafruit electronics shop 
// and help support open source hardware & software! -ada

#include <Arduino.h>
#include "Radio.h"
#include "GPS.h"
#include "SDCard.h"
#include "Photocells.h"

// This sketch is ONLY for the Arduino Due!
// You should make the following connections with the Due and GPS module:
// GPS power pin to Arduino Due 3.3V output.
// GPS ground pin to Arduino Due ground.
// For hardware serial 1 (recommended):
//   GPS TX to Arduino Due Serial1 RX pin 19
//   GPS RX to Arduino Due Serial1 TX pin 18
#define gpsSerial Serial1
#define radioSerial Serial2
const int sdChipSelectPin = 4;

GPS gps(&gpsSerial);
Radio radio(&radioSerial, 2);
SDCard sd(sdChipSelectPin);
Photocells photocells(0, 5);

// Steven: maybe should use container classes. array/vector?
const int numModules = 4;
Module* modules[numModules] = {
    &gps
  , &radio
  , &sd
  , &photocells
};

void setup() {
  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(115200);
  Serial.println("Adafruit GPS library basic test!");

  for(Module *module : modules) {
    module->enable();
    module->begin();
  }

  sd.registerModules(modules, numModules);
  // sd.doSDTimingBenchmark();
}

void loop() {
  gps.tick();
  radio.tick();
  sd.tick();
}

