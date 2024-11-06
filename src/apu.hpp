#pragma once

#include "common.hpp"
#include "Simple_Apu.h"
#include <cstdint>

namespace apu {
	void init();
	void deinit();
	void afterFrame();
	int16_t* callback();
	void writeReg(int reg, int value);
	void setDMCStart(uint32_t start, uint32_t end);
	
	void initChannel(int chanNum, uint32_t start, uint32_t end, int smplRate);
}