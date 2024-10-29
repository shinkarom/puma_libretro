#include "cpu.hpp"
#include "m68k.h"

#include "common.hpp"
#include <iostream>
#include <cstdint>

#include "bus.hpp"

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

void syscall_handler(int opcode) {
	std::cout<<"Syscall "<<opcode<<" triggered"<<std::endl;
}

namespace cpu {
	void init() {
		m68k_set_cpu_type(M68K_CPU_TYPE_68040);
		m68k_init();
		syscall_callback = &syscall_handler;
	}
	
	void deinit() {
		
	}
	
	void frame() {
		bus::write32(0x40200, frameNum);
		
		auto x = m68k_execute(cyclesPerFrame);
		auto pc = m68k_get_reg(nullptr, M68K_REG_PC);
		std::cout<<x<<" "<<pc<<" "<<frameNum<<std::endl;
	}
	
	void onLoad() {
		bus::write32(0, 0x400);
		bus::write32(0x4, codeOffset);
		m68k_pulse_reset();
	}
}