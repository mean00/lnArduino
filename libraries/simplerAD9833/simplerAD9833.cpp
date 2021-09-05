/*************************************************** 
 This is a library for the MCP23017 i2c port expander

 These displays use I2C to communicate, 2 pins are required to
 interface
 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries.
 BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "Arduino.h"
#include "simplerAD9833.h"
#include "simplerAD9833_internal.h"
/**
 * 
 * @param spi
 * @param cs
 */
simplerAD9833::simplerAD9833(hwlnSPIClass *spi,lnPin cs)
{
    _spi=spi;
    _cs=cs;
    if(_cs!=-1)
    {
        digitalWrite(_cs,true);
        pinMode(_cs,OUTPUT);
    }
    _waveform=Sine;
    _frequency=1000;
    _state=0;
    _spiSettings=new lnSPISettings(1*1000,SPI_MSBFIRST,SPI_MODE2,_cs);
}
/**
 * 
 */
simplerAD9833::~simplerAD9833()
{
    _spi=NULL; 
    delete _spiSettings;
    _spiSettings=NULL;
}        
/**
 * 
 * @param addr
 * @return 
 */
int   simplerAD9833::readRegister(int addr)
{
    return 0;
}
/**
 * 
 * @param wf
 */
void              simplerAD9833::setWaveForm(WaveForm wf)
{
    _state&=~(LN_AD9833_OPBITEN+LN_AD9833_DIV2+LN_AD9833_MODE);
    _waveform=wf;
#define TTT(op,mode,div2)    {_state+=op*LN_AD9833_OPBITEN+mode*LN_AD9833_MODE+LN_AD9833_DIV2*div2;}
    switch(_waveform)
    {
        case Square:
            TTT(1,0,1);
            break;
        case Triangle:
            TTT(0,1,0);
            break;            
        case Sine:
            TTT(0,0,0);
            break;            
    }
    writeRegister(0,_state);
}


/**
 * 
 */
void   simplerAD9833::enable()
{
    _state &=~(LN_AD9833_SLEEP1+LN_AD9833_SLEEP12);
    writeRegister(0,_state);
}
/**
 * 
 */
void   simplerAD9833::disable()
{
    _state |=(LN_AD9833_SLEEP1+LN_AD9833_SLEEP12);
    writeRegister(0,_state);
}

/**
 * 
 * @param addr
 * @param value
 */
void simplerAD9833::writeRegister(int addr, int value)
{
    _spi->beginTransaction(*_spiSettings);
    //_spi->write16(0*0x55aa+1*value);
    _spi->write(value>>8);
    _spi->write(value&0xff);
    _spi->endTransaction();
}
  
/**
 * 
 * @param fq
 */

const float mulmul=((float)(1<<28));

void simplerAD9833::setFrequency(int fq)
{
    _frequency=fq;
    // Default frequency : 25 Mhz
    float baseFq=25.*1000.*1000.;
    if(fq>baseFq) fq=baseFq;
    if(!fq) fq=1;
    
    
    float dividerf=(fq*mulmul)/baseFq;
    int divider=(int)(dividerf+.49);
    int High14=divider>>14;
    int Low14=divider&0x3FFF;
    
    // always right Freq 0 ?
     writeRegister(0, LN_AD9833_B28 | LN_AD9833_CTRL(0));
     // Now we can write the fq
     writeRegister(0,Low14 | LN_AD9833_CTRL(1));
     writeRegister(0,High14| LN_AD9833_CTRL(1));
     
    
    
    
}  
// EOF


