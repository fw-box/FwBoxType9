//
// Copyright (c) 2020 Fw-Box (https://fw-box.com)
// Author: Hartman Hsieh
//
// Description :
//   None
//
// Connections :
//
// Required Library :
//

#include "FwBox.h"
#include <Wire.h>

//
// GPIO defines
//
#define DEVICE_TYPE 9
#define FIRMWARE_VERSION "1.1.3"

#define PIN_ONOFF 0 // GPIO0
#define PIN_LED LED_BUILTIN

#define VAL_INDEX_ONOFF 0

//
// Debug definitions
//
#define FW_BOX_DEBUG 0

#if FW_BOX_DEBUG == 1
  #define DBG_PRINT(VAL) Serial.print(VAL)
  #define DBG_PRINTLN(VAL) Serial.println(VAL)
  #define DBG_PRINTF(FORMAT, ARG) Serial.printf(FORMAT, ARG)
  #define DBG_PRINTF2(FORMAT, ARG1, ARG2) Serial.printf(FORMAT, ARG1, ARG2)
#else
  #define DBG_PRINT(VAL)
  #define DBG_PRINTLN(VAL)
  #define DBG_PRINTF(FORMAT, ARG)
  #define DBG_PRINTF2(FORMAT, ARG1, ARG2)
#endif

//
// Global variable
//

void setup()
{
  Wire.begin();
  Serial.begin(115200);

  pinMode(PIN_ONOFF, OUTPUT);
  pinMode(PIN_LED, OUTPUT);

  //
  // Initialize the fw-box core (early stage)
  //
  fbEarlyBegin(DEVICE_TYPE, FIRMWARE_VERSION);

  //
  // Initialize the fw-box core
  //
  fbBegin(DEVICE_TYPE, FIRMWARE_VERSION);

  //
  // Setup MQTT subscribe callback
  //
  setRcvValueCallback(onReceiveValue);

} // void setup()

void loop()
{
  //
  // Update the value
  //
  FwBoxIns.setValue(VAL_INDEX_ONOFF, !digitalRead(PIN_ONOFF)); // This value is reverse

  //
  // Run the handle
  //
  fbHandle();

} // END OF "void loop()"

void onReceiveValue(int valueIndex, String* payload)
{
  DBG_PRINT("onReceiveValue valueIndex = ");
  DBG_PRINTLN(valueIndex);
  DBG_PRINT("onReceiveValue *payload = ");
  DBG_PRINTLN(*payload);

  if(valueIndex == VAL_INDEX_ONOFF) {
    payload->toUpperCase();
    if(payload->equals("ON") == true)
    {
      digitalWrite(PIN_ONOFF, LOW); // This value is reverse
      FwBoxIns.mqttPublish(valueIndex, "ON"); // Sync the LED status to MQTT server

      //
      // Sync the status to LED
      //
      digitalWrite(PIN_LED, LOW); // This value is reverse
    }
    else
    {
      digitalWrite(PIN_ONOFF, HIGH); // This value is reverse
      FwBoxIns.mqttPublish(valueIndex, "OFF"); // Sync the LED status to MQTT server

      //
      // Sync the status to LED
      //
      digitalWrite(PIN_LED, HIGH); // This value is reverse
    }
  }
}
