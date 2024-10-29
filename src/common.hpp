#pragma once

#include <cstdint>

extern void (*wh_callback)(int w, int h);
extern void (*syscall_callback)(int address, int value);

constexpr int buttonMapping[12] = {4, 5, 6, 7, 2, 3, 8, 0, 9, 1, 10, 11};
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