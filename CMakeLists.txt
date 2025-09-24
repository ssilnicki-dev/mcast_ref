cmake_minimum_required(VERSION 3.20)
project(synchub LANGUAGES C)

set(CMAKE_C_COMPILER clang)

set(LINUX_SYSROOT "/compat/linux")
set(LINUX_TRIPLE x86_64-linux-gnu)

set(SOURCES
    mcast_join.c
)

add_compile_options(-Werror)
add_executable(f_mcast ${SOURCES})

add_executable(l_mcast ${SOURCES})
target_include_directories(l_mcast PRIVATE "${LINUX_SYSROOT}/lib/gcc/x86_64-linux-gnu/11/include")

target_compile_options(l_mcast PRIVATE --target=${LINUX_TRIPLE} --sysroot=${LINUX_SYSROOT})
target_link_options(l_mcast PRIVATE --target=${LINUX_TRIPLE} --sysroot=${LINUX_SYSROOT})
