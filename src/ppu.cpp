#include "ppu.hpp"
#include "common.hpp"

#include <cstring>
#include <iostream>
#include "bus.hpp"

namespace ppu {
	
	static uint32_t *frame_buf;
	
	uint32_t palette16bit[65536];
	
	int pendingW, pendingH;
	
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
		pendingW = screenWidth;
		pendingH = screenHeight;
		
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

	}
	
	void beforeFrame() {
			
	}
	
	void afterFrame() {
		if(pendingW!=screenWidth || pendingH!=screenHeight) {
			wh_callback(pendingW, pendingH);
		}
	}
	
	void clear(uint32_t color) {
		for(int i = 0; i<screenTotalPixels; i++) {
			frame_buf[i] = color;
		}	
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
	
	void queueDimensionsChange(int w, int h) {
		if(w > maxScreenWidth) {
			w = maxScreenWidth;
		}
		if(h > maxScreenHeight) {
			h = maxScreenHeight;
		}
		pendingW = w;
		pendingH = h;
		//std::cout<<"Queued dimensions change to "<<w<<" "<<h<<std::endl;
	}
	
	uint32_t* getBuffer() {
		return frame_buf;
	}
}