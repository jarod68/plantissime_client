/* ---------------------------------------------------------------------------
 ** Author: Matthieu Holtz
 ** Year:   2015
 ** -------------------------------------------------------------------------*/

#pragma once

#ifndef XBEELINK_H
#define XBEELINK_H

#include <Arduino.h>


class XBeeLink {
    
private:
    String _xBeeIdentifier;
    
private:
    void  readNodeIdentifier                ();
    
public:
    XBeeLink                                ();
    
    virtual         ~XBeeLink               ();
    
    virtual String  getXBeeIdentifier       ();
    virtual void    printLineCR             (const String& line);
    virtual void    printLineLF             (const String& line);
    virtual String  readSerial              ();
    virtual void    configureSleepOnD7      ();
};


#endif //XBEELINK_H