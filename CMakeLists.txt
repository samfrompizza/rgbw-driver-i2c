cmake_minimum_required(VERSION 3.16)
project(rgbw_driver_i2c C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Werror)

option(BUILD_HOST_TESTS "Build x86 tests with mock I2C" ON)
option(BUILD_CORTEX_M0 "Build Cortex-M0 driver-only target" ON)

add_library(rgbw_driver_core STATIC
    driver/rgbw-driver.c
)
target_include_directories(rgbw_driver_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(rgbw_driver STATIC
    i2c-api/i2chw.c
)
target_link_libraries(rgbw_driver PUBLIC rgbw_driver_core)
target_include_directories(rgbw_driver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(BUILD_HOST_TESTS)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(CHECK REQUIRED check)

    add_executable(rgbw_driver_tests
        tests/test_rgbw_driver.c
    )
    target_link_libraries(rgbw_driver_tests PRIVATE rgbw_driver ${CHECK_LIBRARIES} m)
    target_include_directories(rgbw_driver_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CHECK_INCLUDE_DIRS})
    target_compile_options(rgbw_driver_tests PRIVATE ${CHECK_CFLAGS_OTHER})
    target_link_directories(rgbw_driver_tests PRIVATE ${CHECK_LIBRARY_DIRS})

    enable_testing()
    add_test(NAME rgbw_driver_tests COMMAND rgbw_driver_tests)
endif()

if(BUILD_CORTEX_M0)
    add_library(rgbw_driver_cortex_m0 STATIC
        driver/rgbw-driver.c
    )
    target_include_directories(rgbw_driver_cortex_m0 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_compile_definitions(rgbw_driver_cortex_m0 PRIVATE RGBW_TARGET_CORTEX_M0=1)

    if(CMAKE_C_COMPILER_TARGET MATCHES "arm" OR CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm|ARM)")
        target_compile_options(rgbw_driver_cortex_m0 PRIVATE
            -mcpu=cortex-m0
            -mthumb
            -ffreestanding
        )
    endif()
endif()