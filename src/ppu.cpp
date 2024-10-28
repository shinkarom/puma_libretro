#include "ppu.hpp"
#include "common.hpp"

#include <cstring>

namespace ppu {
	
	static uint32_t *frame_buf;
	
	void init() {
		 frame_buf = new uint32_t[screenTotalPixels];
		 memset(frame_buf,0,screenTotalPixels*sizeof(uint32_t));
	}
	
	void deinit() {
		delete[] frame_buf;
	}
	
	void frame() {
		
	}
	
	uint32_t* getBuffer() {
		return frame_buf;
	}
}