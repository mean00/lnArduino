/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#include "lnSPI.h"
#include "lnArduino.h"
#include "lnGPIO.h"
#include "lnPeripheral_priv.h"
#include "lnSPI_priv.h"
struct SpiDescriptor
{
    uint32_t spiAddress;
    LnIRQ spiIrq;
    int dmaEngine;
    int dmaTxChannel;
    Peripherals rcu;
    lnPin mosi, miso, clk;
};

// We assume all the pins of a given SPI are on the same gpio bank
static const SpiDescriptor spiDescriptor[3] = {
    //                       DMA DMAT periph MOSI MISO CLK
    {LN_SPI0_ADR, LN_IRQ_SPI0, 0, 2, pSPI0, PA7, PA6, PA5},
    {LN_SPI1_ADR, LN_IRQ_SPI1, 0, 4, pSPI1, PB15, PB14, PB13},
    {LN_SPI2_ADR, LN_IRQ_SPI2, 1, 1, pSPI2, PB5, PB4, PB3}};
static LN_SPI_Registers *aspi0 = (LN_SPI_Registers *)LN_SPI0_ADR;
static LN_SPI_Registers *aspi1 = (LN_SPI_Registers *)LN_SPI1_ADR;
static LN_SPI_Registers *aspi2 = (LN_SPI_Registers *)LN_SPI2_ADR;
/**
 * switch between RX/TX and TX only : false is txRx,
 * @param adr
 * @param rxTx
 */

void updateMode(LN_SPI_Registers *d, lnSpiDirection dir)
{

    uint32_t reg = d->CTL0;

    reg &= ~(LN_SPI_CTL0_BDOEN | LN_SPI_CTL0_BDEN | LN_SPI_CTL0_RO);
    switch (dir)
    {
    case lnDuplex:
        reg |= 0;
        break; // 0 all bits = full duplex
    case lnTxOnly:
        reg |= LN_SPI_CTL0_BDOEN | LN_SPI_CTL0_BDEN;
        break;
    case lnRxOnly:
        reg |= LN_SPI_CTL0_BDEN;
        break;
    default:
        xAssert(0);
        break;
    }
    d->CTL0 = reg;
}
/**
 *
 * @param adr
 * @param bits
 */
void updateDataSize(LN_SPI_Registers *d, int bits)
{

    uint32_t reg = d->CTL0;
    switch (bits)
    {
    case 8:
        reg &= ~LN_SPI_CTL0_FF16;
        break;
    case 16:
        reg |= LN_SPI_CTL0_FF16;
        break;
    default:
        xAssert(0);
    }
    d->CTL0 = reg;
}
void updateDmaTX(LN_SPI_Registers *d, bool onoff)
{
    if (onoff)
        d->CTL1 |= (uint32_t)(LN_SPI_CTL1_DMATEN);
    else
        d->CTL1 &= ~((uint32_t)(LN_SPI_CTL1_DMATEN));
}

/**
 *
 * @param instance
 * @param pinCs
 */
#define M(x) spiDescriptor[instance].x

hwlnSPIClass::hwlnSPIClass(int instance, int pinCs)
    : _internalSettings(1000000, SPI_MSBFIRST, SPI_MODE0, -1), _currentSetting(1000000, SPI_MSBFIRST, SPI_MODE0, -1),
      txDma(lnDMA::DMA_MEMORY_TO_PERIPH, M(dmaEngine), M(dmaTxChannel), 16, 16)
{
    _useDMA = false;
    _cookie = NULL;
    _callback = NULL;
    _instance = instance;
    _settings = NULL;
    lnPeripherals::enable((Peripherals)(pSPI0 + instance));
    xAssert(instance < 3);
    const SpiDescriptor *s = spiDescriptor + instance;

    _irq = s->spiIrq;
    lnEnableInterrupt(_irq);
    _adr = s->spiAddress;
    lnPeripherals::enable(s->rcu);
    // setup miso/mosi & clk
    lnPinMode(s->mosi, lnALTERNATE_PP);
    lnPinMode(s->miso, lnFLOATING);
    lnPinMode(s->clk, lnALTERNATE_PP);
    _inSession = false;

    if (pinCs != -1)
    {
        lnPinMode((lnPin)pinCs, lnOUTPUT);
        lnDigitalWrite((lnPin)pinCs, true);
    }
    LN_SPI_Registers *d = (LN_SPI_Registers *)_adr;
    _settings = &_internalSettings;
    sdisable();
}
/**
 *
 * @return
 */
lnPin hwlnSPIClass::misoPin() const
{
    const SpiDescriptor *s = spiDescriptor + _instance;
    return s->miso;
}

/**
 *
 * @return
 */
lnPin hwlnSPIClass::mosiPin() const
{
    const SpiDescriptor *s = spiDescriptor + _instance;
    return s->mosi;
}

