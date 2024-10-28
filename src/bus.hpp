#pragma once

#include <cstdint>

namespace bus {
	void write8(uint32_t address, uint8_t value);
	void write16(uint32_t address, uint16_t value);
	void write32(uint32_t address, uint32_t value);
	uint8_t read8(uint32_t address);
	uint16_t read16(uint32_t address);
	uint32_t read32(uint32_t address);
	
	bool load(char* fileName);
}