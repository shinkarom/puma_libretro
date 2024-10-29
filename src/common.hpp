#pragma once

#include <cstdint>
#include "libretro.h"

extern void (*wh_callback)(int w, int h);
extern void (*syscall_callback)(int address, int value);

constexpr int buttonMapping[12] = {
 RETRO_DEVICE_ID_JOYPAD_UP,
 RETRO_DEVICE_ID_JOYPAD_DOWN, 
 RETRO_DEVICE_ID_JOYPAD_LEFT,
 RETRO_DEVICE_ID_JOYPAD_RIGHT,
 RETRO_DEVICE_ID_JOYPAD_SELECT,
 RETRO_DEVICE_ID_JOYPAD_START,
 RETRO_DEVICE_ID_JOYPAD_A,
 RETRO_DEVICE_ID_JOYPAD_B, 
 RETRO_DEVICE_ID_JOYPAD_X,
 RETRO_DEVICE_ID_JOYPAD_Y,
 RETRO_DEVICE_ID_JOYPAD_L,
 RETRO_DEVICE_ID_JOYPAD_R
 };
constexpr int numButtons = 12;

constexpr auto maxScreenWidth = 256;
constexpr auto maxScreenHeight = 256;
constexpr auto maxScreenTotalPixels = maxScreenWidth * maxScreenHeight;

constexpr auto audioSampleRate = 44100;
constexpr auto framesPerSecond = 60;
constexpr int samplesPerFrame = audioSampleRate / framesPerSecond;
constexpr auto cpuClockRate = 40*1024*1024;
constexpr auto cyclesPerFrame = cpuClockRate / framesPerSecond;
constexpr auto totalMemory = 16*1024*1024;
constexpr auto codeOffset = 0x100000;
constexpr auto framebufferOffset = 0x200;

extern uint64_t frameNum;
extern int screenWidth;
extern int screenHeight;
extern int screenTotalPixels;