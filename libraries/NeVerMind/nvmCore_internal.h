/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#pragma once
#define NVM_INVALID_SECTOR 100
#define NVM_INVALID_ENTRY  0x10000

#define NVM_8BIT(x) ((0xff << x)& 0xff)


#define NVM_SECTOR_ERASING   NVM_8BIT(0)
#define NVM_SECTOR_ERASED    NVM_8BIT(1)
#define NVM_SECTOR_PREPARING NVM_8BIT(2)
#define NVM_SECTOR_ACTIVE    NVM_8BIT(3)
#define NVM_SECTOR_INACTIVE  NVM_8BIT(4)

#define NVM_ENTRY_UNUSED     NVM_8BIT(0)
#define NVM_ENTRY_BEGIN      NVM_8BIT(1)
#define NVM_ENTRY_LEN_VALID  NVM_8BIT(2)
#define NVM_ENTRY_VALID      NVM_8BIT(3)
#define NVM_ENTRY_INVALID    NVM_8BIT(4)
#define NVM_ENTRY_SKIPPED    0