#include "lnWS2812_rp_single.h"
#include "../private_include/ln_rp_spi_priv.h"
#include "lnArduino.h"
#include "lnGPIO.h"
#include "lnSerial.h"

#define P1 0xCU
#define P0 0x8U

#define XXX(a, b, c, d) (a + (b << 4) + (c << 8) + (d << 12))

// expand 4 bits to 16 (*4)
const uint16_t lookupTable[16] = {
    XXX(P0, P0, P0, P0), // 0
    XXX(P0, P0, P0, P1), XXX(P0, P0, P1, P0), XXX(P0, P0, P1, P1),

    XXX(P0, P1, P0, P0), // 4
    XXX(P0, P1, P0, P1), XXX(P0, P1, P1, P0), XXX(P0, P1, P1, P1),

    XXX(P1, P0, P0, P0), // 0
    XXX(P1, P0, P0, P1), XXX(P1, P0, P1, P0), XXX(P1, P0, P1, P1),

    XXX(P1, P1, P0, P0), // 0
    XXX(P1, P1, P0, P1), XXX(P1, P1, P1, P0), XXX(P1, P1, P1, P1),
};

/**
 * @brief
 *
 * @param r
 * @param g
 * @param b
 */
void WS2812_rp2040_single::setColor(uint8_t r, uint8_t g, uint8_t b)
{
    _buffer[0] = lookupTable[g >> 4];
    _buffer[1] = lookupTable[g & 0xf];
    _buffer[2] = lookupTable[r >> 4];
    _buffer[3] = lookupTable[r & 0xf];
    _buffer[4] = lookupTable[b >> 4];
    _buffer[5] = lookupTable[b & 0xf];
    _spi->write8(12, (uint8_t *)_buffer);
}
// EOF