/**
 *
 * @return
 */
lnPin hwlnSPIClass::clkPin() const
{
    const SpiDescriptor *s = spiDescriptor + _instance;
    return s->clk;
}
/**
 *
 */
hwlnSPIClass::~hwlnSPIClass()
{
    end();
}
/**
 *
 */
void hwlnSPIClass::begin()
{
    _settings = &_internalSettings;
    setup();
}
/**
 *
 */
void hwlnSPIClass::end(void)
{
    LN_SPI_Registers *d = (LN_SPI_Registers *)_adr;
    sdisable();
}

void hwlnSPIClass::beginSession(int bitSize)
{
    _inSession = true;
    setup();
    LN_SPI_Registers *d = (LN_SPI_Registers *)_adr;
    updateMode(d, lnTxOnly);
    updateDataSize(d, bitSize);
    senable();
    csOn();
}
/**
 *
 */
void hwlnSPIClass::beginTransaction(lnSPISettings &settings)
{
    _mutex.lock();
    _currentSetting = settings;
    _settings = &_currentSetting;
    setup();
}
void hwlnSPIClass::endTransaction()
{
    _settings = &_internalSettings;
    setup(); // restore settings
    _mutex.unlock();
}

/**
 */
void hwlnSPIClass::endSession()
{
    LN_SPI_Registers *d = (LN_SPI_Registers *)_adr;
    _inSession = false;
    csOff();
    sdisable();
}
/**
 *
 * @param sz
 * @param data
 * @return
 */
bool hwlnSPIClass::writeInternal(int sz, int data)
{
    LN_SPI_Registers *d = (LN_SPI_Registers *)_adr;
    xAssert(_inSession);
    if (d->STAT & LN_SPI_STAT_CONFERR)
    {
        Logger("Conf Error\n");
        return false;
    }

    while (sbusy())
    {
    }
    d->DATA = data;

    waitForCompletion();
    return true;
}

/**
 */
bool hwlnSPIClass::writesInternal(int sz, int nb, const uint8_t *data, bool repeat)
{
    LN_SPI_Registers *d = (LN_SPI_Registers *)_adr;
    switch (sz)
    {
    default:
        xAssert(0);
        break;
    case 8: {
        const uint8_t *p = data;
        int inc = !repeat;
        for (size_t i = 0; i < nb; i++)
        {
            while (sbusy())
            {
            }
            d->DATA = *p;
            p += inc;
        }
        break;
    }
    case 16: {
        const uint16_t *p = (const uint16_t *)data;
        int inc = !repeat;
        for (size_t i = 0; i < nb; i++)
        {
            while (sbusy())
            {
            }
            d->DATA = *p;
            p += inc;
        }
        break;
    }
    }

    waitForCompletion();
    return true;
}
/**
 *
 * @param z
 * @return
 */
bool hwlnSPIClass::write(int z)
{
    return writeInternal(8, z);
}
/**
 *
 * @param z
 * @return
 */
bool hwlnSPIClass::write16(int z)
{
    return writeInternal(16, z);
}

/**
 *
 */
bool hwlnSPIClass::write16Repeat(int nb, const uint16_t pattern)
{
    return writesInternal(16, nb, (const uint8_t *)&pattern, true);
}
/**
 */
bool hwlnSPIClass::write(int nbBytes, const uint8_t *data, bool repeat)
{
    return writesInternal(8, nbBytes, data, repeat);
}
/**
 */
bool hwlnSPIClass::write16(int nbWord, const uint16_t *data, bool repeat)
{
    return writesInternal(16, nbWord, (const uint8_t *)data, repeat);
}

/**
 *
 */
void hwlnSPIClass::csOn()
{
    if (_settings->pinCS != -1)
    {
        lnDigitalWrite((lnPin)_settings->pinCS, false);
    }
}
/**
 *
 */
void hwlnSPIClass::csOff()
{
    if (_settings->pinCS != -1)
    {
        lnDigitalWrite((lnPin)_settings->pinCS, true);
    }
}
/**
 *  This is needed to be able to toggle the CS when all is done
 */
void hwlnSPIClass::waitForCompletion()
{
    LN_SPI_Registers *d = (LN_SPI_Registers *)_adr;
    int dir = d->CTL0 >> 14;
    switch (dir)
    {
    case 0:
    case 1: // bidir mode
        while (sbusy())
        {
        }
        break;
    case 2: // receive only
        break;
    case 3: //  tx only

        while (txbusy())
        {
            __asm__("nop");
        }

        while (sbusy())
        {
            __asm__("nop");
        }
        break;
    }
}
/**
 *
 */
