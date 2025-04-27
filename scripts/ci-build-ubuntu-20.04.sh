# The workspace is the root of the repository
# The script is run from the root of the repository
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DWXT_GENERATE_MSIX:BOOL=OFF -G "Ninja" -S .. -B .
cmake --build . --target all --parallel 4
cmake --build . --target wxTools-appimage
cmake --build . --target wxTools-deb