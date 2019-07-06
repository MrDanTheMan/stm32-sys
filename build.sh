#!/usr/bin/env bash


echo Setting up build directory ...
echo 
rm -rf ./build
mkdir ./build
cp ./stm32f401re.ld ./build/stm32f401re.ld

echo Runing cmake commands ...
echo 
cmake .  -Bbuild -DCMAKE_TOOLCHAIN_FILE=./cmake-arm-toolchain.cmake
cmake .  --build build -DCMAKE_TOOLCHAIN_FILE=./cmake-arm-toolchain.cmake

echo 
echo Runing make commands ...
echo

cd ./build
make

echo
echo Converting to arm bin executable ...

cd ..
arm-none-eabi-objcopy -O binary ./build/bin/stm32-minimal ./build/bin/stm32-minimal.bin

echo Generating OpenOCD config files for debugging
cp ./openocd/nucleo_stm32f401.cfg ./build/bin/nucleo_stm32f401.cfg

echo
echo Finished !