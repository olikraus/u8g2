cmake_minimum_required(VERSION 3.16)

project(u8g2_4wire_hw_spi)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D __ARM_LINUX__")

set(SOURCE_EXE u8g2_4wire_hw_spi.cpp)

set (U8G2_PATH ../../../../../)

AUX_SOURCE_DIRECTORY(${U8G2_PATH}/csrc/ CSRC_FILES)
AUX_SOURCE_DIRECTORY(${U8G2_PATH}/sys/arm-linux/drivers DRV_FILES)
AUX_SOURCE_DIRECTORY(${U8G2_PATH}/sys/arm-linux/port/ PORT_FILES)

set(SOURCE_LIB ${CSRC_FILES} ${DRV_FILES} ${PORT_FILES})
include_directories(${U8G2_PATH}/csrc/)
include_directories(${U8G2_PATH}/sys/arm-linux/drivers)
include_directories(${U8G2_PATH}/sys/arm-linux/port/)

add_library(u8g2 SHARED ${SOURCE_LIB})

add_executable(u8g2_4wire_hw_spi ${SOURCE_EXE})
target_link_libraries(u8g2_4wire_hw_spi u8g2)
