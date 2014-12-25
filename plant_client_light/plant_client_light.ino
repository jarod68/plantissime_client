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

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

/* Project Libraries */

#include <dht11.h>
#include <TSL2561.h>

/* Project classes */

#include "Sensors.h"
#include "XBeeLink.h"

XBeeLink * xbee = NULL;
Sensors * sensors = NULL;

const uint16_t PIN_SOIL_0 = A0;
const uint16_t PIN_SOIL_1 = A1;
const uint16_t PIN_SOIL_2 = A6;
const uint16_t PIN_SOIL_3 = A7;

const uint16_t PIN_DHT11 = 12;
const uint16_t PIN_LED = 9;
const uint16_t PIN_BUTTON = 8;

const uint16_t XBEE_SLEEP = 7;


volatile uint16_t sleepSecondElapsed = 0;

void ledOn ()
{
    digitalWrite(PIN_LED, HIGH);
}

void ledOff ()
{
    digitalWrite(PIN_LED, LOW);
}

// Timers interrupt routine

ISR( WDT_vect ) {
    
    sleepSecondElapsed+=7;
}

void setupSleep ()
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);             // select the watchdog timer mode
    MCUSR &= ~(1 << WDRF);                           // reset status flag
    WDTCSR |= (1 << WDCE) | (1 << WDE);              // enable configuration changes
    WDTCSR = (1<< WDP0) | (1 << WDP1) | (1 << WDP2); // set the prescalar = 7
    WDTCSR |= (1 << WDIE);                           // enable interrupt mode

}

void deepSleep()
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();                                  // enable the sleep mode ready for use
    sleep_mode();                                    // trigger the sleep
    
    sleep_disable();                                 // prevent further sleeps
}

void setup()
{
    ledOn();
    Serial.begin(9600);
    
    pinMode(PIN_DHT11, INPUT);
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_BUTTON, INPUT);
    pinMode(XBEE_SLEEP, OUTPUT);
    
    
    digitalWrite(XBEE_SLEEP, LOW);   // deassert to keep radio awake when sleep mode selected
    
    xbee = new XBeeLink();
    sensors = new Sensors(PIN_SOIL_0, PIN_SOIL_1, PIN_SOIL_2, PIN_SOIL_3, PIN_DHT11, xbee->getXBeeIdentifier());
    xbee->configureSleepOnD7();
    
    setupSleep();
    ledOff();
}


void loop()
{

    deepSleep();

    if(sleepSecondElapsed>10){
        ledOn();
        digitalWrite(XBEE_SLEEP, LOW);
        Serial.print(sensors->capture());
        digitalWrite(XBEE_SLEEP, HIGH);
        sleepSecondElapsed=0;
        delay(1000);
        ledOff();
    }
}
