#include "cpu.hpp"
#include "m68k.h"

unsigned int  m68k_read_memory_8(unsigned int address) {
	return 0;
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
		m68k_set_cpu_type(M68K_CPU_TYPE_68000);
		m68k_init();
		m68k_pulse_reset();
	}
	
	void deinit() {
		
	}
	
	void frame() {
		
	}
}