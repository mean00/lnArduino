/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#pragma once
#include "lnArduino.h"
#include "lnDma.h"
struct LN_I2C_DESCRIPTOR;

/**
 */
class lnI2CSession
{
  public:
    lnI2CSession(int t, uint32_t nb, const uint32_t *sz, const uint8_t **dt)
    {
        target = t;
        nbTransaction = nb;
        transactionSize = sz;
        transactionData = dt;
        curTransaction = 0;
        curIndex = 0;
        total_receive = 0;
        current_receive = 0;
        // sanity check
        for (int i = 0; i < nb; i++)
        {
            xAssert(sz[i] < 65535);
        }
    }
    int target;
    int nbTransaction;
    const uint32_t *transactionSize;
    const uint8_t **transactionData;
    int curTransaction;
    int curIndex;
    int total_receive;
    int current_receive;
};

/**
 *
 * @param instance
 * @param speed
 */
class lnTwoWire
{

  public:
    lnTwoWire(int instance, int speed = 0);
    ~lnTwoWire();
    void setSpeed(int speed);
    void setAddress(int address)
    {
        _targetAddress = address;
    }
    bool write(int n, const uint8_t *data)
    {
        return write(_targetAddress, n, data);
    }
    bool read(int n, uint8_t *data)
    {
        return read(_targetAddress, n, data);
    }
    bool write(int target, uint32_t n, const uint8_t *data);
    bool multiWrite(int target, uint32_t nbSeqn, const uint32_t *seqLength, const uint8_t **data);
    bool read(int target, uint32_t n, uint8_t *data);
    bool multiRead(int target, uint32_t nbSeqn, const uint32_t *seqLength, uint8_t **seqData);
    bool begin(int target = 0);

  protected:
    void stopIrq();
    void startIrq();
    void startRxIrq();
    bool sendNext();
    bool receiveNext();
    bool initiateTx();
    void dmaTxDone();
    void dmaRxDone();
    void setInterruptMode(bool eventEnabled, bool dmaEnabled, bool txEmptyEnabled);

  protected:
    enum _I2C_TX
    {
        I2C_IDLE = 0xff,
        I2C_TX_START = 0,
        I2C_TX_ADDR_SENT = 1,
        I2C_TX_DATA = 2,
        I2C_TX_STOP = 3,
        I2C_TX_END = 4,
        I2C_TX_DATA_DMA = 5,

        I2C_RX_START = 8 + I2C_TX_START,
        I2C_RX_ADDR_SENT = 8 + I2C_TX_ADDR_SENT,
        I2C_RX_DATA = 8 + I2C_TX_DATA,
        I2C_RX_STOP = I2C_TX_STOP + 8,
        I2C_RX_END = I2C_TX_END + 8,
        I2C_RX_DATA_DMA = 8 + I2C_TX_DATA_DMA,
    };

    int _instance;
    int _targetAddress;
    int _speed;
    const LN_I2C_DESCRIPTOR *_d;
    xBinarySemaphore _sem;
    bool _result;
    _I2C_TX _txState;
    lnI2CSession *_session;
    lnDMA _dmaTx;
    lnDMA _dmaRx;

  public:
    void irq(int evt);
    static void dmaTxDone_(void *c, lnDMA::DmaInterruptType typ);
    static void dmaRxDone_(void *c, lnDMA::DmaInterruptType typ);
};
