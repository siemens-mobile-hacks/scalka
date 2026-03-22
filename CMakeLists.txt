cmake_minimum_required(VERSION 3.28.2)

include(../sdk/config.cmake)
project(sieelf_scalka C)

link_libraries(crt crt_helper gcc c m)
file(GLOB_RECURSE SOURCES "src/*.c")

add_definitions(-DSWILIB_MODERN)

add_executable(SCalka_ELKA ${SOURCES})
target_sdk_setup(SCalka_ELKA ELKA)
target_compile_options(SCalka_ELKA BEFORE PUBLIC -std=c11 -Wall -Werror)
