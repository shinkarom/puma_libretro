#include "ppu.hpp"
#include "common.hpp"

#include <cstring>
#include <iostream>
#include "bus.hpp"

namespace ppu {
	
	static uint32_t *frame_buf;
	
	uint32_t palette16bit[65536];
	
	uint32_t convert16to32color(uint16_t color) {
		uint8_t r, g, b;

		// Extract red, green, and blue components from the 555 color
		r = (color >> 10) & 0x1F;
		g = (color >> 5) & 0x1F;
		b = color & 0x1F;

		// Expand each component to 8 bits
		r = (r << 3) | (r >> 2);
		g = (g << 3) | (g >> 2);
		b = (b << 3) | (b >> 2);

		// Combine the expanded components into a 32-bit ARGB color
		return (0xFF << 24) | (r << 16) | (g << 8) | b;
	}
	
	void init() {
		screenWidth = maxScreenWidth;
		screenHeight = maxScreenHeight;
		screenTotalPixels = maxScreenTotalPixels;		
		
		 frame_buf = new uint32_t[maxScreenTotalPixels];
		 memset(frame_buf,0,maxScreenTotalPixels*sizeof(uint32_t));
		 
		 for(int i = 0; i < 65536; i++) {
			 palette16bit[i] = convert16to32color(i);
		 }
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
		
	}
	
	void setPixel(int x, int y, uint32_t color) {
		if(x > screenWidth || y > screenHeight) {
			return;
		}
		frame_buf[y*screenWidth+x] = color;
		//std::cout<<"Set pixel at "<<x<<" "<<y<<" with "<<color<<std::endl;
	}
	
	uint32_t getPixel(int x, int y) {
		if(x > screenWidth || y > screenHeight) {
			return 0;
		}
		return frame_buf[y*screenWidth+x];
	}
	
	uint32_t* getBuffer() {
		return frame_buf;
	}
}