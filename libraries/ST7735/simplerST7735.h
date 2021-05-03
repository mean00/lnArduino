/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#pragma once
#include "lnArduino.h"
#include "gfxfont.h"

struct _current_font
{
	uint8_t* font;
	uint8_t x_size;
	uint8_t y_size;
	uint8_t offset;
	uint8_t numchars;
	uint8_t inverted;
};


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
               enum FontSize
               {
                   SmallFont,MediumFont,BigFont
               };
               class FontInfo
               {
               public:
                 int               maxHeight;          
                 int               maxWidth;
                 const GFXfont    *font;        
               };  
               FontInfo          fontInfo[3];

               FontInfo          *currentFont;
               const GFXfont     *gfxFont;
               int               _fg,_bg;

    
    
                         st7735(int width, int height,  int pinDc,int pinCS);
                virtual ~st7735();
                virtual void init()=0;
                
                void fillScreen(int color); // 16 bits!
                void setAddress(int x, int y, int w, int h);
                void square(int color, int x, int y, int w, int g);
                void setRotation(int rotation);  // 0 1 2 3
                void push2Colors(uint8_t *data, int len, boolean first,uint16_t fg, uint16_t bg);
            
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
                
public: // freetype font
                int     myDrawChar(int x, int y, unsigned char c,  int color, int bg,FontInfo &infos);
                void    square(int x,int y,int w, int h, bool color);
                void    setFontSize(FontSize size);
                void    setFontFamily(const GFXfont *small, const GFXfont *medium, const GFXfont *big);
                void    print(int x,int y,const char *z);
                int     writeChar(char c);
                void    drawRLEBitmap(int widthInPixel, int height, int wx, int wy, int fgcolor, int bgcolor, const uint8_t *data);
                
protected:
                int     mySquare(int x, int y, int w, int xheight, uint16_t filler);
                void    pushColors16(uint16_t *data,int nb);
#define         ST7735_BUFFER_SIZE_WORD 256
                uint16_t scrbuf[ST7735_BUFFER_SIZE_WORD];
                int     cursor_x,cursor_y;


};


#define cmdMode()  digitalWrite(_pinDC,LOW)
#define dataMode() digitalWrite(_pinDC,HIGH)

//EOF
