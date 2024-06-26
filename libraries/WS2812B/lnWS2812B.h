/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#pragma once
#include "lnSPI.h"
#include "lnWS2812B_base.h"
/**
 *
 * @param nbLeds
 * @param s
 */
class WS2812B : public WS2812B_base
{
  public:
    WS2812B(int nbLeds, lnSPI *s);
    virtual ~WS2812B();

    void begin();                                   // call this first
    void setGlobalBrightness(int value);            // between 0 & 255
    void setColor(int r, int g, int b);             // set all the same color
    void setLedColor(int led, int r, int g, int b); // set only one led
    void setLedBrightness(int led, int brightness); // set only one led
    void update();                                  // call this to have the changes committed

  protected:
    lnSPI *_spi;
    uint8_t *_ledsColorSPI;

  protected:
    void convert(int led);
    void convertAll();
};
