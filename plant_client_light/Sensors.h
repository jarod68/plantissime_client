/* ---------------------------------------------------------------------------
 ** Author: Matthieu Holtz
 ** Year:   2015
 ** -------------------------------------------------------------------------*/

#pragma once

#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

#include <TSL2561.h>
#include <dht11.h>

class Sensors {
    
private:
    const uint16_t _pinSoil_0;
    const uint16_t _pinSoil_1;
    const uint16_t _pinSoil_2;
    const uint16_t _pinSoil_3;
    const uint16_t _pinDHT11 ;
    const String   _nodeIdentifier;
    TSL2561 *      _tsl;
    dht11   *      _dht11;
    
protected:
    static  int         readSoil    (const uint16_t analogPin);
    virtual float       readTSL     ();
    virtual bool        readDHT11   (int16_t * humidity, int16_t * temperature);
    
public:
    Sensors              (const uint16_t pinSoil_0,
                          const uint16_t pinSoil_1,
                          const uint16_t pinSoil_2,
                          const uint16_t pinSoil_3,
                          const uint16_t pinDHT11,
                          const String  nodeIdentifier
                         );
    
    virtual         ~Sensors    ();
    
    virtual String  capture     ();

};


#endif //SENSORS_H