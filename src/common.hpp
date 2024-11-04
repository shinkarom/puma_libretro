#pragma once

#include <cstdint>

extern void (*wh_callback)(int w, int h);

constexpr int numButtons = 12;
constexpr int numInputs = 130;

constexpr auto maxScreenWidth = 256;
constexpr auto maxScreenHeight = 256;
constexpr auto maxScreenTotalPixels = maxScreenWidth * maxScreenHeight;

constexpr auto audioSampleRate = 44100;
constexpr auto framesPerSecond = 60;
constexpr int samplesPerFrame = audioSampleRate / framesPerSecond;
constexpr auto cpuClockRate = 40*1024*1024;
constexpr auto cyclesPerFrame = cpuClockRate / framesPerSecond;
constexpr auto totalMemory = 64*1024*1024;
constexpr auto ramSize = 8 * 1024 * 1024;
constexpr auto codeOffset = ramSize;
constexpr auto stackOffset = codeOffset;
constexpr auto numApuRegisters = 0x106;

constexpr auto syscallStackAddress = 0xFFFFFFF9;
constexpr auto syscallAddress = 0xFFFFFFFC;

extern uint64_t frameNum;
extern int screenWidth;
extern int screenHeight;
extern int screenTotalPixels;