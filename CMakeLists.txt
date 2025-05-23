cmake_minimum_required(VERSION 3.28.2)

include(../sdk/config.cmake)
project(sieelf_scalka C)

include_directories("../libsie/src/include")
link_libraries(crt crt_helper gcc c m ft sie)
file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(SCalka_ELKA ${SOURCES})
target_sdk_setup(SCalka_ELKA ELKA)
target_link_directories(SCalka_ELKA PUBLIC "../libsie/build/lib/ELKA")
target_compile_options(SCalka_ELKA BEFORE PUBLIC -std=c11 -Wall -Werror -DSWILIB_MODERN)
