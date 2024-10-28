cmake_minimum_required(VERSION 3.9)
set(LIBNAME puma_libretro)
project(${LIBNAME} LANGUAGES C CXX VERSION 1.0.1 DESCRIPTION "Libretro core")
set(CMAKE_SHARED_LIBRARY_PREFIX )

add_library(musashi STATIC 
	src/musashi/softfloat/softfloat.c
	src/musashi/m68kcpu.c
	src/musashi/m68kdasm.c
	src/musashi/m68kfpu.c
	src/musashi/m68kops.c
)
target_include_directories(musashi PUBLIC src/musashi)

add_library(${LIBNAME} SHARED 
    src/libretro.cpp
	src/opl3.c
	src/apu.cpp
	src/cpu.cpp
	src/bus.cpp
	src/ppu.cpp
    )
set_target_properties(${LIBNAME} PROPERTIES
    SOVERSION 1
    PUBLIC_HEADER src/libretro.h
    LINKER_LANGUAGE CXX
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
target_link_libraries(${LIBNAME} musashi)
target_include_directories(${LIBNAME} PUBLIC src)
