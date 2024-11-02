#include "cpu.hpp"
#include "m68k.h"

#include "common.hpp"
#include <iostream>
#include <cstdint>

#include "bus.hpp"
#include "ppu.hpp"
#include "apu.hpp"
#include "input.hpp"

enum {
	API_printRegisters = 0,
	API_getDimensions,
	API_setPixel,
	API_getPixel,
	API_setDimensions,
	API_getFrameNumber,
	API_writeAudioRegister,
	API_isPressed,
	API_isJustPressed,
	API_isJustReleased,
	API_waitForVBlank,
	API_cls,
	API_drawSprite,
};

void printRegisters() {
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
}

void syscall_handler(int value) {
	//std::cout<<"Syscall "<<value<<" triggered."<<std::endl;
	switch(value) {
		case API_printRegisters: {
			printRegisters();
			break;
		}
		case API_getDimensions:
			bus::push16(screenWidth);
			bus::push16(screenHeight);
			break;
		case API_setPixel: {
			auto color = bus::pop16();
			auto y = bus::pop16();
			auto x = bus::pop16();
			ppu::setPixel(x, y, color);
			break; 
		}
		case API_getPixel: {
			auto y = bus::pop16();
			auto x = bus::pop16();
			auto color = ppu::getPixel(x, y);
			bus::push16(color);
			break;
		}
		case API_setDimensions: {
			auto y = bus::pop16();
			auto x = bus::pop16();
			ppu::queueDimensionsChange(x, y);
			break;
		}
		case API_getFrameNumber: {
			bus::push32(frameNum);
			break;
		}
		case API_writeAudioRegister: {
			auto value = bus::pop16();
			auto reg = bus::pop16();
			apu::writeReg(reg, value);
			break;
		}
		case API_isPressed: {
			auto keynum = bus::pop16();
			if(input::isPressed(keynum)) {
				bus::push16(1);
			} else {
				bus::push16(0);
			}
			break;
		}
		case API_isJustPressed: {
			auto keynum = bus::pop16();
			if(input::isJustPressed(keynum)) {
				bus::push16(1);
			} else {
				bus::push16(0);
			}
			break;
		}
		case API_isJustReleased: {
			auto keynum = bus::pop16();
			if(input::isJustReleased(keynum)) {
				bus::push16(1);
			} else {
				bus::push16(0);
			}
			break;
		}
		case API_waitForVBlank: {
			m68k_end_timeslice();
			break;
		}
		case API_cls: {
			auto color = bus::pop32();
			ppu::clear(color);
			break;
		}
		case API_drawSprite: {
			auto options = bus::pop32();
			auto h = bus::pop16();
			auto w = bus::pop16();
			auto y = bus::pop16();
			auto x = bus::pop16();
			auto address = bus::pop32();
			ppu::drawSprite(address, x, y, w, h, options);
			break;
		}
		default:
			break;
	}
}

unsigned int  m68k_read_memory_8(unsigned int address) {
	auto r = bus::read8(address);
	if(address == syscallStackAddress) {
		return bus::pop8();
	}
	//std::cout<<"Read 8bit from "<<address<<": "<<r<<std::endl;
	return r;
}

unsigned int  m68k_read_memory_16(unsigned int address) {
	auto r = bus::read16(address);
	if(address == syscallStackAddress) {
		return bus::pop16();
	}
	//std::cout<<"Read 16bit from "<<address<<": "<<r<<std::endl;
	return r;
}

unsigned int  m68k_read_memory_32(unsigned int address) {
	auto r = bus::read32(address);
	if(address == syscallStackAddress) {
		return bus::pop32();
	}
	//std::cout<<"Read 32bit from "<<address<<": "<<r<<std::endl;
	return r;
}

void m68k_write_memory_8(unsigned int address, unsigned int value) {
	bus::write8(address, value);
	if(address == syscallAddress) {
		syscall_handler(value);
	} else if(address == syscallStackAddress) {
		bus::push8(value);
	}
}

void m68k_write_memory_16(unsigned int address, unsigned int value) {
	bus::write16(address, value);
	if(address == syscallAddress) {
		syscall_handler(value);
	} else if(address == syscallStackAddress) {
		bus::push16(value);
	}
}

void m68k_write_memory_32(unsigned int address, unsigned int value) {
	bus::write32(address, value);
	if(address == syscallAddress) {
		syscall_handler(value);
	} else if(address == syscallStackAddress) {
		bus::push32(value);
	}
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