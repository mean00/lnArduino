
IF(WIN32)    
    SET(PLATFORM_TOOLCHAIN_PATH /c/gd32/toolchain/bin/) # Use /c/foo or c:\foo depending if you use mingw cmake or win32 cmake
    SET(PLATFORM_PREFIX riscv32-unknown-elf-)
    SET(PLATFORM_TOOLCHAIN_SUFFIX ".exe")
ELSE(WIN32)
    SET(PLATFORM_TOOLCHAIN_PATH /opt/gd32/toolchain/bin/)
    SET(PLATFORM_PREFIX riscv32-unknown-elf-)
    SET(PLATFORM_TOOLCHAIN_SUFFIX "") # .exe on windows
ENDIF(WIN32)
