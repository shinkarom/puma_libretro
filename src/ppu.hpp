#pragma once

#include "cstdint"

namespace ppu {
	void init();
	void deinit();
	void afterLoad();
	void beforeFrame();
	void afterFrame();
	
	uint32_t* getBuffer();
}