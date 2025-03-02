//
#include "Arduino.h"
#include "PNP_compressed.h"
#include "PNP_decl.h"
#include "gd32ST7735.h"
#include "lnI2C.h"
#include "lnSPI.h"
#include "lnWS2812B.h"
#include "ssd1306_i2c_lnGd32vf103.h"

// green = PA1, blue = PA2, RED PC13
#if defined(USE_RP2040) || defined(USE_RP2350)
#define LED GPIO10
#define LEDRED GPIO11
#define SPEED (400 * 1000)
#include "FreeSans7pt7b.h"

#define NB_WS2812B 12

#define PINDC GFIO13
#define PINCS GPIO14
#define PINRST GPIO15
#else
#define LED PA2
#define LEDRED PC13
#define SPEED (400 * 1000)
#include "FreeSans7pt7b.h"

#define NB_WS2812B 12

#define PINDC PB0
#define PINCS PB2
#define PINRST PB1
#endif
#define byte uint8_t
/**
 *
 */
void setup()
{
}
/**
 *
 * @param
 */

void Wheel(byte WheelPos, int &r, int &g, int &b)
{
    if (WheelPos < 85)
    {
        r = WheelPos * 3;
        b = 255 - WheelPos * 3;
        g = 0;
        return;
    }
    if (WheelPos < 170)
    {
        WheelPos -= 85;
        r = 255 - WheelPos * 3;
        g = 0;
        b = WheelPos * 3;
        return;
    }
    WheelPos -= 170;
    r = 0;
    g = WheelPos * 3;
    b = 255 - WheelPos * 3;
}
/**
 *
 * @param
 */

void wheelDemo(WS2812B *ws)
{
    int r, g, b;
    int before, after;

    int brightness = 255;
    int dir = -5;

    while (1)
    {

        for (int j = 0; j < 256; j++)
        {
            before = micros();
            for (int i = 0; i < NB_WS2812B; i++)
            {
                int pos = (i + j) & 0xff;
                Wheel(pos, r, g, b);
                ws->setLedColor(i, r, g, b);
            }
            after = micros();
            // Logger("Convert = %d\n",after-before);
            before = micros();

            after = micros();
            // Logger("Convert2 = %d\n",after-before);
            xDelay(20);
            // ws->setGlobalBrightness(brightness);
            brightness += dir;
            if (brightness == 0 && dir < 0)
                dir = -dir;
            if (brightness == 255 && dir > 0)
                dir = -dir;
            ws->update();
        }
    }
}
/**
 *
 * @param
 */

void pulse(WS2812B *ws)
{
    int brightness = 127;
    int dir = -1;
    ws->setColor(0xff, 0, 0); // RED G* R* B*
    while (1)
    {

        ws->setGlobalBrightness(brightness);
        brightness += dir;
        if (brightness == 0 && dir < 0)
            dir = -dir;
        if (brightness == 127 && dir > 0)
            dir = -dir;
        ws->update();
        xDelay(10);
    }
}
#define VAL 0x0f
uint8_t single[3][3] = {{0, 0, VAL}, {0, VAL, 0}, {VAL, 0, 0}};
/**
 *
 * @param
 */

void dot(WS2812B *ws)
{
    int dex = 0, olddex = 0;
    int r, g, b;
    int color = 0;
    uint8_t *start = &(single[0][0]);
    for (int i = 0; i < NB_WS2812B; i++)
    {
        uint8_t *t = start + (i % 3) * 3;

        ws->setLedColor(i, t[0], t[1], t[2]); // g r b
        ws->setLedBrightness(i, (255 * ((i % 3) + 1)) / 4);
    }
    ws->update();
    while (1)
    {
    }

    while (1)
    {
        // ws->setLedBrightness(olddex,10);
        dex++;
        // if(dex>=NB_WS2812B) dex=0;
        // ws->setLedBrightness(dex,255);
        ws->setGlobalBrightness(dex & 0xff);
        olddex = dex;
        ws->update();
        xDelay(80);
    }
}
/**
 *
 * @param
 */

