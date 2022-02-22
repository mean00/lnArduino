

#pragma once

class lnTUSB
{
public:
              lnTUSB();
      virtual ~lnTUSB();
      void    init();
      void    start();
      void    stop();
public:      
      void    task();
protected:
      bool     _running;
};