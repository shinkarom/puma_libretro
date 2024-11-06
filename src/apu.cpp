#include "apu.hpp"
#include <cstring>
#include <iostream>

#include "common.hpp"
#include "bus.hpp"
#include "Simple_Apu.h"

int16_t audioBuffer[samplesPerFrame*2];
Simple_Apu nesApu;
uint32_t dmcStartAddress;

int dmc_reader( void* user_data, cpu_addr_t addr){
	auto address = dmcStartAddress + (addr - fakeDMCStartAddress);
	auto r = bus::read8(address);
	return r;
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
			std::cout<<"callback "<<audioBuffer[i*2]<<" "<<audioBuffer[i*2+1]<<std::endl;
		}
		std::cout<<"---"<<std::endl;
		*/
		return audioBuffer;
	}
	
	void writeReg(int reg, int value) {
		/*
		if(reg > numApuRegisters) {
			return;
		}
		*/
		nesApu.write_register(reg, value);
		
		//std::cout<<"Audio register "<<std::hex<<reg<<" with "<<value<<std::dec<<std::endl;
	}
	
	void setDMCStart(uint32_t start, uint32_t end) {
		if(start >= totalMemory || end >= totalMemory) {
			return;
		}
		dmcStartAddress = start;
		auto len = (end - start - 1) / 16;
		nesApu.write_register(0x4012, 0x00);
		nesApu.write_register(0x4013, len);
	}
	
}