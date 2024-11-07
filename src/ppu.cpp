#include "ppu.hpp"
#include "common.hpp"

#include <cstring>
#include <iostream>
#include "bus.hpp"
#include "color.hpp"

constexpr auto HFLIP_MASK = 1<<0;
constexpr auto VFLIP_MASK = 1<<1;

namespace ppu {
	
	static uint32_t *frame_buf;
	
	int pendingW, pendingH;
	int bitOffset;
	
	void setFullPixel(int x, int y, uint32_t color) {
		if(x < 0 || x > screenWidth || y<0 || y > screenHeight || (color&0xFF000000!=0xFF000000)) {
			return;
		}
		frame_buf[y*screenWidth+x] = color;
		//std::cout<<"Set pixel at "<<x<<" "<<y<<" with "<<std::hex<<color<<std::dec<<std::endl;
	}
	
	void init() {
		screenWidth = maxScreenWidth;
		screenHeight = maxScreenHeight;
		screenTotalPixels = maxScreenTotalPixels;
		pendingW = screenWidth;
		pendingH = screenHeight;
		
		 frame_buf = new uint32_t[maxScreenTotalPixels];
		 memset(frame_buf,0,maxScreenTotalPixels*sizeof(uint32_t));
		 
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
		setFullPixel(x, y, color::palette16bit[color]);
		//std::cout<<"Set pixel at "<<x<<" "<<y<<" with "<<color<<std::endl;
	}
	
	uint16_t getPixel(int x, int y) {
		if(x > screenWidth || y > screenHeight) {
			return 0;
		}
		return color::convert32to16color(frame_buf[y*screenWidth+x]);
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
	
	void drawSprite(uint32_t address, int x, int y, int w, int h, uint16_t options) {
		//std::cout<<std::hex<<"Will draw sprite from "<<address<<std::dec<<std::endl;
		bitOffset = 0;
		auto x_start = x;
		auto x_delta = 1;
		auto x_end = x+w;
		auto y_start = y;
		auto y_delta = 1;
		auto y_end = y+h;
		//std::cout<<x_start<<" "<<x_delta<<" "<<x_end<<" "<<y_start<<" "<<y_delta<<" "<<y_end<<" "<<std::endl;
		
		if(options & HFLIP_MASK) {
			//std::cout<<"HFLIP ";
			
			auto z = x_start;
			x_start = x_end-1;
			x_end = z;
			x_delta = -1;
		}
		if(options & VFLIP_MASK) {
			//std::cout<<"VFLIP ";
			
			auto z = y_start;
			y_start = y_end-1;
			y_end = z;
			y_delta = -1;
		}
		
		//std::cout<<std::endl;

		//std::cout<<x_start<<" "<<x_delta<<" "<<x_end<<" "<<y_start<<" "<<y_delta<<" "<<y_end<<" "<<std::endl;
		auto pxa = address;
		auto xx = x_start;
		auto yy = y_start;
		uint32_t color;
		for(int _i = 0; _i < w * h; _i++) {
			switch((options&0xFF00)>>8) {
				case 1: {
					color = color::palette16bit[bus::read16(pxa)];
					pxa += 2;
					break;
				}
				case 2: {
					auto c = bus::read8(pxa);
					color = color::palette8bit[c];
					pxa += 1;
					break;
				}
				case 3: {
					auto c = bus::read8(pxa);
					uint8_t pixel_data = (c >> (4 * (1 - bitOffset))) & 0x0F;
					color = color::palette4bit[pixel_data];
					bitOffset++;
					if(bitOffset == 2){
						bitOffset = 0;
						pxa += 1;
					}
					break;
				}
				default: {
					color = bus::read32(pxa);
					pxa += 4;
					break;
				}
			}
			
			//std::cout<<xx<<" "<<yy<<" "<<std::hex<<color<<std::dec<<std::endl;
			setFullPixel(xx, yy, color);
			xx+=x_delta;
			bool lineEnd = (x_delta>0) ? (xx >= x_end) : (xx < x_end);
			if(lineEnd) {
				xx = x_start;
				yy += y_delta;
			}
			
		}
		//std::cout<<"---"<<std::endl;
	}
	
	uint32_t* getBuffer() {
		return frame_buf;
	}
}