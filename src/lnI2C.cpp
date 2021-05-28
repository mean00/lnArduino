/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#include "lnArduino.h"
#include "lnI2C.h"
#include "lnI2C_priv.h"

static const LN_I2C_Registers *aI2C0=(LN_I2C_Registers *)LN_I2C0_ADR;
static const LN_I2C_Registers *aI2C1=(LN_I2C_Registers *)LN_I2C1_ADR;


struct LN_I2C_DESCRIPTOR
{
    LN_I2C_Registers *adr;
    lnPin           _scl;
    lnPin           _sda;
};

static const LN_I2C_DESCRIPTOR i2c_descriptors[2]=
{
    {(LN_I2C_Registers *)LN_I2C0_ADR,PB6,PB7},
    {(LN_I2C_Registers *)LN_I2C1_ADR,PB10,PB11},
};

#define LN_I2C_STAT0_ERROR_MASK (LN_I2C_STAT0_LOSTARB | LN_I2C_STAT0_AERR | LN_I2C_STAT0_BERR | LN_I2C_STAT0_OUERR)

bool  waitStat0BitSet(LN_I2C_Registers *reg, uint32_t bit)
{
    while(1)
    {
        uint32_t v=(reg->STAT0);
        if(v&bit) return true;
        if(v&(LN_I2C_STAT0_ERROR_MASK))
        {
         //   Logger("I2C:Stat error 0x%x\n",v);
            // clear error
            v&=~LN_I2C_STAT0_ERROR_MASK;
            reg->STAT0=v;            
            return false;
        }
    }
}

/**
 * 
 * @param instance
 * @param speed
 */
lnTwoWire::lnTwoWire(int instance, int speed)
{
    _instance=instance;
    _speed=speed;
}
/**
 * 
 */
lnTwoWire::~lnTwoWire()
{
}
/**
 * 
 * @param target
 * @return 
 */
bool lnTwoWire::begin(int target)
{
    // reset peripheral
    Peripherals periph=(Peripherals)(pI2C0+_instance);
    const LN_I2C_DESCRIPTOR *d=i2c_descriptors+_instance;
    LN_I2C_Registers *adr=d->adr;
    
    lnPeripherals::reset(periph);
    adr->CTL0=LN_I2C_CTL0_SRESET;
    xDelay(1);
    adr->CTL0=0;
    if(target)
        _targetAddress=target;
    
    // set pin
    lnPinMode( d->_scl,  lnALTERNATE_OD);
    lnPinMode( d->_sda,  lnALTERNATE_OD);    
    // back to default
    
    adr->CTL1= lnPeripherals::getClock(periph)/1000000;; // 54 mhz apb1 max
    int speed=lnPeripherals::getClock(periph);
    if(!_speed) speed=speed/(100*1000); // default speed is 100k
    else speed=speed/_speed;
    
#define SPDMAX ((1<<12)-1)
    if(speed>SPDMAX) 
        speed=SPDMAX;
    adr->CKCFG=speed;
    adr->CTL0=LN_I2C_CTL0_I2CEN; // And go!
    
    return true;
}

/**
 * 
 * @param speed
 */
void lnTwoWire::setSpeed(int speed)
{
    const LN_I2C_DESCRIPTOR *d=i2c_descriptors+_instance;
    LN_I2C_Registers *adr=d->adr;
    
    
}
/**
 * 
 * @param target
 * @param n
 * @param data
 * @return 
 */
bool lnTwoWire::write(int target, int n, uint8_t *data)
{
    volatile uint32_t stat1;
    // Send start
    const LN_I2C_DESCRIPTOR *d=i2c_descriptors+_instance;
    LN_I2C_Registers *adr=d->adr;
    adr->CTL0|=LN_I2C_CTL0_START; // send start    
    waitStat0BitSet(adr,LN_I2C_STAT0_SBSEND);
    // Send address
    adr->DATA=((_targetAddress & 0x7F)<<1);
    if(!waitStat0BitSet(adr,LN_I2C_STAT0_ADDSEND)) return false;
    stat1=adr->STAT1;
    
    // wait for completion
    
    while(n)
    {
        // wait for Tx ready
        if(!waitStat0BitSet(adr,LN_I2C_STAT0_TBE)) return false;
        
        // send next
        adr->DATA=*data;
        
        // next
        data++;
        n--;
    }
    // wait for BTC
    if(!waitStat0BitSet(adr,LN_I2C_STAT0_BTC)) return false;
    // send stop
    adr->CTL0|=LN_I2C_CTL0_STOP;     
    while(adr->CTL0&LN_I2C_CTL0_STOP)
    {
        
    }
    
    return true;
}
/**
 * 
 * @param target
 * @param n
 * @param data
 * @return 
 */
bool lnTwoWire::read(int target,  int n, uint8_t *data)
{
     volatile uint32_t stat1;
    // Send start
    const LN_I2C_DESCRIPTOR *d=i2c_descriptors+_instance;
    LN_I2C_Registers *adr=d->adr;
    adr->CTL0|=LN_I2C_CTL0_START; // send start    
    waitStat0BitSet(adr,LN_I2C_STAT0_SBSEND);
    // Send address
    adr->DATA=((_targetAddress & 0x7F)<<1)+1;
    if(!waitStat0BitSet(adr,LN_I2C_STAT0_ADDSEND)) return false;
    stat1=adr->STAT1;
    
    // wait for completion
    
    
    while(n)
    {
        // wait for Tx ready
        if(!waitStat0BitSet(adr,LN_I2C_STAT0_RBNE)) return false;
        
        // send next
        uint32_t v=adr->DATA;
        *data=v&0xff;
        // next
        data++;
        n--;
    }
    // send stop
    adr->CTL0|=LN_I2C_CTL0_STOP;     
    while(adr->CTL0&LN_I2C_CTL0_STOP)
    {
        
    }
    
    return true;
}


void   waitCtl0BitSet(uint32_t bit)
{
    
}

// EOF

