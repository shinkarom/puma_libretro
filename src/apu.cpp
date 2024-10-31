#include "apu.hpp"
#include "opl3.h"
#include <cstring>
#include <iostream>

#include "common.hpp"

namespace apu {
	opl3_chip chip;
	int16_t audioBuffer[samplesPerFrame*2];
	
	void init() {
		memset(audioBuffer, 0, samplesPerFrame*2*2);
		OPL3_Reset(&chip, audioSampleRate);
	}
	
	void deinit() {
		
	}
	
	void afterFrame() {

	}
	
	int16_t* callback() {
		OPL3_GenerateStream(&chip, audioBuffer, samplesPerFrame);
		return &audioBuffer[0];
	}
	
	void writeReg(int reg, int value) {
		if(reg > numApuRegisters || value > INT8_MAX) {
			return;
		}
		OPL3_WriteRegBuffered(&chip,reg,value);
		std::cout<<"Audio register "<<std::hex<<reg<<" with "<<value<<std::dec<<std::endl;
	}
}