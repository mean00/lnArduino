#include "lnArduino.h"
#include "lnIRQ_arm.h"
#include "lnSerial.h"

#include "hardware/irq.h"
#include "hardware/uart.h"

struct lnUart_t
{
    uart_inst_t *hw;
};

static const lnUart_t uarts[2] = {{((uart_inst_t *)uart0_hw)}, {((uart_inst_t *)uart1_hw)}};

lnDMA dummyDma(lnDMA::DMA_MEMORY_TO_PERIPH, 0, 0, 0, 0);


/**
    \fn
    \brief
*/
lnSerial::lnSerial(int instance, int rxBufferSize) : _txDma(dummyDma)
{
    _instance = instance;
    init();
}
/**
    \fn
    \brief
*/
bool lnSerial::init()
{
    uart_inst_t *u = uarts[_instance].hw;
    uart_init(u, 115200);
    uart_set_hw_flow(u, false, false);
    uart_set_format(u, 8, 1, UART_PARITY_NONE);
    uart_set_fifo_enabled(u,true);
    return true;
}
/**
    \fn
    \brief
*/
bool lnSerial::setSpeed(int speed)
{
    uart_inst_t *u = uarts[_instance].hw;
    uart_set_baudrate(u, speed);
    return true;
}
/**
    \fn
    \brief
*/
bool lnSerial::enableRx(bool enabled)
{
    return true;
}
/**
    \fn
    \brief
*/
bool lnSerial::transmit(int size, const uint8_t *buffer)
{
    _txMutex.lock();
    uart_inst_t *u = uarts[_instance].hw;
    io_rw_32 *dr= &(uart_get_hw(u)->dr);
    for (int i = 0; i < size; i++)
    {
        if (!(uart_is_writable(u)))
        {
            __asm__("nop");
        }
        *dr = *buffer++;
    }
    _txMutex.unlock();
    return true;
}
/**
    \fn
    \brief
*/
bool lnSerial::dmaTransmit(int size, const uint8_t *buffer)
{
    return transmit(size, buffer);
}
/**
    \fn
    \brief
*/
void lnSerial::disableInterrupt()
{
}
/**
    \fn
    \brief
*/
void lnSerial::enableInterrupt(bool txInterruptEnabled)
{
}
/**
    \fn
    \brief
*/
void lnSerial::purgeRx()
{
}
/**
    \fn
    \brief
*/
void lnSerial::_interrupt(void)
{
}
/**
    \fn
    \brief
*/
int lnSerial::read(int max, uint8_t *to)
{
    return 0;
}
/**
    \fn
    \brief
*/

int lnSerial::getReadPointer(uint8_t **to)
{
    return 0;
}
/**
    \fn
    \brief
*/
void lnSerial::consume(int n)
{
}
/**
    \fn
    \brief
*/
void lnSerial::rawWrite(const char *c)
{
}
/**
    \fn
    \brief
*/
void lnSerial::txInterruptHandler(void)
{
}
/**
    \fn
    \brief
*/
void lnSerial::rxInterruptHandler(void)
{
}
/**
    \fn
    \brief
*/
bool lnSerial::_programTx(void)
{
    return true;
}
/**
    \fn
    \brief
*/
bool lnSerial::_enableTx(txState mode)
{
    return true;
}
/**
    \fn
    \brief
*/

void lnSerial::txDmaCb()
{
}
/**
    \fn
    \brief
*/
void lnSerial::_dmaCallback(void *c, lnDMA::DmaInterruptType it)
{
}
/**
    \fn
    \brief
*/
void lnSerial::interrupts(int instance)
{
}
// EOF