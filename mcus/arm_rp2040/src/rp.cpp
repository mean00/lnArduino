#include "lnArduino.h"
#include "stdio.h"

FILE *const stdout=NULL;

extern void setup();
extern void loop();

/**
 *
 * @param
 */
void initTask(void *)
{
    LoggerInit();
    setup();
    while (1)
    {
        loop();
    }
}


int main()
{
    LoggerInit();
    setup();
    while (1)
    {
        loop();
    }
}