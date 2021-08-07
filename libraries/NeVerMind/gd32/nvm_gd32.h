/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */


#pragma once
#include "nvmCore.h"


/**
 * 
 * @param nbSectors
 */
class lnNvmGd32 : public lnNvm
{
public:
         lnNvmGd32();
virtual  ~lnNvmGd32();         
protected:
    
    virtual bool eraseSector(int sector);
    virtual bool writeSector(int sector, int offset, int size, uint8_t *data);
    virtual bool readSector(int sector, int offset, int size, uint8_t *data);
    virtual bool verifyErase(int sector);
protected:
    uint32_t    _baseAddress;
};

// EOF