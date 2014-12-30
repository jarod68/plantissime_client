/* ---------------------------------------------------------------------------
 ** Author: Matthieu Holtz
 ** Year:   2015
 ** -------------------------------------------------------------------------*/

#include "XBeeLink.h"

#define __ASSERT_USE_STDERR

#include <assert.h>

#define XBEE_TIME_AFTER_CMD_MS 1000

XBeeLink :: XBeeLink        () 
{
    this->readNodeIdentifier();
}

XBeeLink :: ~XBeeLink       ()
{

}

void XBeeLink :: readNodeIdentifier ()
{
    Serial.print("+++");
    delay(XBEE_TIME_AFTER_CMD_MS);
    assert(readSerial() == "OK");
    
    printLineCR("AT");
    delay(XBEE_TIME_AFTER_CMD_MS);
    assert(readSerial() == "OK");
    
    printLineCR("ATNI");
    delay(XBEE_TIME_AFTER_CMD_MS);
    _xBeeIdentifier = readSerial();
    
    printLineCR("ATCN");
    delay(XBEE_TIME_AFTER_CMD_MS);
    assert(readSerial() == "OK");
}


String  XBeeLink :: getXBeeIdentifier       ()
{
    return _xBeeIdentifier;
}

void XBeeLink :: printLineCR         (const String& line)
{
    Serial.print(line);
    Serial.print('\r');
}

void XBeeLink :: printLineLF         (const String& line)
{
    Serial.print(line);
    Serial.print('\n');
}

String XBeeLink :: readSerial   ()
{
    String str = "";
    
    while (true) {
        
        if(!Serial.available())
            continue;
        
        char inChar = (char)Serial.read();
        
        if (inChar == '\r')
            return str;
        
        else if (inChar != '\n')
            str += inChar;
    }
}

void XBeeLink :: configureSleepOnD7()
{
    Serial.print("+++");
    delay(XBEE_TIME_AFTER_CMD_MS);
    assert(readSerial() == "OK");
    
    printLineCR("AT");
    delay(XBEE_TIME_AFTER_CMD_MS);
    assert(readSerial() == "OK");
    
    printLineCR("ATSM1");
    delay(XBEE_TIME_AFTER_CMD_MS);
    assert(readSerial() == "OK");
    
    printLineCR("ATCN");
    delay(XBEE_TIME_AFTER_CMD_MS);
    assert(readSerial() == "OK");
}
