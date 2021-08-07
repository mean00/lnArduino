/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#include "lnArduino.h"
#include "nvmCore.h"
#include "nvmCore_internal.h"

#if 1
    #define VERBOSE Logger
#else
    #define  VERBOSE(x) {}
#endif
/**
 * 
 * @param nbSectors
 */
lnNvm::lnNvm(int nbSectors)
{
    _nbSectors=nbSectors;
    _currentSector=NVM_INVALID_SECTOR;
}
/**
 * 
 */
lnNvm::~lnNvm()
{
    
}

/**
 * 
 * @return 
 */
bool lnNvm::begin()
{
    // find first active sector
    int sec=-1;
    for(int i=0;i<_nbSectors && sec==-1;i++)
    {
        if(getSectorState(i)!=Nvm_Active) continue;
        sec=i;
    }
    //
    if(sec==-1)
    {
        Logger("No valid sector found, reformating\n");
        return false;    
    }
    _currentSector=sec;
    return true;
}


/**
 * Prepare sector 0
 * @return 
 */
bool lnNvm::format()
{
    xAutoMutex aut(&_mutex);
    _currentSector=NVM_INVALID_SECTOR;
    // Prepare sector 0
    if(!prepareSector(0))
    {
        return false;
    }
    if(!setSectorState(0,Nvm_Active))
    {
        return false;
    }
    _currentSector=0;
    return true;
}
/**
 * 
 * @param id
 * @param size
 * @param data
 * @return 
 */

bool lnNvm::findEntry(int id, uint32_t &offset, LN_NVM_ENTRY &entry)
{
    if(_currentSector==NVM_INVALID_SECTOR) return false;
    

    bool run=true;
    int processed=0;
    offset=4;
    while(run && offset<(LN_NVM_SECTOR_SIZE-8-2))
    {
        if(!readSector(_currentSector,offset,8,(uint8_t *)&entry)) 
        {
            return false;
        } 
        if(entry.id==0xffff) return false;
        int roundup=(entry.size+1)&0xfe;
        if(roundup>LN_NVM_MAX_PAYLOAD)
            return false;  
        if(entry.state!=0 && entry.state!=0xffff)
        {
            Logger("Broken entry at offset 0x%x\n",offset);
            return false;
        }
        if(id!=entry.id || entry.state==0)
        {
            offset+=8+roundup;
            continue;
        }      
        if(entry.state==0xffff) // valid
            return true;
        // invalid
         offset+=8+roundup;
         continue;
    }
    Logger("Entry %d not found\n",id);
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
    
    if(_currentSector==NVM_INVALID_SECTOR) return false;
    LN_NVM_ENTRY entry;
    uint32_t offset;
    if(!findEntry(id,offset,entry)) return false;
    
    if(entry.size!=size)
    {
        Logger("Inconistent size\n");
        return false;
    }    
    return readSector(_currentSector,offset+8,size,data);
}
/**
 * 
 * @param address
 * @return 
 */
bool lnNvm::getWriteAddress(uint32_t &offset)
{
   if(_currentSector==NVM_INVALID_SECTOR) return false;
    
    LN_NVM_ENTRY entry;
    bool run=true;
    int processed=0;
    offset=4;
    while(run && offset<=(LN_NVM_SECTOR_SIZE-8-2))
    {
        if(!readSector(_currentSector,offset,8,(uint8_t *)&entry)) 
        {
            return false;
        } 
        if(entry.id==0xffff && entry.size==0xffff) return true; // got it
        if(entry.size>LN_NVM_MAX_PAYLOAD) return false;
        int roundup=(entry.size+1)&0xfe;
        if(roundup>LN_NVM_MAX_PAYLOAD)
            return false;  
        if(entry.state!=0 && entry.state!=0xffff)
        {
            Logger("Broken entry at offset 0x%x\n",offset);
            return false;
        }
        // invalid
         offset+=8+roundup;
         continue;
    }
    Logger("getWriteAddress:: failed\n");
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
    if(_currentSector==NVM_INVALID_SECTOR) return false;
    uint32_t oldOffset;
    LN_NVM_ENTRY entry;    
    bool hasOldEntry=findEntry(id, oldOffset, entry);
    uint32_t writeAddress;
    
    
    if(!getWriteAddress(writeAddress)) return false;
    
    // 
    if((writeAddress+8+size)>=(LN_NVM_SECTOR_SIZE)*9/10)
    {
        if(!garbageCollection()) return false;
        if(!getWriteAddress(writeAddress)) return false;
    }
    int roundup=(size+1)&0xfe;
    entry.id=id;
    entry.size=size;
    entry.state=0xffff;
    entry.tag=0xffff;
    if(!writeSector(_currentSector,writeAddress,8,(uint8_t *)&entry)) return false;
    if(!writeSector(_currentSector,writeAddress+8,roundup,data)) return false;
    
    if(hasOldEntry)
    {
        uint16_t v=0;
        if(!writeSector(_currentSector,oldOffset+4,2,(uint8_t *)&v)) return false; // invalidate old entry
    }
    return true;
}

