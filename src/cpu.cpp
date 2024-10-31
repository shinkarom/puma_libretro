#include "cpu.hpp"
#include "m68k.h"

#include "common.hpp"
#include <iostream>
#include <cstdint>

#include "bus.hpp"
#include "ppu.hpp"
#include "apu.hpp"
#include "input.hpp"

void syscall_handler(int value) {
	//std::cout<<"Syscall "<<value<<" triggered."<<std::endl;
	switch(value) {
		case API_printRegisters: {
			std::cout<<"---"<<std::hex<<std::endl;
			uint32_t t;
			t = m68k_get_reg(nullptr, M68K_REG_D0);
			std::cout<<t<<" ";
			t = m68k_get_reg(nullptr, M68K_REG_D1);
			std::cout<<t<<" ";
			t = m68k_get_reg(nullptr, M68K_REG_D2);
			std::cout<<t<<" ";
			t = m68k_get_reg(nullptr, M68K_REG_D3);
			std::cout<<t<<" ";
			t = m68k_get_reg(nullptr, M68K_REG_D4);
			std::cout<<t<<" ";
			t = m68k_get_reg(nullptr, M68K_REG_D5);
			std::cout<<t<<" ";
			t = m68k_get_reg(nullptr, M68K_REG_D6);
			std::cout<<t<<" ";
			t = m68k_get_reg(nullptr, M68K_REG_D7);
			std::cout<<t<<std::endl;
			t = m68k_get_reg(nullptr, M68K_REG_A0);
			std::cout<<t<<" ";
			t = m68k_get_reg(nullptr, M68K_REG_A1);
			std::cout<<t<<" ";
			t = m68k_get_reg(nullptr, M68K_REG_A2);
			std::cout<<t<<" ";
			t = m68k_get_reg(nullptr, M68K_REG_A3);
			std::cout<<t<<" ";
			t = m68k_get_reg(nullptr, M68K_REG_A4);
			std::cout<<t<<" ";
			t = m68k_get_reg(nullptr, M68K_REG_A5);
			std::cout<<t<<" ";
			t = m68k_get_reg(nullptr, M68K_REG_A6);
			std::cout<<t<<" ";
			t = m68k_get_reg(nullptr, M68K_REG_A7);
			std::cout<<t<<std::endl;
			std::cout<<std::dec<<"---"<<std::endl;
			break;
		}
		case API_getDimensions:
			push16(screenWidth);
			push16(screenHeight);
			break;
		case API_setPixel: {
			auto color = pop32();
			auto y = pop16();
			auto x = pop16();
			ppu::setPixel(x, y, color);
			break; 
		}
		case API_getPixel: {
			auto y = pop16();
			auto x = pop16();
			auto color = ppu::getPixel(x, y);
			push32(color);
			break;
		}
		case API_setDimensions: {
			auto y = pop16();
			auto x = pop16();
			ppu::queueDimensionsChange(x, y);
			break;
		}
		case API_getFrameNumber: {
			push32(frameNum);
			break;
		}
		case API_writeAudioRegister: {
			auto value = pop16();
			auto reg = pop16();
			apu::writeReg(reg, value);
			break;
		}
		case API_isPressed: {
			auto keynum = pop16();
			if(input::isPressed(keynum)) {
				push16(1);
			} else {
				push16(0);
			}
			break;
		}
		case API_isJustPressed: {
			auto keynum = pop16();
			if(input::isJustPressed(keynum)) {
				push16(1);
			} else {
				push16(0);
			}
			break;
		}
		case API_isJustReleased: {
			auto keynum = pop16();
			if(input::isJustReleased(keynum)) {
				push16(1);
			} else {
				push16(0);
			}
			break;
		}
		case API_waitForVBlank: {
			m68k_end_timeslice();
			break;
		}
		case API_cls: {
			auto color = pop32();
			ppu::clear(color);
			break;
		}
		default:
			break;
	}
}

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
		if(address == syscallAddress) {
			syscall_handler(value);
		}
}

void m68k_write_memory_16(unsigned int address, unsigned int value) {
	bus::write16(address, value);
	if(address == syscallAddress) {
		syscall_handler(value);
	}
}

void m68k_write_memory_32(unsigned int address, unsigned int value) {
	bus::write32(address, value);
	if(address == syscallAddress) {
		syscall_handler(value);
	}
}

void push8(uint8_t value) {
	auto sp = m68k_get_reg(nullptr, M68K_REG_SP);
	m68k_set_reg(M68K_REG_SP, sp-2);
	bus::write8(sp-1, value);
	//std::cout<<"push8: "<<(int)(value)<<std::endl;
}

void push16(uint16_t value) {
	auto sp = m68k_get_reg(nullptr, M68K_REG_SP);
	m68k_set_reg(M68K_REG_SP, sp-2);
	bus::write16(sp-2, value);
	//std::cout<<"push16: "<<value<<std::endl;
}

void push32(uint32_t value) {
	auto sp = m68k_get_reg(nullptr, M68K_REG_SP);
	m68k_set_reg(M68K_REG_SP, sp-4);
	bus::write32(sp-4, value);
	//std::cout<<"push32: "<<value<<std::endl;
}

uint8_t peek8() {
	auto sp = m68k_get_reg(nullptr, M68K_REG_SP);
	return bus::read8(sp+1);
}

uint16_t peek16() {
	auto sp = m68k_get_reg(nullptr, M68K_REG_SP);
	return bus::read16(sp);
}

uint32_t peek32() {
	auto sp = m68k_get_reg(nullptr, M68K_REG_SP);
	return bus::read32(sp);
}

uint8_t pop8() {
	auto sp = m68k_get_reg(nullptr, M68K_REG_SP);
	m68k_set_reg(M68K_REG_SP, sp+2);
	auto result = bus::read8(sp+1);
	//std::cout<<"pop8: "<<(int)(result)<<std::endl;
	return result;
}

uint16_t pop16() {
	auto sp = m68k_get_reg(nullptr, M68K_REG_SP);
	m68k_set_reg(M68K_REG_SP, sp+2);
	auto result = bus::read16(sp);
	//std::cout<<"pop16: "<<result<<std::endl;
	return result;
}

uint32_t pop32() {
	auto sp = m68k_get_reg(nullptr, M68K_REG_SP);
	m68k_set_reg(M68K_REG_SP, sp+4);
	auto result = bus::read32(sp);
	//std::cout<<"pop32: "<<result<<std::endl;
	return result;
}

void silentPop8() {
	auto sp = m68k_get_reg(nullptr, M68K_REG_SP);
	m68k_set_reg(M68K_REG_SP, sp+2);
}

void silentPop16() {
	auto sp = m68k_get_reg(nullptr, M68K_REG_SP);
	m68k_set_reg(M68K_REG_SP, sp+2);
}

void silentPop32() {
	auto sp = m68k_get_reg(nullptr, M68K_REG_SP);
	m68k_set_reg(M68K_REG_SP, sp+4);
}

namespace cpu {
	void init() {
		m68k_set_cpu_type(M68K_CPU_TYPE_68040);
		m68k_init();
	}
	
	void deinit() {
		
	}
	
	void frame() {
		auto x = m68k_execute(cyclesPerFrame);
		auto pc = m68k_get_reg(nullptr, M68K_REG_PC);
		//std::cout<<x<<" "<<pc<<" "<<frameNum<<std::endl;
	}
	
	void onLoad() {
		bus::write32(0, stackOffset);
		bus::write32(0x4, codeOffset);
		m68k_pulse_reset();
	}
}