bool hwlnSPIClass::read1wire(int nbRead, uint8_t *rd)
{
    LN_SPI_Registers *d = (LN_SPI_Registers *)_adr;
    updateMode(d, lnRxOnly); // 1 Wire RX

    // clear stuff (not sure it is useful)
    for (int i = 0; i < nbRead; i++)
    {
        while (!(d->STAT & LN_SPI_STAT_RBNE))
        {
            __asm__("nop");
        }
        rd[i] = d->DATA;
    }
    waitForCompletion();
    return true;
}

/**
 */
bool hwlnSPIClass::transfer(int nbBytes, uint8_t *dataOut, uint8_t *dataIn)
{
    LN_SPI_Registers *d = (LN_SPI_Registers *)_adr;
    xAssert(_inSession);
    for (size_t i = 0; i < nbBytes; i++)
    {
        while (!(d->STAT & LN_SPI_STAT_TBE))
        {
        }
        d->DATA = dataOut[i];

        while (!(d->STAT & LN_SPI_STAT_RBNE))
        {
        }
        dataIn[i] = d->DATA;
    }
    waitForCompletion();
    return true;
}
/**
 *
 * @return
 */
void hwlnSPIClass::txDone()
{
    _done.give();
}
/**
 * \brief program the peripheral with the current settings
 */
void hwlnSPIClass::setup()
{
    LN_SPI_Registers *d = (LN_SPI_Registers *)_adr;
    xAssert(_settings);
    sdisable();
    d->STAT &= LN_SPI_STAT_MASK;
    d->CTL0 &= LN_SPI_CTL0_MASK;
    d->CTL1 &= LN_SPI_CTL1_MASK;

    uint32_t ctl0 = d->CTL0;
    ctl0 |= LN_SPI_CTL0_MSTMODE;
    // Drive the NSS by sw, pull it up
    // there does not seem to be a way to completely disconnect NSS management
    ctl0 |= LN_SPI_CTL0_SWNSS;
    ctl0 |= LN_SPI_CTL0_SWNSSEN;

    switch (_settings->bOrder)
    {
    case SPI_LSBFIRST:
        ctl0 |= LN_SPI_CTL0_LSB;
        break;
    case SPI_MSBFIRST:
        ctl0 &= ~LN_SPI_CTL0_LSB;
        break;
    default:
        xAssert(0);
        break;
    }
    uint32_t s = 0;
    switch (_settings->dMode)
    {
    // https://en.wikipedia.org/wiki/Serial_Peripheral_Interface
    case SPI_MODE0: // Pol0, Phase 0/Edge 1
        s = 0;      // Low 1 edge
        break;
    case SPI_MODE1:           // Pol 0 Phase 1 Edge 0
        s = LN_SPI_CTL0_CKPH; // Low 2 edge
        break;
    case SPI_MODE2:           // POL 1 PHA 0 Edge 1
        s = LN_SPI_CTL0_CKPL; // high , 1 edge
        break;
    case SPI_MODE3:                              // POL 1 PHA 1 Edge 0
        s = LN_SPI_CTL0_CKPL | LN_SPI_CTL0_CKPH; // high , 2 edge
        break;
    default:
        xAssert(0);
        break;
    }
    ctl0 |= s;
    d->CTL0 = ctl0;
    uint32_t prescale = 0, speed = _settings->speed, apb;
    xAssert(speed);

    Peripherals periph = (Peripherals)((int)pSPI0 + _instance);
    apb = lnPeripherals::getClock(periph);
    prescale = (apb + speed / 2) / speed;
    // prescale can only go from 2 to 256, and prescale=2^(psc+1) actually

    uint32_t psc = 0;
    if (prescale <= 2)
        psc = 0;
    else if (prescale >= 256)
        psc = 7;
    else
    {
        psc = 0;
        uint32_t toggle = 2;
        while (prescale > toggle)
        {
            toggle *= 2;
            psc++;
        }
    }
    uint32_t sp = d->CTL0;
    sp &= ~(7 << 3);
    sp |= psc << 3;
    d->CTL0 = sp;
    updateMode(d, lnTxOnly); // Tx only by default
}
/**
 *
 * @param dataSize
 */
void hwlnSPIClass::setDataSize(int dataSize)
{
    LN_SPI_Registers *d = (LN_SPI_Registers *)_adr;
    updateDataSize(d, dataSize);
}

void hwlnSPIClass::setBitOrder(spiBitOrder order)
{
    _internalSettings.bOrder = order;
    setup();
}
void hwlnSPIClass::setDataMode(spiDataMode mode)
{
    _internalSettings.dMode = mode;
    setup();
}
void hwlnSPIClass::setSpeed(int speed)
{
    _internalSettings.speed = speed;
    setup();
}

int hwlnSPIClass::getPeripheralClock()
{
    Peripherals p = (Peripherals)(pSPI0 + _instance);
    return lnPeripherals::getClock(p);
}

// EOF
