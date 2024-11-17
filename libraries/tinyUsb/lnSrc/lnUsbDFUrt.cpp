#include "lnUsbDFUrt.h"
#include "device/dcd.h"
#include "lnArduino.h"
#include "lnUsbStack.h"
#include "tusb.h"

static lnDFUCbrt *_dfuCbrt = NULL;
/**
 *
 */
void lnUsbDFURT::addDFURTCb(lnDFUCbrt *cb)
{
    _dfuCbrt = cb;
}
/**
 *
 */
extern "C" void tud_dfu_runtime_reboot_to_dfu_cb()
{

    if (!_dfuCbrt)
    {
        Logger("No DFU RT callback\n");
        return;
    }
    _dfuCbrt();
}
// EOF
