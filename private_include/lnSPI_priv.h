/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#pragma once

struct LN_SPI_Registers
{
    uint32_t CTL0; //0
    uint32_t CTL1; //4
    uint32_t STAT; //8
    uint32_t DATA; //c
    uint32_t CRCPOLY; //10
    uint32_t RXCRC;   //14
    uint32_t TXCRC;   //18
    uint32_t I2SCTL;  //1C
    uint32_t I2SPSC;  //20
};
