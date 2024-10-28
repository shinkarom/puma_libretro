#pragma once

#include "cstdint"

namespace ppu {
	void init();
	void deinit();
	void frame();
	
	uint32_t* getBuffer();
}