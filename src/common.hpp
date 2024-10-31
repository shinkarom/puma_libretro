#pragma once

#include <cstdint>

extern void (*wh_callback)(int w, int h);

enum {
	API_printRegisters = 0,
	API_getDimensions,
	API_setPixel,
	API_getPixel,
	API_setDimensions,
	API_getFrameNumber,
	API_writeAudioRegister,
	API_isPressed,
	API_isJustPressed,
	API_isJustReleased,
	API_waitForVBlank,
	API_cls,
} apiFunctions;

constexpr int numButtons = 12;
constexpr int numKeys = 104;

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
constexpr auto stackOffset = codeOffset;
constexpr auto framebufferOffset = 0x200;
constexpr auto apuRegistersOffset = 0x80000;
constexpr auto numApuRegisters = 0x106;

constexpr auto syscallAddress = 0xFFFFFF00;

extern uint64_t frameNum;
extern int screenWidth;
extern int screenHeight;
extern int screenTotalPixels;