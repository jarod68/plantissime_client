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

#include <Arduino.h>
#include <dht11.h>
#include <Wire.h>
#include <TSL2561.h>


uint8_t myLED;

void setup() {

    myLED = 13;


    pinMode(myLED, OUTPUT);
}


void loop() {
    
    digitalWrite(myLED, HIGH);
    delay(500);
    digitalWrite(myLED, LOW);
    delay(500);
    
}
