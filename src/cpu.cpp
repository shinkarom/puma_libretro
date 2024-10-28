#include "cpu.hpp"
#include "m68k.h"

#include "common.hpp"
#include <iostream>
#include <cstdint>

#include "bus.hpp"

int nextCycles;

unsigned int  m68k_read_memory_8(unsigned int address) {
	auto r = bus::read8(address);
	//std::cout<<"Read 8bit from "<<address<<": "<<r<<std::endl;
	return r;
}

unsigned int  m68k_read_memory_16(unsigned int address) {
	auto r = bus::read16(address);
	//std::cout<<"Read 16bit from "<<address<<": "<<r<<std::endl;
	return r;
}

unsigned int  m68k_read_memory_32(unsigned int address) {
	auto r = bus::read32(address);
	//std::cout<<"Read 32bit from "<<address<<": "<<r<<std::endl;
	return r;
}

void m68k_write_memory_8(unsigned int address, unsigned int value) {
	bus::write8(address, value);
}

void m68k_write_memory_16(unsigned int address, unsigned int value) {
	bus::write16(address, value);
}

void m68k_write_memory_32(unsigned int address, unsigned int value) {
	bus::write32(address, value);
}

namespace cpu {
	void init() {
		m68k_set_cpu_type(M68K_CPU_TYPE_68040);
		m68k_init();
	}
	
	void deinit() {
		
	}
	
	void frame() {
		auto x = m68k_execute(nextCycles);
		nextCycles = cyclesPerFrame+(cyclesPerFrame - x);
		auto pc = m68k_get_reg(nullptr, M68K_REG_PC);
		//std::cout<<x<<" "<<(cyclesPerFrame - x)<<" "<<pc<<std::endl;
	}
	
	void onLoad() {
		bus::write32(0, 0x400);
		bus::write32(0x4, codeOffset);
		m68k_pulse_reset();
		nextCycles = cyclesPerFrame;
	}
}