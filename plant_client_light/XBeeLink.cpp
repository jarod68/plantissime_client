
#include "XBeeLink.h"


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
    delay(1000);
    readSerial();
    
    printLine("AT");
    delay(1000);
    readSerial();
    
    printLine("ATNI");
    delay(1000);
    _xBeeIdentifier = readSerial();
    delay(4000);
    
}


String  XBeeLink :: getXBeeIdentifier       ()
{
    return _xBeeIdentifier;
}

void XBeeLink :: printLine         (const String& line)
{
    Serial.print(line);
    Serial.print('\r');
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
    delay(1000);
    readSerial();
    
    printLine("AT");
    delay(1000);
    readSerial();
    
    printLine("ATSM1");
    delay(1000);
    readSerial();
    delay(4000);
    
}
