

#pragma once
/**
 * 
 * @return 
 */
// Careful this is not strictly respected, dont change the  sector size value
#define LN_NVM_SECTOR_SIZE 1024

class lnNvm
{
public:
         lnNvm(int nbSectors);
virtual  ~lnNvm();  
    bool begin();
    bool format();
    bool read(int id, int size, uint8_t *data);
    bool write(int id, int size, uint8_t *data);
    
protected:
    
    int     _nbSectors;
    int     _currentSector;
    xMutex  _mutex;
    
    
    virtual bool eraseSector(int sector)=0;
    virtual bool writeSector(int sector, int offset, int size, uint8_t *data)=0;
    virtual bool readSector(int sector, int offset, int size, uint8_t *data)=0;
    virtual bool verifyErase(int sector)=0;
            bool prepareSector(int sector);
            bool setState(int sector,int offset, uint8_t state);
            bool garbageCollect(int fromSector, int toSector);
            bool parseEntry(int sector,int offset,int &entryID,int &entrySize,int &nextOffset);
            bool findLastEntryOf(int id, int size, int &offset);
            bool findWriteOffset(int &writeOffset) ;
            
};