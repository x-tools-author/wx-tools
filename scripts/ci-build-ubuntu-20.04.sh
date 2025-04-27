# The workspace is the root of the repository
# The script is run from the root of the repository
cmake -DCMAKE_BUILD_TYPE=Release -DWXT_GENERATE_MSIX:BOOL=OFF -G "Ninja"
cmake --build . --target all
cmake --build . --target wxTools-deb