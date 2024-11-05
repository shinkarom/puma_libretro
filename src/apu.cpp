#include "apu.hpp"
#include <cstring>
#include <iostream>

#include "common.hpp"

int16_t audioBuffer[samplesPerFrame*2];
Simple_Apu nesApu;

int dmc_reader( void* user_data, cpu_addr_t ){
	return 0;
}

namespace apu {
		
	void init() {
		memset(audioBuffer, 0, samplesPerFrame*2*2);
		
		nesApu.sample_rate(audioSampleRate);
		nesApu.dmc_reader(dmc_reader,nullptr);
		
	}
	
	void deinit() {
		
	}
	
	void afterFrame() {
		
	}
	
	int16_t* callback() {
		
		nesApu.end_frame();
		int16_t sample;
		for(int i = 0; i<samplesPerFrame; i++) {
			nesApu.read_samples(&sample,1);
			audioBuffer[i*2] = sample;
			audioBuffer[i*2+1] = sample;
		}
		
		//std::cout<<std::hex<<nesApu.read_status()<<std::dec<<std::endl;
		/*
		for(int i = 0; i<samplesPerFrame;i++){
			std::cout<<audioBuffer[i*2]<<std::endl;
		}
		std::cout<<"---"<<std::endl;
		*/
		return audioBuffer;
	}
	
	void writeReg(int reg, int value) {
		if(reg > numApuRegisters) {
			return;
		}
		int val = value & 0xFF;
		int r = reg + 0x4000;
		if(reg == 14 || reg == 16) {
			return;
		}
		nesApu.write_register(r, val);
		//std::cout<<"Audio register "<<std::hex<<r<<" with "<<val<<std::dec<<std::endl;
	}
	
}