include(rnCmake)
RUST_ADD( rsPowerSupply ${CMAKE_CURRENT_SOURCE_DIR}/Cargo.toml)
#corrosion_import_crate(MANIFEST_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Cargo.toml CFLAGS ${LN_LTO_RUST_FLAGS} )


