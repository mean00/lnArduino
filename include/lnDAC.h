/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 * 
 * 
 * In simpleMode the setup is as follows :
 * simpleMode();
 * enable();
 * setValue(xxx);
 * setValue(xxx);
 * 
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
   lnPin   pin();
   void    enable();
   void    disable();
   void    simpleMode(); // call this if you just want to manually set the dac value
   void    setValue(int value); // 12 bits value 00..495
        
protected:
        int _instance;
};