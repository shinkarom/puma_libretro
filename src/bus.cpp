#include "bus.hpp"
#include "common.hpp"

#include <fstream>
#include <iostream>

namespace bus {
	
	uint8_t memory[totalMemory];
	
	void write8(uint32_t address, uint8_t value) {
		if(address == 0xFFFFFF00) {
			syscall_callback(address, value);
			return;
		} else if(address>totalMemory)
			return;
		memory[address] = value;
	}
	
	void write16(uint32_t address, uint16_t value) {
		if(address == 0xFFFFFF00) {
			syscall_callback(address, value);
			return;
		} else if(address>totalMemory)
			return;
		memory[address] = (value>>8)&0xFF;
		memory[address+1] = (value)&0xFF;
	}
	
	void write32(uint32_t address, uint32_t value) {
		if(address == 0xFFFFFF00) {
			syscall_callback(address, value);
			return;
		} else if(address>totalMemory)
			return;
		memory[address] =  (value>>24)&0xFF;
		memory[address+1] =  (value>>16)&0xFF;
		memory[address+2] =  (value>>8)&0xFF;
		memory[address+3] =  (value)&0xFF;
	}
	
	uint8_t read8(uint32_t address) {
		if(address>totalMemory)
			return 0;
		return memory[address];
	}
	
	uint16_t read16(uint32_t address) {
		if(address>totalMemory)
			return 0;
		auto r = (read8(address)<<8)|(read8(address+1));
		return r;
	}
	
	uint32_t read32(uint32_t address) {
		if(address>totalMemory)
			return 0;
		auto r = (read8(address)<<24)|(read8(address+1)<<16)|(read8(address+2)<<8)|(read8(address+3));
		return r;
	}
	
	bool load(char* fileName) {
		std::ifstream file(fileName, std::ios::binary);
		if(!file.is_open()){
			return false;
		}
		file.seekg(0, std::ios::end);
		std::streampos fileSize = file.tellg();
		file.seekg(0, std::ios::beg);
		
		if(fileSize > totalMemory-codeOffset) {
			return false;
		}
		
		file.read((char*)(memory+codeOffset), fileSize);
		
		std::cout<<"Read file of size "<<fileSize<<std::endl;
		
		return true;
	}
}