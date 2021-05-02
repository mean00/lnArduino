/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#pragma once
#include "lnArduino.h"
/**
 */
class st7735Access
{
public:
    virtual void sendByte(int byte)=0; // 8 bytes
    virtual void sendWord(int byte)=0; // 16 bytes
    virtual void sendBytes(int nb, const uint8_t *data)=0; // 8 bits
    virtual void sendWords(int nb, const uint16_t *data)=0; // 16 bits
    virtual void updateHwRotation()=0;
    virtual void floodWords(int nb, const uint16_t data)=0; // 16 bits
};
/**
 */
class st7735 : public st7735Access
{
public:
                         st7735(int width, int height,  int pinDc,int pinCS);
                virtual ~st7735();
                virtual void init()=0;
                
                void fillScreen(int color); // 16 bits!
                void setAddress(int x, int y, int w, int h);
                void square(int color, int x, int y, int w, int g);
                void setRotation(int rotation);  // 0 1 2 3
            
protected:
                int _physicalWidth;
                int _physicalHeight;
                int _width;
                int _height;
                int _rotation;
                int _pinCS;
                int _pinDC;
                int _PhysicalXoffset;
                int _PhysicalYoffset;
                int _xOffset;
                int _yOffset;
protected:
                void csOn();
                void csOff();
                void sendCommand(uint8_t cmd, int size, const uint8_t *data);
                void baseInit();
};
