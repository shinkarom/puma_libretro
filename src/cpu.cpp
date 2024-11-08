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
	API_setDimensions,
	API_writeAudioRegister,
	API_isPressed,
	API_isJustPressed,
	API_isJustReleased,
	API_waitForVBlank,
	API_set1bitPalette,
	API_set2bitPalette,
	API_cls,
	API_setPixel,
	API_getPixel,
	API_drawSprite,
	API_drawLine,
	API_drawCircle,
	API_drawCircleOutline,
	API_drawEllipse,
	API_drawEllipseOutline,
	API_drawRectangle, 
	API_drawRectangleOutline,
	API_drawTriangle,
	API_drawTriangleOutline,
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
		case API_getDimensions:{
			auto mode = (screenWidth-16) + ((screenHeight-16)/16);
			bus::push16(mode);
			break;
		}
		case API_setPixel: {
			auto color = bus::pop32();
			auto y = bus::pop16();
			auto x = bus::pop16();
			ppu::setPixel(x, y, color);
			break; 
		}
		case API_getPixel: {
			auto y = bus::pop16();
			auto x = bus::pop16();
			auto color = ppu::getPixel(x, y);
			bus::push32(color);
			break;
		}
		case API_setDimensions: {
			auto mode = bus::pop16();
			auto w = ((mode&0xF0)+16);
			auto h = ((mode&0x0F)+1)*16;
			ppu::queueDimensionsChange(w, h);
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
			auto options = bus::pop16();
			auto h = bus::pop16();
			auto w = bus::pop16();
			auto y = bus::pop16();
			auto x = bus::pop16();
			auto address = bus::pop32();
			ppu::drawSprite(address, x, y, w, h, options);
			break;
		}
		case API_set1bitPalette: {
			auto color2 = bus::pop32();
			auto color1 = bus::pop32();
			ppu::set1bitPalette(color1, color2);
			break;
		}
		case API_set2bitPalette: {
			auto color4 = bus::pop32();
			auto color3 = bus::pop32();
			auto color2 = bus::pop32();
			auto color1 = bus::pop32();
			ppu::set2bitPalette(color1, color2, color3, color4);
			break;
		}
		case API_drawLine: {
			auto color = bus::pop32();
			auto y2 = bus::pop16();
			auto x2 = bus::pop16();
			auto y1 = bus::pop16();
			auto x1 = bus::pop16();
			ppu::drawLine(x1, y1, x2, y2, color);
		}
		case API_drawCircle: {
			auto color = bus::pop32();
			auto radius = bus::pop16();
			auto y0 = bus::pop16();
			auto x0 = bus::pop16();
			ppu::drawCircleFilled(x0, y0, radius, color);
			break;
		}
		case API_drawCircleOutline: {
			auto color = bus::pop32();
			auto radius = bus::pop16();
			auto y0 = bus::pop16();
			auto x0 = bus::pop16();
			ppu::drawCircleOutline(x0, y0, radius, color);
			break;
		}
		case API_drawEllipse: {
			auto color = bus::pop32();
			auto b = bus::pop16();
			auto a = bus::pop16();
			auto y0 = bus::pop16();
			auto x0 = bus::pop16();
			ppu::drawEllipseFilled(x0, y0, a, b, color);
			break;
		}
		case API_drawEllipseOutline: {
			auto color = bus::pop32();
			auto b = bus::pop16();
			auto a = bus::pop16();
			auto y0 = bus::pop16();
			auto x0 = bus::pop16();
			ppu::drawEllipseOutline(x0, y0, a, b, color);
			break;
		}
		case API_drawRectangle: {
			auto color = bus::pop32();
			auto y2 = bus::pop16();
			auto x2 = bus::pop16();
			auto y1 = bus::pop16();
			auto x1 = bus::pop16();
			ppu::drawRectangleFilled(x1, y1, x2, y2, color);
			break;
		}
		case API_drawRectangleOutline: {
			auto color = bus::pop32();
			auto y2 = bus::pop16();
			auto x2 = bus::pop16();
			auto y1 = bus::pop16();
			auto x1 = bus::pop16();
			ppu::drawRectangleOutline(x1, y1, x2, y2, color);
			break;
		}
		case API_drawTriangle: {
			auto color = bus::pop32();
			auto y3 = bus::pop16();
			auto x3 = bus::pop16();
			auto y2 = bus::pop16();
			auto x2 = bus::pop16();
			auto y1 = bus::pop16();
			auto x1 = bus::pop16();
			ppu::drawTriangleFilled(x1, y1, x2, y2, x3, y3, color);
			break;
		}
		case API_drawTriangleOutline: {
			auto color = bus::pop32();
			auto y3 = bus::pop16();
			auto x3 = bus::pop16();
			auto y2 = bus::pop16();
			auto x2 = bus::pop16();
			auto y1 = bus::pop16();
			auto x1 = bus::pop16();
			ppu::drawTriangleOutline(x1, y1, x2, y2, x3, y3, color);
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
		m68k_set_irq(2);
		//auto pc = m68k_get_reg(nullptr, M68K_REG_PC);
		//std::cout<<x<<" "<<pc<<" "<<frameNum<<std::endl;
	}
	
	void onLoad() {
		bus::write32(0, stackOffset);
		bus::write32(0x4, codeOffset);
		m68k_pulse_reset();
	}
}