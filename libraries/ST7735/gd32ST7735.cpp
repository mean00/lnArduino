/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#include "gd32ST7735.h"
#include "simplerST7735_priv.h"
/**
 * 
 * @param w
 * @param h
 * @param spi
 * @param pinDc
 * @param pinCS
 */
gd32ST7735::gd32ST7735(int w, int h, hwlnSPIClass *spi, int  pinDc, int pinCS) :  st7735(w,h,pinDc,pinCS)
{

    _spi=spi;
    _PhysicalXoffset=1;
    _PhysicalYoffset=26;
    
}
/**
 * 
 */
gd32ST7735::~gd32ST7735()
{

}
// This is the init sequence for the 160x80 LCd on the longan nano
void gd32ST7735::init()
{
  baseInit();


    // Initialization settings. Based on lcd.c in gd32v_lcd example.
  static const uint8_t init_sequence[] =
  {
      0,ST7735_INVON,    0xff,
      3,ST7735_FRMCTR1,  0x05, 0x3a, 0x3a, 0xff,
      3,ST7735_FRMCTR2,  0x05, 0x3a, 0x3a, 0xff,
      6,ST7735_FRMCTR3,  0x05, 0x3a, 0x3a, 0x05, 0x3a, 0x3a, 0xff,
      1,ST7735_INVCTR,   0x03, 0xff,
      3,ST7735_PWCTR1,   0x62, 0x02, 0x04, 0xff,
      1,ST7735_PWCTR2,   0xc0, 0xff,
      2,ST7735_PWCTR3,   0x0d, 0x00, 0xff,
      2,ST7735_PWCTR4,   0x8d, 0x6a, 0xff,
      2,ST7735_PWCTR5,   0x8d, 0xee, 0xff,
      1,ST7735_VMCTR1,   0x0e, 0xff,
      16,ST7735_GMCTRP1, 0x10, 0x0e, 0x02, 0x03, 0x0e, 0x07, 0x02, 0x07, 0x0a, 0x12, 0x27, 0x37, 0x00, 0x0d, 0x0e, 0x10, 0xff,
      16,ST7735_GMCTRN1, 0x10, 0x0e, 0x03, 0x03, 0x0f, 0x06, 0x02, 0x08, 0x0a, 0x13, 0x26, 0x36, 0x00, 0x0d, 0x0e, 0x10, 0xff,
      1,ST7735_COLMOD  , 0x55, 0xff, // 0x3a: 0x6 -> 18 bits /pix, 0x5-> 15 bit/pixel
      1,ST7735_MADCTL,   0x78, 0xff, // 0x36: coord 08..168 / 120...200
      0,ST7735_DISPON,   0xff,
      0,ST7735_SLPOUT,   0xff,
      0xff
  };
  const uint8_t *gp=init_sequence;
  digitalWrite(_pinCS,LOW);
  while(1)
  {
      int size=gp[0];
      if(size==0xff) break;
      xAssert(gp[size+2]==0xff)
      sendCommand(gp[1],size,gp+2);
      gp+=size+3;        
  }   
  digitalWrite(_pinCS,HIGH);    
}
/**
 * 
 * @param byte
 */
void gd32ST7735::sendByte(int byte)
{
    _spi->write(byte);
}
/**
 * 
 * @param byte
 */
void gd32ST7735::sendWord(int byte)
{
    _spi->write16(byte);
}
/**
 * 
 * @param nb
 * @param data
 */
void gd32ST7735::sendBytes(int nb, const uint8_t *data)
{
    _spi->write(nb,data);
}
/**
 * 
 * @param nb
 * @param data
 */
void gd32ST7735::sendWords(int nb, const uint16_t *data)
{
    for(int i=0;i<nb;i++)
        _spi->write16(data[i]);
}
/**
* 
* @param nb
* @param data
*/
void gd32ST7735::floodWords(int nb, const uint16_t data)
{
  for(int i=0;i<nb;i++)
      sendWord(data);
}
  
static const uint8_t rotMode[4]={0x8,0xc8,0x78,0xa8};
/**
 * 
 */
void gd32ST7735::updateHwRotation(void)
{
    sendCommand(ST7735_MADCTL,1,rotMode+_rotation);
}

// EOF