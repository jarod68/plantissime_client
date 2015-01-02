/* ---------------------------------------------------------------------------
 ** Author: Matthieu Holtz
 ** Year:   2015
 ** -------------------------------------------------------------------------*/

#include "Sensors.h"


Sensors :: Sensors      (const uint16_t pinSoil_0,
                         const uint16_t pinSoil_1,
                         const uint16_t pinSoil_2,
                         const uint16_t pinSoil_3,
                         const uint16_t pinDHT11,
                         const String nodeIdentifier
                        ):
                        _pinSoil_0(pinSoil_0),
                        _pinSoil_1(pinSoil_1),
                        _pinSoil_2(pinSoil_2),
                        _pinSoil_3(pinSoil_3),
                        _pinDHT11(pinDHT11),
                        _nodeIdentifier(nodeIdentifier),
                        _tsl(new TSL2561(TSL2561_ADDR_FLOAT)),
                        _dht11(new dht11)
{
    uint16_t i = 0;
    while (_tsl->begin() && i<5)
        i++;
    
    _tsl->setGain(TSL2561_GAIN_16X);     // set 16x gain (for dim situations)
    
    _tsl->setTiming(TSL2561_INTEGRATIONTIME_101MS);
}

Sensors :: ~Sensors()
{
    delete _tsl;
    delete _dht11;
}

uint16_t Sensors::readSoil    (const uint16_t analogPin)
{
    int value = analogRead(analogPin);
    
    if (value >= 900)
        return -1;
    
    return value;
    
}

long Sensors::readPowerSupply    ()
{
    // Read 1.1V reference against AVcc
    // set the reference to Vcc and the measurement to the internal 1.1V reference
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
#else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif
    
    delay(2); // Wait for Vref to settle
    ADCSRA |= _BV(ADSC); // Start conversion
    while (bit_is_set(ADCSRA,ADSC)); // measuring
    
    uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH
    uint8_t high = ADCH; // unlocks both
    
    long result = (high<<8) | low;
    
    result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
    return result; // Vcc in millivolts

}

float       Sensors::readTSL     ()
{
    //return _tsl->getLuminosity(TSL2561_VISIBLE);
    
    uint32_t lum = _tsl->getFullLuminosity();
    uint16_t ir, full;
    ir = lum >> 16;
    full = lum & 0xFFFF;
    
    return _tsl->calculateLux(full, ir);
}

bool        Sensors::readDHT11   (int16_t * humidity, int16_t * temperature)
{
    
    if (_dht11->read(_pinDHT11) !=0)
        return false;
    
    *humidity     = _dht11->humidity;
    *temperature  = _dht11->temperature;
    
    return true;
    
}

String Sensors::capture              ()
{
    
    float luminosity = this->readTSL();
    char stringLuminosity[16];
    dtostrf(luminosity, 3, 1, stringLuminosity);
    
    int16_t humidity = -1;
    int16_t temperature = -1;
    this->readDHT11(&humidity, &temperature);
    
    int16_t soil0 = readSoil(_pinSoil_0);
    int16_t soil1 = readSoil(_pinSoil_1);
    int16_t soil2 = readSoil(_pinSoil_2);
    int16_t soil3 = readSoil(_pinSoil_3);
    
    char stringSoil0[16];
    dtostrf(soil0, 3, 1, stringSoil0);
    char stringSoil1[16];
    dtostrf(soil1, 3, 1, stringSoil1);
    char stringSoil2[16];
    dtostrf(soil2, 3, 1, stringSoil2);
    char stringSoil3[16];
    dtostrf(soil3, 3, 1, stringSoil3);
    
    long powerSupply = readPowerSupply();

    String	json = String("{\"cid\":");
    json.concat(_nodeIdentifier);
    json.concat(",\"lum\":");
    json.concat(stringLuminosity);
    json.concat(",\"hum\":");
    json.concat(humidity);
    json.concat(",\"temp\":");
    json.concat(temperature);
    json.concat(",\"s0\":");
    json.concat(stringSoil0);
    json.concat(",\"s1\":");
    json.concat(stringSoil1);
    json.concat(",\"s2\":");
    json.concat(stringSoil2);
    json.concat(",\"s3\":");
    json.concat(stringSoil3);
    json.concat(",\"power\":");
    json.concat(powerSupply);
    json.concat("}");
    
    return json;
}