void singleValue(WS2812B *ws, int v)
{
    Logger("Value =%d\n", v);
    ws->setColor(v, 0, 0); // r g b
    ws->update();
}
/**
 *
 * @param
 */

void colors(WS2812B *ws)
{

    int dex = 0;
    while (1)
    {
        // ws->setLedBrightness(olddex,10);
        dex += 1;
        if (dex >= 127)
            dex = 0;
        ws->setColor(dex, 0, 0); // r g b
        ws->update();
        Logger("Value =%d\n", dex);
        xDelay(20);
    }
}

/**
 *
 * @param
 */

void ssd1306Task(void *a)
{
    lnI2C *i2c = new lnI2C(0, SPEED);
    i2c->begin(0x3c);
    OLED_lnGd32 *ssd1306 = new OLED_lnGd32(*i2c, -1);
    ssd1306->begin();
    ssd1306->setFontFamily(&FreeSans7pt7b, &FreeSans7pt7b, &FreeSans7pt7b);
    ssd1306->setFontSize(OLEDCore::SmallFont);
    while (1)
    {
        Logger("Loop..\n");
        ssd1306->clrScr();
        ssd1306->square(100, 20, 30, 30, true);
        ssd1306->print(2, 40, "Demo");
        ssd1306->update();
        xDelay(500);
        ssd1306->clrScr();
        ssd1306->square(2, 20, 30, 30, true);
        ssd1306->print(80, 40, "Demo");
        ssd1306->update();
        xDelay(500);
    }
}

/**
 *
 * @param
 */
void ws2812bTask(void *)
{
    hwlnSPIClass *spi = new hwlnSPIClass(1);
    WS2812B *ws = new WS2812B(NB_WS2812B, spi);
    ws->begin();

    wheelDemo(ws);
}
/**
 *
 * @param
 */
void lcdST7735Task(void *)
{
    hwlnSPIClass *spi = new hwlnSPIClass(0, -1);
    spi->begin();
    lnSPISettings transaction(30 * 1000 * 1000, SPI_MSBFIRST, SPI_MODE0, -1);
    spi->beginTransaction(transaction);
    // Reset LCD
    pinMode(PINRST, OUTPUT);
    digitalWrite(PINRST, HIGH);
    xDelay(50);
    digitalWrite(PINRST, LOW);
    xDelay(50);
    digitalWrite(PINRST, HIGH);

    gd32ST7735 *lcd = new gd32ST7735(160, 80, spi, PINDC, PINCS);
    lcd->init();
    lcd->setRotation(2);
    lcd->fillScreen(0);

    lcd->drawRLEBitmap(PNP_width, PNP_height, 0, 0, 0x1f << 6, 0x1f << 11, PNP);

    // init fonts
    lcd->setFontFamily(&FreeSans7pt7b, &FreeSans7pt7b, &FreeSans7pt7b);
    lcd->setFontSize(st7735::SmallFont);

    int r = 0;
    int roundup;
    bool onoff;
    while (1)
    {
        roundup++;
        vTaskDelay(1200);
        digitalToggle(LED);
        onoff = !onoff;
        if (onoff)
        {
            lcd->square(0x1f, 10, 20, 40, 20);
            lcd->print(20, 20, "This is  a test");
        }
        else
        {
            lcd->square(0xffff, 10, 20, 40, 20);
        }
        r++;
        r &= 15;
        lcd->setRotation(r >> 2);
        Logger("Blue\n");
    }
}
/**
 *
 */
void loop()
{
    Logger("Everything demo...\n");
    Logger("Starting SSD1306...\n");
    xTaskCreate(ssd1306Task, "ssd1306Task", 500, NULL, 2, NULL);
    Logger("Starting WS2812B...\n");
    xTaskCreate(ws2812bTask, "ws2812bTask", 500, NULL, 2, NULL);
    Logger("Starting lcdST7735...\n");
    xTaskCreate(lcdST7735Task, "lcdST7735Task", 500, NULL, 2, NULL);

    while (1)
    {
        xDelay(500);
        Logger("Im still alive\n");
    }
}
// EOF
