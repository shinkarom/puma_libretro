#pragma once

#include <cstdint>

constexpr int numButtons = 12;
constexpr int numInputs = 130;

constexpr auto screenWidth = 160;
constexpr auto screenHeight = 160;
constexpr auto screenTotalPixels = screenWidth * screenHeight;

constexpr auto audioSampleRate = 44100;
constexpr auto framesPerSecond = 60;
constexpr int samplesPerFrame = audioSampleRate / framesPerSecond;
constexpr auto cpuClockRate = 40*1024*1024;
constexpr auto cyclesPerFrame = cpuClockRate / framesPerSecond;
constexpr auto totalMemory = 32*1024*1024;
constexpr auto systemMemorySize = 1 * 1024 * 1024;
constexpr auto systemMemoryStart = totalMemory - systemMemorySize;
constexpr auto maxFileSize = totalMemory - 0x200 - systemMemorySize;
constexpr auto codeOffset = 0x200;
constexpr auto stackOffset = totalMemory;
constexpr auto numApuRegisters = 0x106;

constexpr auto syscallStackAddress = 0xFFFFFFF9;
constexpr auto syscallAddress = 0xFFFFFFFC;

constexpr int fakeDMCStartAddress = 0xC000;

constexpr auto transparentColor = 0xFF000000;