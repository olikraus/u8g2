cmake_minimum_required(VERSION 3.5)
project(u8g2)

set(COMPONENT_ADD_INCLUDEDIRS csrc)
file(GLOB COMPONENT_SRCS csrc/*.c)
file(GLOB COMPONENT_HDRS csrc/*.h)

if(COMMAND register_component)
    set(COMPONENT_NAME ".")
    register_component()
else()
    add_library(u8g2 ${COMPONENT_SRCS})
    target_include_directories(u8g2 PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/csrc>" "$<INSTALL_INTERFACE:include>")
endif()

install(TARGETS u8g2
        EXPORT u8g2-targets
        ARCHIVE DESTINATION lib)

install(FILES ${COMPONENT_HDRS}
        DESTINATION include/u8g2)

install(EXPORT u8g2-targets
        DESTINATION lib/cmake/u8g2)

install(FILES ${u8g2_SOURCE_DIR}/pkg/u8g2-config.cmake
        DESTINATION lib/cmake/u8g2)
