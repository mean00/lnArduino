IF(WIN32)    
    SET(PLATFORM_TOOLCHAIN_SUFFIX ".exe")
ENDIF(WIN32)

IF("${LN_ARCH}" STREQUAL "RISCV") # RISCV
  IF( ${LN_MCU} STREQUAL "CH32V3x")    #----------CH32V307
    #SET(PLATFORM_PREFIX riscv-none-embed-) # MRS toolchain
    IF(WIN32)    
      SET(PLATFORM_TOOLCHAIN_PATH todo_todo) # Use /c/foo or c:\foo depending if you use mingw cmake or win32 cmake
    ELSE(WIN32)
      
      #SET(PLATFORM_PREFIX "riscv-none-embed-") # MRS toolchain
      #SET(PLATFORM_TOOLCHAIN_TRIPLET "riscv-none-embed-")
      SET(PLATFORM_TOOLCHAIN_PATH "/riscv/tools2/bin")    
      SET(PLATFORM_C_FLAGS "-march=rv32imac -mabi=ilp32 ")
      SET(PLATFORM_PREFIX "riscv64-unknown-elf-") # MRS toolchain      
      # FOR CLANG  
      
      SET(PLATFORM_TOOLCHAIN_TRIPLET "riscv64-unknown-elf-")      
      #-- CLANG --
      SET(PLATFORM_CLANG_C_FLAGS "--target=riscv32 -march=rv32imac -mabi=ilp32 ")      
      SET(PLATFORM_CLANG_PATH  "/riscv/tools_llvm/bin")
      SET(PLATFORM_CLANG_VERSION "-16")
      SET(PLATFORM_CLANG_SYSROOT "/riscv/tools_llvm/lib/clang-runtimes/riscv32-none-eabi/riscv32/")
      #SET(PLATFORM_CLANG_VERSION "-14")
      #SET(PLATFORM_CLANG_SYSROOT "/riscv/llvm_xpack_14/")

    ENDIF(WIN32)
  ELSE( ${LN_MCU} STREQUAL "CH32V3x") #----------- GD32VF103
    SET(PLATFORM_PREFIX riscv32-unknown-elf-)
    SET(PLATFORM_C_FLAGS "-march=rv32imac -mabi=ilp32 ")
    IF(WIN32)    
      SET(PLATFORM_TOOLCHAIN_PATH /c/gd32/toolchain/bin/) # Use /c/foo or c:\foo depending if you use mingw cmake or win32 cmake
    ELSE(WIN32)    
      SET(PLATFORM_TOOLCHAIN_PATH /opt/gd32/toolchain/bin/)
    ENDIF(WIN32)
  ENDIF()
ELSE()
   SET(PLATFORM_PREFIX arm-none-eabi-)
   SET(PLATFORM_C_FLAGS " ")
   IF(WIN32)
      SET(PLATFORM_TOOLCHAIN_PATH  "/c/dev/arm83/bin")
   ELSE()
           #SET(PLATFORM_TOOLCHAIN_PATH "/home/fx/Arduino_stm32/arm-gcc-2020q2/bin")
      #SET(PLATFORM_CLANG_PATH  /opt/llvm/bin)
      #SET(PLATFORM_CLANG_VERSION "-14")
      #SET(PLATFORM_CLANG_PATH  /opt/llvm_arm/LLVMEmbeddedToolchainForArm-14.0.0/bin)
      #SET(PLATFORM_CLANG_VERSION "")
      SET(PLATFORM_CLANG_PATH  /opt/llvm_arm/14/bin)
      SET(PLATFORM_CLANG_VERSION "")
      SET(PLATFORM_TOOLCHAIN_PATH "/home/fx/Arduino_stm32/arm-gcc-2021q4/bin")
   ENDIF()
ENDIF()

