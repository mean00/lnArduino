

#pragma once
/**
 * 
 * @return 
 */
// Careful this is not strictly respected, dont change the  sector size value
#define LN_NVM_SECTOR_SIZE 1024
#define NVM_INVALID_SECTOR 100
#define LN_NVM_MAX_PAYLOAD 64
enum LN_NVM_SECTOR_STATE
{
    Nvm_Erased=0, // ff ff
    Nvm_Prep=1,   // 00 ff
    Nvm_Active=2, // 00 00
    Nvm_Error=3
};


struct LN_NVM_ENTRY
{
    uint16_t id;
    uint16_t size;
    uint16_t state; // ff active 0  inactive
    uint16_t tag;
};


/**
 * 
 * @param nbSectors
 */
class lnNvm
{
public:
         lnNvm(int nbSectors);
virtual  ~lnNvm();  
    bool begin();
    bool format();
    bool read(int id, int size, uint8_t *data);
    bool write(int id, int size, uint8_t *data);
    
    bool dumpEntries(int sector);
    
protected:
    
    int     _nbSectors;
    int     _currentSector;
    xMutex  _mutex;
    int     _writeIndex;
    
    
    virtual bool eraseSector(int sector)=0;
    virtual bool writeSector(int sector, int offset, int size, uint8_t *data)=0;
    virtual bool readSector(int sector, int offset, int size, uint8_t *data)=0;
    virtual bool verifyErase(int sector)=0;
    
    
            bool prepareSector(int sector);
            bool garbageCollection();
    
    LN_NVM_SECTOR_STATE     getSectorState(int sector);
    bool                    setSectorState(int sector,LN_NVM_SECTOR_STATE state)  ;
    
    
    bool        findEntry(int id, uint32_t &offset, LN_NVM_ENTRY &entry);
    bool        getWriteAddress(uint32_t &address);
    
};
// EOF