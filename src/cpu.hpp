#pragma once

#include <cstdint>

void push8(uint8_t value);
void push16(uint16_t value);
void push32(uint32_t value);
uint8_t peek8();
uint16_t peek16();
uint32_t peek32();
uint8_t pop8();
uint16_t pop16();
uint32_t pop32();
void silentPop8();
void silentPop16();
void silentPop32();

namespace cpu {
	void init();
	void deinit();
	void frame();
	void onLoad();
}