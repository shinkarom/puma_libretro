#include "ppu.hpp"
#include "common.hpp"

#include <cstring>
#include "bus.hpp"

namespace ppu {
	
	static uint32_t *frame_buf;
	
	void init() {
		screenWidth = maxScreenWidth;
		screenHeight = maxScreenHeight;
		screenTotalPixels = maxScreenTotalPixels;		
		
		 frame_buf = new uint32_t[maxScreenTotalPixels];
		 memset(frame_buf,0,maxScreenTotalPixels*sizeof(uint32_t));
	}
	
	void deinit() {
		delete[] frame_buf;
	}
	
	void afterLoad() {
		bus::write16(0x40204, screenWidth);
		bus::write16(0x40206, screenHeight);
	}
	
	void beforeFrame() {
		auto w = bus::read16(0x40204);
		auto h = bus::read16(0x40206);
		
		if(w!=screenWidth || h!=screenHeight) {
			if(w > maxScreenWidth) {
				bus::write16(0x40204, maxScreenWidth);
				w = maxScreenWidth;
			}
			if(h > maxScreenHeight) {
				bus::write16(0x40206, maxScreenHeight);
				h = maxScreenHeight;
			}
			wh_callback(w, h);
			//TODO: chande screen dimensions
		}
	}
	
	void afterFrame() {
		int i = 0;
		int offset = framebufferOffset;
		auto px = frame_buf;
		do {
			*px = bus::read32(offset);
			i++;
			offset+=4;
			px++;
		} while(i<screenTotalPixels);
			
		
	}
	
	uint32_t* getBuffer() {
		return frame_buf;
	}
}