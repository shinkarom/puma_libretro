#include "cpu.hpp"
#include "m68k.h"

#include "common.hpp"
#include <iostream>
#include <cstdint>

uint8_t memory[totalMemory];

unsigned int  m68k_read_memory_8(unsigned int address) {
	if(address<totalMemory) {
		return memory[address];
	} else {
		return 0;
	}
}

unsigned int  m68k_read_memory_16(unsigned int address) {
	return 0;
}

unsigned int  m68k_read_memory_32(unsigned int address) {
	return 0;
}

void m68k_write_memory_8(unsigned int address, unsigned int value) {
	
}

void m68k_write_memory_16(unsigned int address, unsigned int value) {
	
}

void m68k_write_memory_32(unsigned int address, unsigned int value) {
	
}

namespace cpu {
	void init() {
		m68k_set_cpu_type(M68K_CPU_TYPE_68040);
		m68k_init();
		m68k_pulse_reset();
	}
	
	void deinit() {
		
	}
	
	void frame() {
		auto x = m68k_execute(cyclesPerFrame);
		std::cout<<x<<" "<<cyclesPerFrame<<std::endl;
	}
}