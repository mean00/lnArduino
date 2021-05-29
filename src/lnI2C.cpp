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
    LnIRQ           _irqEv;
    LnIRQ           _irqErr;
};

static const LN_I2C_DESCRIPTOR i2c_descriptors[2]=
{
    {(LN_I2C_Registers *)LN_I2C0_ADR,PB6,PB7,LN_IRQ_I2C0_EV,LN_IRQ_I2C0_ER},
    {(LN_I2C_Registers *)LN_I2C1_ADR,PB10,PB11,LN_IRQ_I2C1_EV,LN_IRQ_I2C1_ER}
};

#define LN_I2C_STAT0_ERROR_MASK (LN_I2C_STAT0_LOSTARB | LN_I2C_STAT0_AERR | LN_I2C_STAT0_BERR | LN_I2C_STAT0_OUERR)
/**
 */
class lnI2CSession 
{
public:
    int target;
    int nbTransaction;
    int *transactionSize;
    uint8_t **transactionData;
    int     curTransaction;
    int     curIndex;
};



lnTwoWire *irqHandler[2]={NULL,NULL};
/**
 * 
 */
void lnTwoWire::startIrq()
{
    
    xAssert(irqHandler[_instance]==NULL);
    irqHandler[_instance]=this;
    
    lnEnableInterrupt(_d->_irqErr);
    lnEnableInterrupt(_d->_irqEv);
     LN_I2C_Registers *adr=_d->adr;
     adr->CTL1|=(LN_I2C_CTL1_ERRIE | LN_I2C_CTL1_EVIE | LN_I2C_CTL1_BUFIE);
}
/**
 * 
 */
void lnTwoWire::stopIrq()
{
    lnDisableInterrupt(_d->_irqErr);
    lnDisableInterrupt(_d->_irqEv);
    _d->adr->CTL1&=~(LN_I2C_CTL1_ERRIE | LN_I2C_CTL1_EVIE | LN_I2C_CTL1_BUFIE);
     irqHandler[_instance]=NULL;
}
/**
 * 
 * @param reg
 * @param bit
 * @return 
 */
