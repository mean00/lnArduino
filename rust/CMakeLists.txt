SET(Rust_CARGO_TARGET "thumbv7m-none-eabi")
find_package(Corrosion REQUIRED)
corrosion_import_crate(MANIFEST_PATH rnArduino/Cargo.toml)

