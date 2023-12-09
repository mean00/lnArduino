

#pragma once
#include "stdint.h"
class lnRingBuffer
{
  public:
    /**
     * @brief Construct a new ln Ring Buffer object
     *
     * @param size
     */
    lnRingBuffer(int size)
    {
        int c = size;
        xAssert(size == (size & ~(size - 1))); // make sure it is a power of 2
        _size = size;
        _mask = size - 1;
        _head = _tail = 0;
        _buffer = new uint8_t[size];
    }
    /**
     * @brief Destroy the ln Ring Buffer object
     *
     */
    virtual ~lnRingBuffer()
    {
        delete _buffer;
        _buffer = NULL;
    }
    /**
     * @brief
     *
     * @return int
     */
    int size()
    {
        return _size;
    }
    /**
     * @brief
     *
     * @return true
     * @return false
     */
    bool empty()
    {
        return (_head == _tail);
    }
    /**
     * @brief
     *
     * @return true
     * @return false
     */
    bool full()
    {
        return (_head == (_tail + _size - 1));
    }
    /**
     * @brief
     *
     * @return int
     */
    int count()
    {
        return _head - _tail;
    }
    /**
     * @brief
     *
     * @return int
     */
    int free()
    {
        return _size - (_head - _tail) - 1;
    }
    /**
     * @brief
     *
     * @param a
     * @param b
     * @return int
     */
    static inline int CMIN(int a, int b)
    {
        if (a <= b)
            return a;
        return b;
    }
    /**
     * @brief
     *
     */
    void flush()
    {
        _tail = _head = 0;
    }
    /**
     * @brief
     *
     * @return uint8_t*
     */
    uint8_t *ringBuffer()
    {
        return _buffer;
    }

    //----------------------------------
    void consume(int n)
    {
        _tail += n;
        if (_tail == _head)
        {
            flush();
        }
    }
    /**
     * @brief Get the Read Pointer object
     *
     * @param to
     * @return int
     */
    int getReadPointer(uint8_t **to)
    {
        uint32_t h = _head & _mask;
        uint32_t t = _tail & _mask;
        *to = _buffer + t;
        int nb;
        if (h >= t)
            nb = h - t;
        else
            nb = _size - t;
        return nb;
    }
    /**
     * @brief
     *
     * @param insize
     * @param data
     * @return true
     * @return false
     */
    int put(int insize, const uint8_t *data)
    {
        int size = CMIN(insize, free());
        int orgsize = size;
        // FIXME : optimize!
        int c;
        while (size)
        {
            int h = _head & _mask;
            int t = _tail & _mask;
            if (h >= t)
            { // copy till the end
                c = CMIN(_size - h, size);
            }
            else
            {
                c = CMIN(t - h - 1, size);
            }
            memcpy(_buffer + h, data, c);
            _head += c;
            data += c;
            size -= c;
        }
        return orgsize;
    }
    /**
     * @brief
     *
     * @param size
     * @param data
     * @return int
     */
    int get(int size, uint8_t *data)
    {
        size = CMIN(size, count());
        int orgsize = size;
        int c;
        while (size)
        {
            int h = _head & _mask;
            int t = _tail & _mask;
            if (h > t)
            {
                c = CMIN(h - t, size);
            }
            else
            {
                c = CMIN(_size - t, size);
            }
            memcpy(data, _buffer + t, c);
            _tail += c;
            data += c;
            size -= c;
        }
        // for(int i=0;i<size;i++)            {                data[i]=_buffer[_tail & _mask];                _tail++;
        if (_tail == _head)
            flush();
        return orgsize;
    }
    //----------------------------------
  protected:
    int _head, _tail;
    int _mask, _size;
    uint8_t *_buffer;
};