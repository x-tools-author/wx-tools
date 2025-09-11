if exist build-arm64 rd /s /q build-arm64
mkdir build-arm64
cd build-arm64

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsamd64_arm64.bat"

cmake -DCMAKE_BUILD_TYPE=Release -DWXT_PORTABLE_EDITION:BOOL=OFF -DWXT_BUILD_TESTS:BOOL=OFF -G "Ninja" ../../../
cmake --build . --target wxTools-msix
