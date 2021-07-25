 # lnArduino : Arduino style API for GD32 chips


The aim of this project is to offer an Arduino style framework for the Longan Nano board (RISCV) or GD32F1/F3 (ARM) boards.

The API is 99% the same between the Riscv & Arm versions. Mostly tiny pinout/clock value changes.

_What are the differences compared to vanilla Arduino ?_
* Cmake based build system, use whatever IDE you want.
* Support ARM and RISCV GD32 MCUs, same API (tested with GD32VF103, GD32F103 and GD32F303).
* FreeRTOS out of the box, using the RISCV port made by QQxiaoming [1] / or vanilla ARM M3/M4
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
 * Basic Flash erase/write

_Demos_
The demoProject contains simple API demonstration programs.

_How to build the demos :_
* Symlink/copy the Arduino_gd32_freeRTOS folder under the demo folder
Fill-in platformconfig.cmake to point to your toolchain (adjust flags if needed)
mkdir build && cd build && cmake .. && make
* edit mcuSelect.Cmake to define which MCU you are using

_Bonus:_

There is also some python helper scripts to have basic FreeRTOS support with gdb


_Heritage:_
* FreeRTOS from FreeRTOS
* Embedded_printf from [2]
* The riscv freeRTOS port is from QQxiaming (see [1])
* A couple of header file from arduino to offer a basic compatibility API
* A couple of header from nucleisys to access n200 extra functions
* Everything else has been written from scratch


The demoProject folder contains sample code to show the API.

[1] https://github.com/QQxiaoming/gd32vf103_freertos.git
[2] https://github.com/mpaland/printf


