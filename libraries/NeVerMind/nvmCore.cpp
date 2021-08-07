/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#include "lnArduino.h"
#include "nvmCore.h"
#include "nvmCore_internal.h"


/**
 * 
 * @param nbSectors
 */
lnNvm::lnNvm(int nbSectors)
{
    _nbSectors=0;
    _currentSector=NVM_INVALID_SECTOR;
}
/**
 * 
 */
lnNvm::~lnNvm()
{
    
}

/**
 * should only be X bits set to zero on the right and all 1s on the left
 * @param s
 * @return 
 */
static bool valid4(uint8_t s)
{
    switch(s)
    {
        case 8:
        case 12:
        case 0xe: 
        case 0xf: return true;break;
        default:
            return false;
            break;
        
    }
}
static bool valid(uint8_t s)
{
    if((s&0xf0)==0xf0)
    {
        return valid4(s &0xf);
    }
    if(s&0x0f) return false;
    return valid4(s >>4);
}

/**
 * 
 * @return 
 */
bool lnNvm::begin()
{
  // find first active sector
    uint8_t s;
    for(int i=0;i<_nbSectors;i++)
    {
        if(!readSector(i,0,1,&s)) continue;
        if(s==NVM_SECTOR_ACTIVE)
        {
             _currentSector=i;
             return true;
        }
    }
    Logger("No valid sector found, reformating\n");
    return format();    
}


/**
 * Prepare sector 0
 * @return 
 */
bool lnNvm::format()
{
    xAutoMutex aut(&_mutex);
    bool r=true;
    // Prepare sector 0
    if(!prepareSector(0))
    {
        r= false;
    }
    if(r)
        if(!setState(0,0,NVM_SECTOR_ACTIVE))
        {
            r=false;
        }
    if(r)
        _currentSector=0;
    else
        Logger("Cannot format\n");    
    return true;
}
/**
 * 
 * @param offset
 * @param entryID
 * @param entrySize
 * @param nextOffset
 * @return 
 */
bool lnNvm::parseEntry(int sector,int offset,int &entryID,int &entrySize,int &nextOffset)
{
    uint8_t s[4];
    if(!readSector(sector,offset,4,s)) return false;
    
    if(!valid(s[0])) return false;
    switch(s[0])
    {
        case NVM_ENTRY_UNUSED: return false;
        case NVM_ENTRY_BEGIN: 
                    entryID=NVM_INVALID_ENTRY;
                    setState(_currentSector,offset,NVM_ENTRY_SKIPPED);
                    nextOffset=offset+4;
                    return true;
        case NVM_ENTRY_SKIPPED: 
                    entryID=NVM_INVALID_ENTRY;
                    nextOffset=offset+4;
                    return true;
        case NVM_ENTRY_LEN_VALID:                     
                    entryID=NVM_INVALID_ENTRY;
                    nextOffset=offset+4+s[1];
                    return true;
            
        case NVM_SECTOR_INACTIVE:                    
                    entryID=NVM_INVALID_ENTRY;
                    nextOffset=offset+4+s[1];
                    return true;
        case NVM_SECTOR_ACTIVE:                                        
                    entryID=(s[2]<<8)+s[3];
                    nextOffset=offset+4+s[1];
                    return true;                    
        default:
                    xAssert(0);
                    return false; // Interrupted
    }
}

/**
 * 
 * @param id
 * @param size
 * @param data
 * @return 
 */
bool lnNvm::garbageCollect(int fromSector, int toSector)
{
    Logger("Garbage collecting from %d to %d...\n",fromSector,toSector);
    uint8_t tmp[256];
    int parsed=0;
    if(fromSector==NVM_INVALID_SECTOR) return false;    
    if(!prepareSector(toSector)) return false;
    
    
    // lookup the ID
    int offset=4;
    uint8_t s;
    int foundAt=-1;
    bool scanning=true;
    int entryID,entrySize,nextOffset;
    while(offset<LN_NVM_SECTOR_SIZE-12 && scanning)
    {
        if(!parseEntry(fromSector,offset,entryID,entrySize,nextOffset))
        {
            scanning=false;
        }
        if(!readSector(fromSector,offset+4,entrySize,tmp)) return false;
        if(!writeSector(toSector,entryID,entrySize,tmp)) return false;          
        offset=nextOffset;
        parsed++;
    }    
    if(!setState(toSector,0,NVM_SECTOR_ACTIVE)) return false;
    if(!setState(fromSector,0,NVM_SECTOR_INACTIVE)) return false;
    Logger("Garbage collector done, %d entries parsed \n",parsed);
    return true;
}


/**
 * 
 * @param id
 * @param size
 * @param data
 * @return 
 */
