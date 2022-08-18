If(NOT Rust_CARGO_TARGET)
    SET(Rust_CARGO_TARGET "thumbv7m-none-eabi" CACHE INTERNAL "")
ENDIF(NOT Rust_CARGO_TARGET)
# Lookup Corrosion
IF(NOT CORROSION_FOUND)
        #SET(CORROSION_FOUND True CACHE INTERNAL "")
    find_file (CORRODED CorrosionConfig.cmake
                        /usr/lib64/cmake/Corrosion
                        /usr/lib/cmake/Corrosion
                        /usr/local/lib64/cmake/Corrosion
                        /usr/local/lib/cmake/Corrosion )
    if(${CORRODED} STREQUAL "CORRODED-NOTFOUND" )
        MESSAGE(STATUS "Corrosion not found !")
    else()
        MESSAGE(STATUS "Corrosion : ${CORRODED}")
        get_filename_component(CORRODED ${CORRODED} DIRECTORY)
        SET(Corrosion_DIR "${CORRODED}")
    endif()
    find_package(Corrosion REQUIRED)
ENDIF(NOT CORROSION_FOUND)
