cmake_minimum_required(VERSION 2.8)

project(stm32-minimal C)

# Binary output paths
set(CMAKE_BINARY_DIR ./bin/)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(PROJECT_SOURCE_DIR ./source/)

# STM32-ARM CMSIS paths
set(STM32_DRIVER_DIR ./Drivers/)
set(STM32_DRIVER_DEVICE ${STM32_DRIVER_DIR}/Device/ST/)
set(STM32_DRIVER_CORE ${STM32_DRIVER_DIR}/Include/)

# Setup project include headers
include_directories(${STM32_DRIVER_DEVICE}/${MCU_FAMILY}/Include)
include_directories(${STM32_DRIVER_CORE}/)
include_directories(${PROJECT_SOURCE_DIR})

# STM32-ARM driver source
#file(GLOB STM_DRIVER_SRC ${STM32_DRIVER_DIR}/${MCU_FAMILY}/Source/Templates/system_${MCU_FAMILY}.c)

# Main project (user) source files
set(SOURCES 
${PROJECT_SOURCE_DIR}/sys_stm32f4.c
${PROJECT_SOURCE_DIR}/crt0.c 
${PROJECT_SOURCE_DIR}/clock.c 
${PROJECT_SOURCE_DIR}/gpio.c 
${PROJECT_SOURCE_DIR}/uart.c 
${PROJECT_SOURCE_DIR}/main.c )

# Linker script setup
set(LINKER_SCRIPT ./stm32f401re.ld)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${LINKER_SCRIPT}")


add_executable(stm32-minimal ${SOURCES} ${STM_DRIVER_SRC})