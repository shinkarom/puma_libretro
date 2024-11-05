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
		chan.init(0, 32, 14080);
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
	
	void initChannel(int chanNum, uint32_t start, uint32_t end, int smplRate) {
		//std::cout<<"Will do channel init with "<<start<<" "<<end<<" "<<smplRate<<std::endl;
		chan.init(start, end, smplRate);
	}
}