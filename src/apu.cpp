#include "apu.hpp"
#include <cstring>
#include <iostream>

#include "common.hpp"

extern "C" {
	#include "cmixer.h"
}

namespace apu {
	int16_t audioBuffer[samplesPerFrame*2];
	int16_t waveform[32];
	int pos = 0;
	cm_Source* source;
	
	void cmEventHandler(cm_Event *e) {
		switch(e->type) {
			case CM_EVENT_SAMPLES: {
				for(int i = 0; i<e->length/2; i++) {
					e->buffer[i*2] = waveform[pos];
					e->buffer[i*2+1] = waveform[pos];
					pos = (pos+1)%32;
				}
			}
			case CM_EVENT_REWIND: {
				pos = 0;
			}
			default:
				break;
		}
	}
	
	cm_SourceInfo sourceInfo = {.handler = &cmEventHandler, .udata = nullptr, .samplerate = 14080, .length = 32};
	
	void init() {
		memset(audioBuffer, 0, samplesPerFrame*2*2);
		cm_init(audioSampleRate);
		for(int i = 0; i<32; i++) {
			waveform[i] = (i<16) ? -32767 : 32767;
		}
		pos = 0;
		source = cm_new_source(&sourceInfo);
		cm_set_master_gain(1);
		cm_set_loop(source, 1);
	}
	
	void deinit() {
		
	}
	
	void afterFrame() {
		
	}
	
	int16_t* callback() {
		cm_process(audioBuffer, samplesPerFrame*2);
		return &audioBuffer[0];
	}
	
	void writeReg(int reg, int value) {
		if(reg > numApuRegisters || value > INT8_MAX) {
			return;
		}
		switch(reg) {
			case 0: {
				if(value == 0) {
					cm_stop(source);
				} else {
					cm_play(source);
				}
				break;
			}
			default:
				break;
		}
		//std::cout<<"Audio register "<<std::hex<<reg<<" with "<<value<<std::dec<<std::endl;
	}
}