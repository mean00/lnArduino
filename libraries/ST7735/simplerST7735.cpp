/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#include "simplerST7735.h"

/**
 * 
 */
void st7735::csOn()
{
    digitalWrite(_pinCS,LOW) ;
}
/**
 * 
 */
void st7735::csOff()
{
     digitalWrite(_pinCS,HIGH) ;
}

/**
 * 
 * @param cmd
 * @param size
 * @param data
 */
void st7735::sendCommand(uint8_t cmd, int size, const uint8_t *data)
{
        cmdMode();
        sendByte(cmd);
        if(size)
        {
            dataMode();
            sendBytes(size,data);
        }  
}
/**
 * 
 * @param width
 * @param height
 * @param pinDc
 * @param pinCS
 */
st7735::st7735(int width, int height,  int pinDc, int pinCS)
{
    _xOffset=0;
    _yOffset=0;
    _PhysicalXoffset=0;
    _PhysicalYoffset=0;
    _physicalWidth=width;
    _physicalHeight=height;
    _pinDC=pinDc;
    _pinCS=pinCS;
    _rotation=0;
    _width=_physicalWidth;
    _height=_physicalHeight;
     _fg=0xffff;
     _bg=0;
}
/**
 * 
 */
st7735::~st7735()
{

}
/*
 * 
 * @param color
 */              
void st7735::fillScreen(int color) // 16 bits!
{
    square(color,0,0,_width,_height);
}
/**
 * 
 * @param x
 * @param y
 * @param w
 * @param h
 */
void st7735::setAddress(int x, int y, int w, int h)
{
    int a1,a2,b1,b2;
    a1=x+_xOffset;
    a2=a1+w-1;
    b1=y+_yOffset;
    b2=b1+h-1;
    
      
      cmdMode();
      sendByte(0x2a);
      dataMode();
      sendWord(a1);
      sendWord(a2);
      cmdMode();
      sendByte(0x2b);
      dataMode();
      sendWord(b1);
      sendWord(b2);
      cmdMode();
      sendByte(0x2c);      
}
/**
 * 
 * @param color
 * @param x
 * @param y
 * @param w
 * @param h
 */
void st7735::square(int color, int x, int y, int w, int h)
{
    csOn();
    setAddress(x,y,w,h);
    int f=w*h;
    dataMode();
    floodWords(f,color);
    csOff();
}
/**
 * 
 * @param rotation
 */
void st7735::setRotation(int rotation)
{
    _rotation=rotation;
    switch(rotation)
    {
        case 0:
        case 2:
                _xOffset=_PhysicalXoffset;
                _yOffset=_PhysicalYoffset;
                _width=_physicalWidth;
                _height=_physicalHeight;                
                break;
        case 1:
        case 3:	
                _xOffset=_PhysicalYoffset;
                _yOffset=_PhysicalXoffset;
                _width=_physicalHeight;
                _height=_physicalWidth;                
                break;
        default : xAssert(0);
                break;
    }
    updateHwRotation();
}
/**
 * 
 */
void st7735::baseInit()
{
    pinMode(_pinDC,OUTPUT);
    pinMode(_pinCS,OUTPUT);
    
    digitalWrite(_pinDC,HIGH);
    digitalWrite(_pinCS,HIGH);
}
// EOF