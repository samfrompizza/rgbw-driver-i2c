cmake_minimum_required(VERSION 3.16)
project(rgbw_driver_i2c C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Werror)

add_library(rgbw_driver
    driver/rgbw-driver.c
    i2c-api/i2chw.c
)
target_include_directories(rgbw_driver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(rgbw_driver_tests
    tests/test_rgbw_driver.c
    driver/rgbw-driver.c
    i2c-api/i2chw.c
)
target_include_directories(rgbw_driver_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_test(NAME rgbw_driver_tests COMMAND rgbw_driver_tests)