bool  waitStat0BitSet(LN_I2C_Registers *reg, uint32_t bit)
{
    while(1)
    {
        uint32_t v=(reg->STAT0);
        if(v&bit) return true;
        if(v&(LN_I2C_STAT0_ERROR_MASK))
        {
            Logger("I2C:Stat error 0x%x\n",v);
            // clear error
            v&=~LN_I2C_STAT0_ERROR_MASK;
            reg->STAT0=v;            
            return false;
        }
    }
}
bool  waitStat0BitClear(LN_I2C_Registers *reg, uint32_t bit)
{
    while(1)
    {
        uint32_t v=(reg->STAT0);
        if(!(v&bit)) return true;
        if(v&(LN_I2C_STAT0_ERROR_MASK))
        {
            Logger("I2C:Stat error 0x%x\n",v);
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
    _d=i2c_descriptors+_instance;    
    _session=NULL;
}
/**
 * 
 */
lnTwoWire::~lnTwoWire()
{
      _session=NULL;
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
    
    LN_I2C_Registers *adr=_d->adr;
    
    lnPeripherals::reset(periph);
    adr->CTL0=LN_I2C_CTL0_SRESET;
    xDelay(1);
    adr->CTL0=0;
    if(target)
        _targetAddress=target;
    
    // set pin
    lnPinMode( _d->_scl,  lnALTERNATE_OD);
    lnPinMode( _d->_sda,  lnALTERNATE_OD);    
    // back to default
    
    adr->CTL1=0;
    setSpeed(_speed);    
    adr->CTL0=LN_I2C_CTL0_I2CEN; // And go!
    
    return true;
}

/**
 * 
 * @param speed
 */
void lnTwoWire::setSpeed(int newSpeed)
{
    
    LN_I2C_Registers *adr=_d->adr;  
    Peripherals periph=(Peripherals)(pI2C0+_instance);
    
    uint32_t ctl1=adr->CTL1;
    ctl1&=~(0x3f);
    ctl1|=lnPeripherals::getClock(periph)/1000000;; // 54 mhz apb1 max
    adr->CTL1= ctl1;
    _speed=newSpeed;
    
    int speed=lnPeripherals::getClock(periph);
    if(!_speed) speed=speed/(100*1000); // default speed is 100k
    else speed=speed/newSpeed;
    
#define SPDMAX ((1<<12)-1)
    if(speed>SPDMAX) 
        speed=SPDMAX;
    adr->CKCFG=speed;
    
}
/**
 */
bool lnTwoWire::multiWrite(int target, int nbSeqn,int *seqLength, uint8_t **seqData)
{
    volatile uint32_t stat1;
    // Send start
    
    LN_I2C_Registers *adr=_d->adr;
   
#if 1
    
    lnI2CSession session;
    session.target=target;
    session.nbTransaction=nbSeqn;
    session.transactionSize=seqLength;
    session.transactionData=seqData;
    session.curTransaction=0;
    session.curIndex=0;
    _session=&session;
    _txState=I2C_TX_START;
    adr->CTL0|=LN_I2C_CTL0_START; // send start    
    // enable interrupt
    startIrq();
    _sem.take();
    stopIrq();
    _session=NULL;
    if(_result)
    {
          if(!waitStat0BitClear(adr,LN_I2C_CTL0_STOP)) _result=false;
    }
    return _result;
#else    
    adr->CTL0|=LN_I2C_CTL0_START; // send start    
    waitStat0BitSet(adr,LN_I2C_STAT0_SBSEND);
    // Send address
    adr->DATA=((target & 0x7F)<<1);
    if(!waitStat0BitSet(adr,LN_I2C_STAT0_ADDSEND)) return false;
    stat1=adr->STAT1;
    
    // wait for completion
    int seq=0;
    while(nbSeqn)
    {
        int n=seqLength[seq];
        uint8_t *data=seqData[seq];
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
        nbSeqn--;
        seq++;
    }
    // wait for BTC
    if(!waitStat0BitSet(adr,LN_I2C_STAT0_BTC)) return false;
    // send stop
    adr->CTL0|=LN_I2C_CTL0_STOP;  
    if(!waitStat0BitClear(adr,LN_I2C_CTL0_STOP)) return false;
#endif    
    return true;
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
    return multiWrite(target,1,&n,&data);
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
    
    LN_I2C_Registers *adr=_d->adr;
    adr->CTL0|=LN_I2C_CTL0_START; // send start    
    waitStat0BitSet(adr,LN_I2C_STAT0_SBSEND);
    // Send address
    adr->DATA=((target & 0x7F)<<1)+1;
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
/**
 * 
 * @return  true if we continue, false if all has been transferred
 */
bool lnTwoWire::sendNext()
{    
    int t=_session->curTransaction;
    uint8_t  *data=_session->transactionData[t];
    int      size=_session->transactionSize[t];
    
    _d->adr->DATA=data[_session->curIndex++];
    if(_session->curIndex<=size) // next transaction
        return true;
    // next transaction
    _session->curTransaction++;
    if(_session->curTransaction>_session->nbTransaction)
    {
        // all done!
        return false;
    }
    _session->curIndex=0; // start next transaction
    return true;
}

/**
 * 
 * evt:
 *      0 : Event
 *      1:  Error
 * @param evt
 * @return 
 */
volatile uint32_t lastI2cStat;
void lnTwoWire::irq(int evt)
{
    xAssert(_session);
    switch(evt)
    {
        case 1: // error
                lnDisableInterrupt(_d->_irqErr);
                lnDisableInterrupt(_d->_irqEv);
                _result=false;
                _sem.give();
                return;
                break;
        case 0:
                break;
        default:
                xAssert(0);
                break;
    }    
    lastI2cStat=(_d->adr->STAT0);
    volatile uint32_t v1;
    switch(_txState)
    {
        case I2C_TX_START:                  
                  xAssert(lastI2cStat&LN_I2C_STAT0_SBSEND);
                  _d->adr->DATA=((_session->target & 0x7F)<<1); 
                  _txState=I2C_TX_ADDR_SENT;
                  return;
                  break;
        case I2C_TX_ADDR_SENT:
                  xAssert(lastI2cStat&LN_I2C_STAT0_ADDSEND);
                  v1=_d->adr->STAT1;// Clear bit
                  xAssert(_d->adr->STAT0 & LN_I2C_STAT0_TBE);
                  if(!sendNext())
                  {
                      _d->adr->CTL1&=~( LN_I2C_CTL1_BUFIE);
                      _txState=I2C_TX_END;
                  }
                  else
                      _txState=I2C_TX_DATA;
                  return;
                  break;
            
        case I2C_TX_DATA:
                   xAssert(_d->adr->STAT0 & LN_I2C_STAT0_TBE);
                   if(!sendNext())
                   {
                      _txState=I2C_TX_STOP;
                       _d->adr->CTL1&=~( LN_I2C_CTL1_BUFIE);
                   }
                  else
                  {
                      _txState=I2C_TX_DATA;
                  }
                  return;
                  break;
        case I2C_TX_STOP:
                    xAssert(lastI2cStat&LN_I2C_STAT0_BTC);
                    _d->adr->CTL0|=LN_I2C_CTL0_STOP;  
                    _txState=I2C_TX_END;
                    lnDisableInterrupt(_d->_irqErr);
                    lnDisableInterrupt(_d->_irqEv);
                    _result=true;
                    _sem.give();
                    return;
                    break;
        case I2C_TX_END:
                    xAssert(0);
                    return;
                    break;
        default:
            xAssert(0);
            break;
    }
}

/**
 * 
 * @param instance
 * @param error
 */
void i2cIrqHandler(int instance, bool error)
{
    lnTwoWire *i=irqHandler[instance];
    xAssert(i);
    i->irq((int)error);
}

// EOF

