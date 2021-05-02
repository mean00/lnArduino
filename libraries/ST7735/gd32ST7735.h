/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#pragma once
#include "lnSPI.h"
#include "simplerST7735.h"
/**
 * 
 * @param w
 * @param h
 * @param spi
 * @param pinDc
 * @param pinCS
 */
class gd32ST7735 : public st7735
{
    public:
                     gd32ST7735(int w, int h, hwlnSPIClass *spi, int  pinDc, int pinCS);
             virtual ~gd32ST7735();
             virtual void init();
             virtual void sendByte(int byte); // 8 bytes
             virtual void sendWord(int byte); // 16 bytes
             virtual void sendBytes(int nb, const uint8_t *data); // 8 bits
             virtual void sendWords(int nb, const uint16_t *data); // 16 bits
             virtual void floodWords(int nb, const uint16_t data); // 16 bits
             virtual void updateHwRotation();
    protected:
            hwlnSPIClass *_spi;
};