 # lnArduino : Arduino style API for GD32 chips


The aim of this project is to offer an Arduino style framework for the Longan Nano board (RISCV) or GD32F1/F3 (ARM) boards.
What are the differences ?  
* Cmake based build system, use whatever IDE you want.
* Support ARM and RISCV GD32 MCUs, same API
* FreeRTOS out of the box, using the RISCV port made by QQxiaoming [1] / vanilla ARM CM3/CM4
* Better peripherals support (DMA, tasks..)
* Only loosely compatible with Arduino API but easy to port

A note of warning : This is my take on what i would like to have/use for such a board
It might not fit your needs.

_What works for the moment :_
 * FreeRTOS
 * Spi transmit (DMA)
 * Serial transmit (DMA)
 * I2C Transmit (DMA) / Receive (Interrupt)
 * GPIO + interrupt
 * PWM support
 * Onboard LCD support (Longan Nano)
 * DAC (DMA) support
 * very basic ADC support


_How to build the examples :_
* Symlink/copy the Arduino_gd32_freeRTOS folder under the demo folder
Fill-in platformconfig.cmake to point to your toolchain (adjust flags if needed)
mkdir build && cd build && cmake .. && make
* define LN_ARCH ("ARM" or "RISCV") and LN_MCU ("M3" or "M4" or "VF103")

_Bonus:_

There is also some python helper scripts to have basic FreeRTOS support with gdb


Very little is left from the original arduino/sdk


The demoProject folder contains sample code to show the API.

[1] https://github.com/QQxiaoming/gd32vf103_freertos.git