/**
 * 
 * @param sector
 * @return 
 */
bool lnNvm::prepareSector(int sector)
{
    if(!eraseSector(sector)) return false;
    return setSectorState(sector,Nvm_Prep);
}
/**
 * 
 * @param sector
 * @param state
 * @return 
 */
bool lnNvm::setSectorState(int sector,LN_NVM_SECTOR_STATE state)
{
    LN_NVM_SECTOR_STATE oldState=getSectorState(sector);
    uint16_t val=0;
    if(oldState==state) return true;
    switch(state+oldState*16)
    {
        case 1:// erase=>prep
                return writeSector(sector,0,2,(uint8_t *)&val);
                break;
        case 18: // pre => active
                return writeSector(sector,2,2,(uint8_t *)&val);
                break;
        default:
            return false;
            break;
    }
}
/**
 * 
 * @param sector
 * @return 
 */
LN_NVM_SECTOR_STATE lnNvm::getSectorState(int sector)
{
    uint16_t a,b;
    if(!readSector(sector,0,2,(uint8_t *)&a)) return Nvm_Error;
    if(!readSector(sector,2,2,(uint8_t *)&b)) return Nvm_Error;
    if(a==0 && b==0xffff) return Nvm_Prep;
    if(a==0 && b==0x00) return Nvm_Active;
    if(a==0xffff && b==0xffff) return Nvm_Erased;
    return Nvm_Erased;
}
/**
 * 
 * @return 
 */
bool lnNvm::garbageCollection()
{
    if(_currentSector==NVM_INVALID_SECTOR) return false;
    
    uint8_t tmp[LN_NVM_MAX_PAYLOAD];
    int offset=4;
    int otherSector=(_currentSector+1)%_nbSectors;
    if(!prepareSector(otherSector)) return false;
    
    LN_NVM_ENTRY entry;
    bool run=true;
    int processed=0;
    int writeOffset=4;
    VERBOSE("Garbage collecting\n");
    while(run && offset<(LN_NVM_SECTOR_SIZE-8-2))
    {
        if(!readSector(_currentSector,offset,8,(uint8_t *)&entry)) 
        {
            run=false;
            continue;
        }
        int roundup=(entry.size+1)&0xfe;
        if(entry.id==0xffff)
        {
            VERBOSE("End of scan at offset %x  \n",offset);
            run=false;
            break;
        }
        if(entry.size>LN_NVM_MAX_PAYLOAD)
        {
            VERBOSE("Payload exceeded %x :%d \n",offset,entry.size);
            run=false;
            continue;
        }
        if(entry.state!=0xffff)
        {
            VERBOSE("Invalid entry at %x  :%x  \n",offset,entry.state);
            offset=offset+8+roundup;
            continue;
        }
        if(!readSector(_currentSector,offset+8,entry.size,tmp))
        {
            VERBOSE("Read payload error entry at %x  :%x \n",offset);
            run=false;
            continue;
        }
        VERBOSE("Found valid entry %d  \n",entry.id);        
        offset=offset+8+roundup;
        // Write
        entry.state=0xffff;
        if(!writeSector(otherSector,writeOffset,8,(uint8_t *)&entry))
        {
            run=false;
            continue;
        }
        writeOffset+=8;
        if(!writeSector(otherSector,writeOffset,roundup,tmp))
        {
            run=false;
            continue;
        }
        writeOffset+=roundup;
        processed++;        
        
    }
    Logger("Garbage collecting done %d entries processed\n",processed);
    if(!setSectorState(otherSector,Nvm_Active)) return false;
    int previousCurrent=_currentSector;
    _currentSector=otherSector;
    eraseSector(previousCurrent);
    Logger("Garbage collecting ok\n");
    return true;    
}
/**
 */
bool lnNvm::dumpEntries(int sector)
{

    bool run=true;
    int processed=0;
    int offset=4;
    LN_NVM_ENTRY entry;
    while(run && offset<(LN_NVM_SECTOR_SIZE-8-2))
    {
        if(!readSector(sector,offset,8,(uint8_t *)&entry)) 
        {
            return false;
        } 
        if(entry.id==0xffff) return false;
        int roundup=(entry.size+1)&0xfe;
        if(roundup>LN_NVM_MAX_PAYLOAD)
            return false;  
        if(entry.state!=0 && entry.state!=0xffff)
        {
            Logger("Broken entry at offset 0x%x\n",offset);
            return false;
        }
        Logger("Offset=%x  \n",offset);
        Logger("ID    =%x  \n",entry.id);
        Logger("size  =%x  \n",entry.size);
        Logger("state =%x  \n",entry.state);
        // invalid
         offset+=8+roundup;
         continue;
    }
    return true;
}
// EOF