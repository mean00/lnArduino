

#pragma once
#include "stdint.h"
class lnRingBuffer
{
    public:
        lnRingBuffer(int size)
        {
            int c=size;
            while(c) // make sure it is a power of 2
            {
                if(c==1) break;
                xAssert( !(c&1));
                c>>=1;
            }
            _size=size;
            _mask=size-1;
            _head=_tail=0;
            _buffer = new uint8_t[size];
        }
        virtual ~lnRingBuffer()
        {
            delete _buffer;
            _buffer=NULL;
        }
       
        bool empty() { return (_head==_tail);}
        bool full()  { return (_head==(_tail+_size));}
        int  count() {return _head-_tail;}
        int  free() {return _size-(_head-_tail);}
        int  CMIN(int a,int b) { if(a<b) return a; return b;}
        void flush() { _tail = _head =0;}
        

        //----------------------------------
        void consume(int n) 
        {
            _tail+=n; 
            if(_tail==_head) 
            {
               flush();
            }
        }
        int  getReadPointer(uint8_t **to)
        {
            *to=_buffer+(_tail&_mask);
            uint32_t h=_head & _mask;
            uint32_t t=_tail & _mask;
            if(h>=t)
            {
                return h-t;
            }
            return _size-h;
        }
        bool put(int insize, const uint8_t *data)
        {            
            int size = CMIN(insize,free());
            // FIXME : optimize!
            for(int i=0;i<size;i++)
            {
                _buffer[_head++ & _mask] = data[i];
                
            }                       
            return true;
        }
        int get(int size, uint8_t *data)
        {            
            size = CMIN(size,count());
            for(int i=0;i<size;i++)
            {
                data[i]=_buffer[_tail & _mask];
                _tail++;
            }         
            if(_tail==_head) 
                flush();
            return size;
        }
        //----------------------------------
    protected:
        int _head,_tail;
        int _mask,_size;
        uint8_t *_buffer;
};