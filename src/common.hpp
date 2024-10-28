#pragma once

#include <cstdint>

constexpr auto screenWidth = 256;
constexpr auto screenHeight = 384;
constexpr auto screenTotalPixels = screenWidth * screenHeight;

constexpr auto audioSampleRate = 44100;
constexpr auto framesPerSecond = 60;
constexpr int samplesPerFrame = audioSampleRate / framesPerSecond;
constexpr auto cpuClockRate = 40*1024*1024;
constexpr auto cyclesPerFrame = cpuClockRate / framesPerSecond;
constexpr auto totalMemory = 16*1024*1024;
constexpr auto codeOffset = 0x400;

extern uint64_t frameNum;