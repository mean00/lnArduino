include(rnCmake)
SET(Rust_CARGO_TARGET "thumbv7m-none-eabi" CACHE INTERNAL "")

IF(USE_USB) # need to add LN_USB_NB_CDC >0
    SET(FEAT NO_DEFAULT_FEATURES FEATURES="cdc" )
ENDIF()

corrosion_import_crate(MANIFEST_PATH ${CMAKE_CURRENT_SOURCE_DIR}/rnArduino/Cargo.toml ${FEAT})

