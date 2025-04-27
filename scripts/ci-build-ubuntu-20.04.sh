# The workspace is the root of the repository
# The script is run from the root of the repository
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DWXT_GENERATE_MSIX:BOOL=OFF -G "Ninja" -S .. -B .
cmake --build ./build --target all
cmake --build ./build --target wxTools-appimage
cmake --build ./build --target wxTools-deb