bool lnNvm::findLastEntryOf(int id, int size, int &offset)
{
    if(_currentSector==NVM_INVALID_SECTOR) return false;
    
    // lookup the ID
    offset=4;
    uint8_t s;
    int foundAt=-1;
    bool scanning=true;
    int entryID,entrySize,nextOffset;
    while(offset<LN_NVM_SECTOR_SIZE-12 && scanning)
    {
        if(parseEntry(_currentSector,offset,entryID,entrySize,nextOffset))
        {
            if(entryID==id)
            {
                if(size!=entrySize)
                {
                    Logger("Inconsistent size\n");
                    return false;
                }
                if(offset+4+size>LN_NVM_SECTOR_SIZE)
                {
                    Logger("Inconsistent size 2\n");
                    return false;
                }
                return true;
            }
        }else
        {
            return false;
        }
        offset=nextOffset;
    }    
    Logger("Cannot find entry %d\n",id);
    return false;    
}

/**
 * 
 * @param id
 * @param size
 * @param data
 * @return 
 */
bool lnNvm::read(int id, int size, uint8_t *data)
{
    xAutoMutex aut(&_mutex);
    if(_currentSector==NVM_INVALID_SECTOR) return false;
    int offset;
    if(!findLastEntryOf(id,size,offset))
    {
        return false;
    }  
    return readSector(_currentSector,offset+4,size,data);  
}
/**
 * 
 * @param writeOffset
 * @return 
 */
bool lnNvm::findWriteOffset(int &writeOffset) 
{
     if(_currentSector==NVM_INVALID_SECTOR) return false;
    
    // lookup the ID
    writeOffset=4;
    uint8_t s;
    int foundAt=-1;
    bool scanning=true;
    int entryID,entrySize,nextOffset;
    while(writeOffset<LN_NVM_SECTOR_SIZE-12 && scanning)
    {
        if(!readSector(_currentSector,writeOffset,1,&s)) return false;
        if(s==0xff) 
        {
            for(int i=writeOffset;i<LN_NVM_SECTOR_SIZE;i++)
            {
                  if(!readSector(_currentSector,i,1,&s)) return false;
                  if(s!=0xff) return false;
            }
            return true;
        }
        if(!parseEntry(_currentSector,writeOffset,entryID,entrySize,nextOffset))
            return false;
        writeOffset=nextOffset;            
    }
    return false;
}

/**
 * 
 * @param id
 * @param size
 * @param data
 * @return 
 */
bool lnNvm::write(int id, int size, uint8_t *data)
{
    xAutoMutex aut(&_mutex);
    if(_currentSector==NVM_INVALID_SECTOR) return false;
    int oldOffset=-1;
    if(!findLastEntryOf(id,size,oldOffset))
    {
        oldOffset=-1;
    }
    int writeOffset;
    if(!findWriteOffset(writeOffset) )
    {
        return false;
    }
    
    if(writeOffset>LN_NVM_SECTOR_SIZE-12)
    {
        
        if(!garbageCollect(_currentSector, (_currentSector+1)%_nbSectors)) return false;
        _currentSector=(_currentSector+1)%_nbSectors;
    }
    
    
    if(!setState(_currentSector,writeOffset,NVM_ENTRY_BEGIN)) return false;
    uint8_t tmp[2];
    tmp[0]=size;
    if(!writeSector(_currentSector,writeOffset+1,1,tmp)) return false;
    if(!setState(_currentSector,writeOffset,NVM_ENTRY_LEN_VALID)) return false;
    tmp[0]=id>>8;
    tmp[1]=id;
    if(!writeSector(_currentSector,writeOffset+2,2,tmp)) return false;        
    if(!writeSector(_currentSector,writeOffset+4,size,data)) return false;        
    if(!setState(_currentSector,writeOffset,NVM_ENTRY_VALID)) return false;
    if(oldOffset!=-1)
        if(!setState(_currentSector,oldOffset,NVM_ENTRY_INVALID)) return false;
    return true;
}
/**
 * 
 * @param sector
 * @return 
 */
bool lnNvm::prepareSector(int sector)
{
    // 1 -Erase
    if(!eraseSector(sector))
    {
        Logger("Cannot erase sector %d\n",sector);
        return false;
    }
    if(!verifyErase(sector))
    {
        Logger("Cannot erase sector check failed %d\n",sector);
        return false;
    }
    if(!setState(sector,0,NVM_SECTOR_PREPARING))
    {
        Logger("Cannot change state to preparing %d\n",sector);
        return false;
    }    
    return true;
}

/**
 * 
 * @param state
 * @return 
 */
bool lnNvm::setState(int sector, int offset, uint8_t state)
{
    uint8_t oldState=0;
    if(!readSector(sector,offset,1,&oldState))
    {
        return false;
    }
    // state should be old state with some bits set to 0
    if(!valid(state)) return false;
    if(!valid(oldState)) return false;
    if((state & oldState)!=state) return false;
    if(!writeSector(sector,offset,1,&state)) return false;
    if(!readSector(sector,offset,1,&oldState))
    {
        return false;
    }
    if(state!=oldState) return false;
    return true;
    
}