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
		uint8_t r, g, b, a;

		// Extract alpha, red, green, and blue components from the 16-bit color
		a = (color & 0x8000) ? 0xFF : 0x00; // Check the 16th bit for alpha
		r = (color >> 10) & 0x1F;
		g = (color >> 5) & 0x1F;
		b = color & 0x1F;

		// Expand each component to 8 bits
		r = (r << 3) | (r >> 2);
		g = (g << 3) | (g >> 2);
		b = (b << 3) | (b >> 2);

		// Combine the expanded components into a 32-bit ARGB color
		return (a << 24) | (r << 16) | (g << 8) | b;
	}
	
	uint16_t convert32to16color(uint32_t color) {
		uint8_t r, g, b, a;

		// Extract alpha, red, green, and blue components from the 32-bit ARGB color
		a = (color >> 24) & 0xFF;
		r = (color >> 16) & 0xFF;
		g = (color >> 8) & 0xFF;
		b = color & 0xFF;

		// Compress each component to 5 bits
		r = (r >> 3) & 0x1F;
		g = (g >> 3) & 0x1F;
		b = (b >> 3) & 0x1F;

		// Set the 16th bit based on the alpha channel
		uint16_t result = (r << 10) | (g << 5) | b;
		if (a > 0) {
			result |= 0x8000; // Set the 16th bit
		}

		return result;
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
	
	void setPixel(int x, int y, uint16_t color) {
		if(x > screenWidth || y > screenHeight) {
			return;
		}
		frame_buf[y*screenWidth+x] = palette16bit[color];
		//std::cout<<"Set pixel at "<<x<<" "<<y<<" with "<<color<<std::endl;
	}
	
	uint16_t getPixel(int x, int y) {
		if(x > screenWidth || y > screenHeight) {
			return 0;
		}
		return convert32to16color(frame_buf[y*screenWidth+x]);
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
	
	void drawSprite(uint32_t address, int x, int y, int w, int h, uint32_t options) {
		//std::cout<<std::hex<<"Will draw sprite from "<<address<<std::dec<<std::endl;
		auto x_start = x;
		auto x_delta = 1;
		auto x_end = x+w;
		auto y_start = y;
		auto y_delta = 1;
		auto y_end = y+h;
		
		auto pxa = address;
		auto xx = x_start;
		auto yy = y_start;
		for(int _i = 0; _i < w * h; _i++) {
			auto color = palette16bit[bus::read16(pxa)];
			pxa += 2;
			auto coord = yy*screenWidth+xx;
			if(color & 0xFF000000 || xx >= screenWidth || yy >= screenHeight) {
				
			} else {
				frame_buf[coord] = color;	
			}
			xx+=x_delta;
			if(xx >= x_end) {
				xx = x_start;
				yy+=y_delta;
			}
		}
		//std::cout<<"---"<<std::endl;
	}
	
	uint32_t* getBuffer() {
		return frame_buf;
	}
}