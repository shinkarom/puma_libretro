#include "apu.hpp"
#include <cstring>
#include <iostream>

#include "common.hpp"
#include "channel.hpp"

extern "C" {
	#include "cmixer.h"
}

int16_t audioBuffer[samplesPerFrame*2];
Channel chan;

namespace apu {
		
	void init() {
		memset(audioBuffer, 0, samplesPerFrame*2*2);
		cm_init(audioSampleRate);
		
		channel::init();
		chan.init();
	}
	
	void deinit() {
		
	}
	
	void afterFrame() {
		
	}
	
	int16_t* callback() {
		cm_process(audioBuffer, samplesPerFrame*2);
		return audioBuffer;
	}
	
	void writeReg(int reg, int value) {
		if(reg > numApuRegisters) {
			return;
		}
		switch(reg) {
			case 0: {
				if(value == 0) {
					chan.stop();
				} else {
					chan.play();
				}
				break;
			}
			default:
				break;
		}
		//std::cout<<"Audio register "<<std::hex<<reg<<" with "<<value<<std::dec<<std::endl;
	}
}