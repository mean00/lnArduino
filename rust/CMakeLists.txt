include(rnCmake)
SET(Rust_CARGO_TARGET "thumbv7m-none-eabi" CACHE INTERNAL "")
corrosion_import_crate(MANIFEST_PATH ${CMAKE_CURRENT_SOURCE_DIR}/rnArduino/Cargo.toml )

