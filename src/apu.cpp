#include "apu.hpp"
#include "opl3.h"
#include <cstring>

#include "common.hpp"

namespace apu {
	opl3_chip chip;
	int16_t audioBuffer[samplesPerFrame*2];
	
	void init() {
		memset(audioBuffer, 0, samplesPerFrame*2*2);
		OPL3_Reset(&chip, audioSampleRate);
		int16_t x[26] = {0x105,	0x01, //	Enable OPL3 mode
			0x20,	0x01,	//Operator 1 (Modulator) Multiplier 1
			0x40,	0x00,	//Operator 1 Output Level
			0x60,	0xF0,	//Operator 1 Attack/Decay Rate
			0x80,	0xF0,	//Operator 1 Sustain/Release Rate
			0xE0,	0x00,	//Operator 1 Sine Wave
			0x23,	0x01,	//Operator 2 (Carrier) Multiplier 1
			0x43,	0x00,	//Operator 2 Output Level
			0x63,	0xF0,	//Operator 2 Attack/Decay Rate
			0x83,	0xF0,	//Operator 2 Sustain/Release Rate
			0xE3,	0x00,	//Operator 2 Sine Wave
		};
	
		for(int i = 0; i<26;i+=2){
			OPL3_WriteRegBuffered(&chip, x[i*2], x[i*2+1]);
		}
	}
	void deinit() {
		
	}
	
	void afterFrame() {
		if(frameNum%60 == 0) {
		OPL3_WriteRegBuffered(&chip, 0xA0,	0x40);
		OPL3_WriteRegBuffered(&chip, 0xB0,	0x31);
		} if(frameNum%60 == 30) {
			OPL3_WriteRegBuffered(&chip, 0xB0,	0x00);
		}
	}
	
	int16_t* callback() {
		OPL3_GenerateStream(&chip, audioBuffer, samplesPerFrame);
		return &audioBuffer[0];
	}
}