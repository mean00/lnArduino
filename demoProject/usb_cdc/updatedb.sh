
mkdir -p build
cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=True ..
cd ..

compdb -p  build/ list > compile_commands.json
