// 
// plant_client_light 
//
// Description of the project
// Developed with [embedXcode](http://embedXcode.weebly.com)
// 
// Author	 	Matt
// 				Matthieu Holtz
//
// Date			24/12/2014 14:01
// Version		0.1
// 
// Copyright	© Matt, 2014
// License		<#license#>
//
// See			ReadMe.txt for references
//

/* Arduino Libraries */

#include <Arduino.h>
#include <Wire.h>

/* Project Libraries */

#include <dht11.h>
#include <TSL2561.h>
#include <XBee.h>

/* Project classes */

#include "Sensors.h"


Sensors * sensors = NULL;

XBee * bee = NULL;

const uint16_t PIN_SOIL_0 = A0;
const uint16_t PIN_SOIL_1 = A1;
const uint16_t PIN_SOIL_2 = A6;
const uint16_t PIN_SOIL_3 = A7;

const uint16_t PIN_DHT11 = 12;
const uint16_t PIN_LED = 9;
const uint16_t PIN_BUTTON = 8;

const uint16_t XBEE_SLEEP = 7;


void setup()
{
    Serial.begin(9600);
    
    pinMode(PIN_DHT11, INPUT);
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_BUTTON, INPUT);
    pinMode(XBEE_SLEEP, OUTPUT);
    
    bee = new XBee();
    
    sensors = new Sensors(PIN_SOIL_0, PIN_SOIL_1, PIN_SOIL_2, PIN_SOIL_3, PIN_DHT11);
                          
}

void loop()
{
    Serial.print(sensors->capture());
    
    delay(4000);
    
}
