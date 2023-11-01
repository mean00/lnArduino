#include "lnArduino.h"
#include "lnIRQ_arm.h"
#include "lnSerial.h"

#include "hardware/irq.h"
#include "hardware/uart.h"
#include "ln_rp_dma.h"
#include "ln_rp_dma_channel_map.h"
/**
 * @brief     We dont use DMA for Rx 
 * 
 */
struct lnUart_t
{
    const uart_inst_t *hw;
    const uint32_t    dreq_tx;
    const uint32_t    channel_tx;
};

/*
*/
static const lnUart_t uarts[2] = {{((const uart_inst_t *)uart0_hw), lnRpDMA::LN_DMA_DREQ_UART0_TX  , LN_RP_UART0_CHANNEL_TX,                                                                         
                                        }, 
                                        {
                                        ((const uart_inst_t *)uart1_hw),  lnRpDMA::LN_DMA_DREQ_UART1_TX, LN_RP_UART1_CHANNEL_TX,                                        
                                        }
                                };

/**
 * @brief 
 * 
 */
class lnRpSerial : public lnSerialCore
{
  public:   
  // public API
         lnRpSerial(int instance, int rxBufferSize = 128);
    bool init();
    bool setSpeed(int speed);
    bool enableRx(bool enabled);
    bool transmit(int size, const uint8_t *buffer);
    bool dmaTransmit(int size, const uint8_t *buffer);
    void purgeRx();
    int  read(int max, uint8_t *to);
    void disableInterrupt();
    void enableInterrupt(bool txInterruptEnabled);
    // no copy interface
    int getReadPointer(uint8_t **to);
    void consume(int n);
    void rawWrite(const char *str);

};

/**
    \fn
    \brief
*/
lnRpSerial::lnRpSerial(int instance, int rxBufferSize) : lnSerialCore(instance, rxBufferSize)
{
    _instance = instance;
    init();
}
/**
    \fn
    \brief
*/
bool lnRpSerial::init()
{
    uart_inst_t *u = (uart_inst_t *)uarts[_instance].hw;
    uart_init(u, 115200);
    uart_set_hw_flow(u, false, false);
    uart_set_format(u, 8, 1, UART_PARITY_NONE);
    uart_set_fifo_enabled(u,true);
    uart_set_irq_enables(u, false, false); // disable Rx & Tx
    return true;
}
/**
    \fn
    \brief
*/
bool lnRpSerial::setSpeed(int speed)
{
    uart_inst_t *u = (uart_inst_t *)uarts[_instance].hw;
    uart_set_baudrate(u, speed);
    return true;
}
/**
    \fn
    \brief
*/
bool lnRpSerial::enableRx(bool enabled)
{
    return true;
}
/**
    \fn
    \brief
*/
bool lnRpSerial::transmit(int size, const uint8_t *buffer)
{
    _txMutex.lock();
    uart_inst_t *u = (uart_inst_t *)uarts[_instance].hw;
    io_rw_32 *dr= &(uart_get_hw(u)->dr);
    for (int i = 0; i < size; i++)
    {
        while (!(uart_is_writable(u)))
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
bool lnRpSerial::dmaTransmit(int size, const uint8_t *buffer)
{
    return transmit(size, buffer);
}
/**
    \fn
    \brief
*/
void lnRpSerial::disableInterrupt()
{
}
/**
    \fn
    \brief
*/
void lnRpSerial::enableInterrupt(bool txInterruptEnabled)
{
}
/**
    \fn
    \brief
*/
void lnRpSerial::purgeRx()
{
}
/**
 * @brief 
 * 
 * @param to 
 * @return int 
 */
int lnRpSerial::getReadPointer(uint8_t **to)
{
    xAssert(0);
}
/**
 * @brief 
 * 
 * @param n 
 */
void lnRpSerial::consume(int n)
{
    xAssert(0);
}
/**
 * @brief 
 * 
 * @param str 
 */
void lnRpSerial::rawWrite(const char *str)
{
    xAssert(0);
}
int  lnRpSerial::read(int max, uint8_t *to)
{
    xAssert(0);
    return 0;
}
/**
 * @brief Create a Ln Serial object
 * 
 * @param instance 
 * @param rxBufferSize 
 * @return lnSerialCore* 
 */
lnSerialCore *createLnSerial(int instance, int rxBufferSize)
{
    return new lnRpSerial(instance, rxBufferSize);
}
// EOF