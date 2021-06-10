 # Arduino_gd32_freeRTOS

**VERY MUCH WORK IN PROGRESS**

The aim of this project is to offer an Arduino style framework for the Longan Nano board.
What are the differences ?  
* Cmake based build system, use whatever IDE you want.
* FreeRTOS out of the box, using the port made by QQxiaoming [1]
* Better peripherals support (DMA, tasks..)
* Only loosely compatible with Arduino API but easy to port

A note of warning : This is my take on what i would like to have/use for such a board
It might not fit your needs.

_What works for the moment :_
 * FreeRTOS
 * Spi transmit (DMA)
 * Serial transmit (DMA)
 * I2C Transmit (DMA)
 * GPIO + interrupt
 * Onboard LCD support


_How to build the examples :_
Symlink/copy the Arduino_gd32_freeRTOS folder under the demo folder
Fill-in platformconfig.cmake to point to your toolchain (adjust flags if needed)
mkdir build && cd build && cmake .. && make



Code from the longduino / gd32 libs are used internally for now, but they'll be removed as progress is done.

The demoProject folder contains sample code to show the API.

[1] https://github.com/QQxiaoming/gd32vf103_freertos.git

