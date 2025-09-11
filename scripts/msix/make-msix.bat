if exist build rd /s /q build
mkdir build
cd build

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

cmake -DCMAKE_BUILD_TYPE=Release -DWXT_PORTABLE_EDITION:BOOL=OFF -G "Ninja" ../../../
cmake --build . --target wxTools-msix

