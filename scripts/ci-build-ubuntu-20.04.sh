# The workspace is the root of the repository
# The script is run from the root of the repository
mkdir -p build
cd build

# first arg
if [ "$1" == "daily" ]; then
  cmake -DCMAKE_BUILD_TYPE=Release -DWXT_DAILY_BUILD:BOOL=ON -DWXT_GENERATE_MSIX:BOOL=OFF -G "Ninja" -S .. -B .
else
  cmake -DCMAKE_BUILD_TYPE=Release -DWXT_GENERATE_MSIX:BOOL=OFF -G "Ninja" -S .. -B .
fi

cmake --build . --target all --parallel 4
cmake --build . --target wxTools-deb