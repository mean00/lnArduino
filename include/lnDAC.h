/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#pragma once
/**
 * 
 * @param instance
 */
class lnDAC
{
public:
            lnDAC(int instance);
   virtual ~lnDAC();
   void    enable();
   void    disable();
   void    simpleMode();
   void    setValue(int value); // 12 bits value 00..495
        
protected:
        int _instance;
};