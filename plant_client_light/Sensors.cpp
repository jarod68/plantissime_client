
#include "Sensors.h"


Sensors :: Sensors      (const uint16_t pinSoil_0,
                         const uint16_t pinSoil_1,
                         const uint16_t pinSoil_2,
                         const uint16_t pinSoil_3,
                         const uint16_t pinDHT11
                        ):
                        _pinSoil_0(pinSoil_0),
                        _pinSoil_1(pinSoil_1),
                        _pinSoil_2(pinSoil_2),
                        _pinSoil_3(pinSoil_3),
                        _pinDHT11(pinDHT11),
                        _tsl(new TSL2561(TSL2561_ADDR_FLOAT)),
                        _dht11(new dht11)
{
    uint16_t i = 0;
    while (_tsl->begin() && i<5)
        i++;
    
    _tsl->setGain(TSL2561_GAIN_16X);     // set 16x gain (for dim situations)
    
    _tsl->setTiming(TSL2561_INTEGRATIONTIME_13MS);
}

Sensors :: ~Sensors()
{
    delete _tsl;
    delete _dht11;
}

int Sensors::readSoil    (const uint16_t analogPin)
{
    int value = analogRead(analogPin);
    
    if (value >= 1000)
        return -1;
    
    return value;
    
}

float       Sensors::readTSL     ()
{
    return _tsl->getLuminosity(TSL2561_VISIBLE);
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
    
    String	json = String("{\"cid\":");
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
    json.concat("}");
    
    return json;
}
