// Adafruit IO Publish Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

// Modified by Mike Holden 2017 for coffee monitor project
// Connects to adafruit TMP007 sensor module, puts temp on IOT
// Compiled for NodeMCU 1.0 board using ESP8266 libraries.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/

#include <Wire.h>
#include "Adafruit_TMP007.h"

// TMP 007 connections:
// Connect VCC to +3V 
// Gnd -> Gnd
// SCL connects to the I2C clock pin. ESP8266 GPIO5
// SDA connects to the I2C data pin. ESP8266 GPIO4

Adafruit_TMP007 tmp007;

// this int will hold the current count for our sketch
float temperature = 0;

// set up the 'myfeed' feed
AdafruitIO_Feed *myfeed = io.feed("coffeetemp");

void setup() {

  // start the serial connection
  Serial.begin(115200);
  Serial.println("\r\n");
  Serial.println("hello temperature");

  // wait for serial monitor to open
  while(! Serial);

  Serial.println("\r\n");
  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  Serial.println("IO stream Connected");

  // you can also use tmp007.begin(TMP007_CFG_1SAMPLE) or 2SAMPLE/4SAMPLE/8SAMPLE to have
  // lower precision, higher rate sampling. default is TMP007_CFG_16SAMPLE which takes
  // 4 seconds per reading (16 samples)

// Try a few times to connect then give up
  int iiuu;
  for (iiuu=0;iiuu<5;iiuu++) {
    if (! tmp007.begin()) {
      Serial.print(iiuu);
      Serial.println(" No sensor found");
      delay(1000);
    } else {
      Serial.println("Sensor connected.");
      break;
    }
  }
}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  temperature = tmp007.readObjTempC();
  Serial.print("Object Temperature: "); Serial.print(temperature); Serial.println("*C");
  //float diet = tmp007.readDieTempC();
  //Serial.print("Die Temperature: "); Serial.print(diet); Serial.println("*C");

  // save to the feed on Adafruit IO
  Serial.print("sending -> ");
  Serial.println(temperature*1.8 +32.0);
  myfeed->save(temperature*1.8 +32.0);

  // wait 
  delay(4000);

}


