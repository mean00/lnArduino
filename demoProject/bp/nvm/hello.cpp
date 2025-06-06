#include "esprit.h"
#include "lnDAC.h"
#include "lnTimer.h"
#include "math.h"

#include "nvm_gd32.h"

#define PWM PB9
#define TIMER 3
#define CHANNEL 3

void setup()
{
    lnPinMode(LN_SYSTEM_LED, lnOUTPUT);
}
/**
 *
 */

void stop()
{
    Logger("** FAILURE ** \n");
    while (1)
    {
        lnDelayMs(40);
    }
}

#define TAIST(x)                                                                                                       \
    {                                                                                                                  \
        bool result = x;                                                                                               \
        if (!result)                                                                                                   \
        {                                                                                                              \
            Logger("FAILURE of %s at line %d \n", #x, __LINE__);                                                       \
            stop();                                                                                                    \
            return;                                                                                                    \
        }                                                                                                              \
    }

int val;

void InitNvm(lnNvmGd32 &nvm)
{

    uint8_t v = 1;
    TAIST(nvm.write(1, 1, &v));
    v = 0;

    nvm.dumpEntries(-1);

    TAIST(nvm.read(1, 1, &v));
    TAIST(v == 1);

    v = 2;
    TAIST(nvm.write(1, 1, &v));
    nvm.dumpEntries(-1);
    v = 0;
    TAIST(nvm.read(1, 1, &v));
    TAIST(v == 2);

    v = 3;
    TAIST(nvm.write(1, 1, &v));
    nvm.dumpEntries(-1);
    v = 0;
    TAIST(nvm.read(1, 1, &v));
    TAIST(v == 3);
}

void TestNvm()
{
    uint8_t v;
    lnNvmGd32 nvm;
    TAIST(nvm.begin());
    TAIST(nvm.read(1, 1, &v));
    TAIST(v == 3);
}
int roundx;
void loop()
{
    lnNvmGd32 nvm;
    // nvm.format();
    // lnDelayMs(5000);
    Logger("Starting...\n");
    if (!nvm.begin())
    {
        Logger("Formating\n");
        nvm.format();
    }

    for (int roundx = 0; roundx < 10; roundx++)
    {
        Logger("Round %d  \n", roundx);
        InitNvm(nvm);
    }

    Logger("End => ALL OK !\n");
    while (1)
    {
        lnDelayMs(10);
    }
}
