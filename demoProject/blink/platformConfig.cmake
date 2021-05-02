
IF(WIN32)    
    SET(PLATFORM_TOOLCHAIN_PATH /c/gd32/toolchain/bin/) # Use /c/foo or c:\foo depending if you use mingw cmake or win32 cmake
    SET(PLATFORM_PREFIX riscv32-unknown-elf-)
    SET(PLATFORM_TOOLCHAIN_SUFFIX ".exe")
    SET(PLATFORM_C_FLAGS "-march=rv32imac -mabi=ilp32 -mcmodel=medlow")
ELSE(WIN32)
    #SET(PLATFORM_TOOLCHAIN_PATH /opt/gd32/toolchain/bin/)
    SET(PLATFORM_TOOLCHAIN_PATH /opt/gd32/toolchain2/bin/)
    SET(PLATFORM_PREFIX riscv32-unknown-elf-)
    SET(PLATFORM_TOOLCHAIN_SUFFIX "") # .exe on windows
    SET(PLATFORM_C_FLAGS "-march=rv32imac -mabi=ilp32 -mcmodel=medlow")
ENDIF(WIN32)
