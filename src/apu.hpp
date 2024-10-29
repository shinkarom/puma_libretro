#pragma once

#include "common.hpp"

namespace apu {
	void init();
	void deinit();
	void afterFrame();
	int16_t* callback();
}