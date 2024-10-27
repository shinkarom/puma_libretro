#pragma once

#include "common.hpp"

namespace apu {
	void init();
	void deinit();
	void frame();
	int16_t* callback();
}