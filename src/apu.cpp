#include "apu.hpp"
#include "opl3.h"
#include <cstring>
#include <iostream>

#include "common.hpp"

namespace apu {
	opl3_chip chip;
	int16_t audioBuffer[samplesPerFrame*2];
	
	void regCallback(int reg, int value) {
		OPL3_WriteRegBuffered(&chip,reg,value);
		std::cout<<"Audio register "<<std::hex<<reg<<" with "<<value<<std::endl;
	}
	
	void init() {
		memset(audioBuffer, 0, samplesPerFrame*2*2);
		OPL3_Reset(&chip, audioSampleRate);
		audioRegistersCallback = &regCallback;
	}
	
	void deinit() {
		
	}
	
	void afterFrame() {

	}
	
	int16_t* callback() {
		OPL3_GenerateStream(&chip, audioBuffer, samplesPerFrame);
		return &audioBuffer[0];
	}
}