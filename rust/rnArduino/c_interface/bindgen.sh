#!/bin/bash
#
gen_cpp()
{
echo "__________________cpp:${1}_________________________"
bash ../../../cmake/rustgen.sh  ${1}   ${2} $PWD
}

gen_c()
{
echo "__________________c:${1}_________________________"
bash ../../../cmake/rustgen_c.sh  ${1}   ${2} $PWD
}


#gen_cpp ../../../include/lnSPI.h ../src/rn_spi_l.rs
gen_cpp lnTimer_c.h ../src/rn_timer_c.rs
gen_cpp lnExti_c.h ../src/rn_exti_c.rs
gen_c lnI2C_c.h ../src/rn_i2c_c.rs
gen_cpp lnGPIO_c.h ../src/rn_gpio_c.rs
gen_cpp lnSPI_c.h ../src/rn_spi_c.rs
gen_c lnCDC_c.h ../src/rn_cdc_c.rs
gen_c lnUSB_c.h ../src/rn_usb_c.rs
gen_c lnFast_EventGroup_c.h ../src/rn_fast_event_c.rs
gen_c lnFreeRTOS_c.h  ../src/rn_freertos_c.rs

