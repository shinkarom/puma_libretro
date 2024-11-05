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

add_library(nes_snd STATIC
	vendor/Nes_Snd_Emu/Simple_Apu.cpp
	vendor/Nes_Snd_Emu/nes_apu/apu_snapshot.cpp
	vendor/Nes_Snd_Emu/nes_apu/Blip_Buffer.cpp
	vendor/Nes_Snd_Emu/nes_apu/Multi_Buffer.cpp
	vendor/Nes_Snd_Emu/nes_apu/Nes_Apu.cpp
	vendor/Nes_Snd_Emu/nes_apu/Nes_Namco.cpp
	vendor/Nes_Snd_Emu/nes_apu/Nes_Oscs.cpp
	vendor/Nes_Snd_Emu/nes_apu/Nes_Vrc6.cpp
	vendor/Nes_Snd_Emu/nes_apu/Nonlinear_Buffer.cpp
	
)
target_include_directories(nes_snd PUBLIC 
	vendor/Nes_Snd_Emu
	vendor/Nes_Snd_Emu/boost
	vendor/Nes_Snd_Emu/nes_apu
)

add_library(${LIBNAME} SHARED 
    src/libretro.cpp
	src/cmixer.c
	src/apu.cpp
	src/cpu.cpp
	src/bus.cpp
	src/ppu.cpp
	src/input.cpp
	src/color.cpp
	src/channel.cpp
    )
set_target_properties(${LIBNAME} PROPERTIES
    SOVERSION 1
    PUBLIC_HEADER src/libretro.h
    LINKER_LANGUAGE CXX
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
target_link_libraries(${LIBNAME} musashi nes_snd)
target_include_directories(${LIBNAME} PUBLIC src)
