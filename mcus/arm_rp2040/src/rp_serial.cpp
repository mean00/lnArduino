/**
 * @file    rp_serial.cpp
 * @author  mean00
 * @brief 
 * @version 0.1
 * @date 2023-11-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "lnArduino.h"
#include "lnIRQ_arm.h"
#include "lnSerial.h"

#include "hardware/irq.h"
#include "hardware/uart.h"
#include "ln_rp_dma.h"
#include "ln_rp_dma_channel_map.h"
#include "hardware/regs/intctrl.h"
#include "hardware/irq.h"

class lnRpSerial;
static void uart0_irq_handler(void);
static void uart1_irq_handler(void);

static lnRpSerial *rpSerialInstances[2]={NULL,NULL};

/**
 * @brief     We dont use DMA for Rx 
 * 
 */
struct lnUart_t
{
    const uart_inst_t *hw;
    const uint32_t    dreq_tx;
    const uint32_t    channel_tx;
    const int         irq;
    irq_handler_t     irq_handler;
};

/*
*/
static const lnUart_t uarts[2] = {{((const uart_inst_t *)uart0_hw), lnRpDMA::LN_DMA_DREQ_UART0_TX  , LN_RP_UART0_CHANNEL_TX, 
                                                UART0_IRQ,uart0_irq_handler
                                        }, 
                                        {
                                        ((const uart_inst_t *)uart1_hw),  lnRpDMA::LN_DMA_DREQ_UART1_TX, LN_RP_UART1_CHANNEL_TX, 
                                                    UART1_IRQ  ,uart1_irq_handler
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
    bool init(lnSerialMode more);
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

    // custom
    void irq_handler();
    bool transmitIrq(int size, const uint8_t *buffer);
    bool transmitPolling(int size, const uint8_t *buffer);

    int  txLimit;
    int  txCurrent;
    const uint8_t *txData;

};
/**
 * @brief 
 * 
 */
void uart0_irq_handler()
{
    xAssert( rpSerialInstances[0] );
    rpSerialInstances[0]->irq_handler();
}
/**
 * @brief 
 * 
 */
void uart1_irq_handler()
{
    xAssert( rpSerialInstances[1] );
    rpSerialInstances[1]->irq_handler();
}


/**
    \fn
    \brief
*/
lnRpSerial::lnRpSerial(int instance, int rxBufferSize) : lnSerialCore(instance, rxBufferSize)
{
    xAssert(instance < 2);
    xAssert(!rpSerialInstances[instance])
    _instance = instance;        
    rpSerialInstances[instance]=this;
}
/**
    \fn
    \brief
*/
bool lnRpSerial::init(lnSerialMode mode)
{
    uart_inst_t *u = (uart_inst_t *)uarts[_instance].hw;

    _mode = mode;
    uart_init(u, 115200);
    uart_set_hw_flow(u, false, false);
    uart_set_format(u, 8, 1, UART_PARITY_NONE);
    uart_set_fifo_enabled(u,false);
    irq_set_exclusive_handler(uarts[_instance].irq, uarts[_instance].irq_handler);
    uart_set_irq_enables(u, false, false); // disable Rx & Tx
    irq_set_enabled(uarts[_instance].irq, true);
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
    //return transmitPolling(size,buffer);
    return transmitIrq(size,buffer);   
}
/**
 * @brief 
 * 
 * @param size 
 * @param buffer 
 * @return true 
 * @return false 
 */
bool lnRpSerial::transmitIrq(int size, const uint8_t *buffer)
{
    _txMutex.lock();
    // Pre-fill the uart
    txLimit=size;
    txCurrent=0;
    txData=buffer;
    uart_inst_t *u = (uart_inst_t *)uarts[_instance].hw;
    io_rw_32 *dr= &(uart_get_hw(u)->dr);
    while ((uart_is_writable(u)) && txCurrent<txLimit)
            *dr = txData[txCurrent++];
    // if more to do, let the irq handle it
    if(txCurrent!=txLimit)
    {
         uart_set_irq_enables(u, false, true); // enable Tx
        _txDone.take();
    }
    _txMutex.unlock();
    return true;
}
/**
 * @brief 
 * 
 * @param size 
 * @param buffer 
 * @return true 
 * @return false 
 */
bool lnRpSerial::transmitPolling(int size, const uint8_t *buffer)
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
 * @brief 
 * 
 */
void lnRpSerial::irq_handler()
{
    uart_inst_t *u = (uart_inst_t *)uarts[_instance].hw;
    io_rw_32 *dr= &(uart_get_hw(u)->dr);
    while ((uart_is_writable(u)) && txCurrent<txLimit)
            *dr = txData[txCurrent++];
    if(txCurrent==txLimit)
    {
         uart_set_irq_enables(u, false, false); // disable Tx
        _txDone.give();
    }
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
    uart_inst_t *u = (uart_inst_t *)uarts[_instance].hw;
    io_rw_32 *dr= &(uart_get_hw(u)->dr);
    int size=strlen(str);
    for (int i = 0; i < size; i++)
    {
        while (!(uart_is_writable(u)))
        {
            __asm__("nop");
        }
        *dr = *str++;
    }
}
/**
 * @brief 
 * 
 * @param max 
 * @param to 
 * @return int 
 */
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