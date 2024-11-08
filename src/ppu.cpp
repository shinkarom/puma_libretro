#include "ppu.hpp"
#include "common.hpp"

#include <cstring>
#include <cmath>
#include <iostream>
#include "bus.hpp"
#include "color.hpp"

constexpr auto HFLIP_MASK = 1<<0;
constexpr auto VFLIP_MASK = 1<<1;

namespace ppu {
	
	static uint32_t *frame_buf;
	
	int pendingW, pendingH;
	uint32_t palette1bit[2] = {0x00000000, 0xFFFFFFFF};
	uint32_t palette2bit[4] = {0x00000000, 0x00000000, 0xFF888888, 0xFFFFFFFF};
	
	int bitOffset;
	
	void setFullPixel(int x, int y, uint32_t color) {
		if(x < 0 || x > screenWidth || y<0 || y > screenHeight || (color&0xFF000000!=0xFF000000)) {
			return;
		}
		auto dstColor = frame_buf[y*screenWidth+x];
		frame_buf[y*screenWidth+x] = color::blendARGB(color, dstColor);
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
		setFullPixel(x, y, color);
		//std::cout<<"Set pixel at "<<x<<" "<<y<<" with "<<color<<std::endl;
	}
	
	uint16_t getPixel(int x, int y) {
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
					uint8_t byte_data = bus::read8(pxa);
					uint8_t pixel_data = (byte_data >> (4 * (1 - bitOffset))) & 0x0F;
					color = color::palette4bit[pixel_data];
					bitOffset = 1 - bitOffset; // Toggle between 0 and 1
					if (bitOffset == 0) pxa += 1; // Move to next byte after 2 pixels
					break;
				}
				case 4: {
					uint8_t byte_data = bus::read8(pxa);
					uint8_t pixel_data = (byte_data >> (2 * (3 - bitOffset))) & 0x03;
					color = palette2bit[pixel_data];
					bitOffset = (bitOffset + 1) % 4; // Move to the next 2-bit position
					if (bitOffset == 0) pxa += 1; // Move to next byte after 4 pixels
					break;
				}
				case 5: { 
					uint8_t byte_data = bus::read8(pxa);
					uint8_t pixel_data = (byte_data >> (7 - bitOffset)) & 0x01;
					color = palette1bit[pixel_data];
					bitOffset = (bitOffset + 1) % 8; // Move to the next 1-bit position
					if (bitOffset == 0) pxa += 1; // Move to next byte after 8 pixels
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
	
	void set1bitPalette(uint32_t color1, uint32_t color2) {
		palette1bit[0] = color1;
		palette1bit[1] = color2;
	}
	
	void set2bitPalette(uint32_t color1, uint32_t color2, uint32_t color3, uint32_t color4) {
		palette2bit[0] = color1;
		palette2bit[1] = color2;
		palette2bit[2] = color3;
		palette2bit[3] = color4;
	}
	
	void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color) {
		int dx = std::abs(x2 - x1);
		int dy = std::abs(y2 - y1);
		int sx = (x1 < x2) ? 1 : -1;
		int sy = (y1 < y2) ? 1 : -1;
		int err = dx - dy;

		while (true) {
			setFullPixel(x1, y1, color);  // Draw the pixel

			// If the start point reaches the end point, stop drawing
			if (x1 == x2 && y1 == y2) break;

			int e2 = 2 * err;
			
			if (e2 > -dy) {
				err -= dy;
				x1 += sx;
			}
			
			if (e2 < dx) {
				err += dx;
				y1 += sy;
			}
		}
		//std::cout<<"Drew line from "<<x1<<" "<<y1<<" to "<<x2<<" "<<y2<<" with "<<std::hex<<color<<std::dec<<std::endl;
	}
	
	void drawCircleOutline(uint16_t x0, uint16_t y0, uint16_t radius, uint32_t color) {
		int x = radius;
		int y = 0;
		int err = 0;

		while (x >= y) {
			setFullPixel(x0 - x, y0 - y, color);
			setFullPixel(x0 + x, y0 - y, color);
			setFullPixel(x0 - x, y0 + y, color);
			setFullPixel(x0 + x, y0 + y, color);
			setFullPixel(x0 - y, y0 - x, color);
			setFullPixel(x0 + y, y0 - x, color);
			setFullPixel(x0 - y, y0 + x, color);
			setFullPixel(x0 + y, y0 + x, color);

			y += 1;
			if (err <= 0) {
				err += 2*y + 1;
			} else {
				x -= 1;
				//err -= 2*x + 1;
				err += 2*(y-x) + 1;
			}
		}
	}

	void drawCircleFilled(uint16_t x0, uint16_t y0, uint16_t radius, uint32_t color) {
		int x = radius;
		int y = 0;
		int err = 0;

		while (x >= y) {
			for (int i = x0 - x; i <= x0 + x; i++) {
				setFullPixel(i, y0 - y, color);
				setFullPixel(i, y0 + y, color);
			}
			for (int i = x0 - y; i <= x0 + y; i++) {
				setFullPixel(i, y0 - x, color);
				setFullPixel(i, y0 + x, color);
			}

			y++;
			if (err <= 0) {
				err += 2 * y + 1;
			} else {
				x--;
				err += 2 * (y - x) + 1;
			}
		}
	}

	void drawEllipseOutline(uint16_t x0, uint16_t y0, uint16_t a, uint16_t b, uint32_t color) {
		if(a == b){
			drawCircleOutline(x0, y0, a, color);
			return;
		}
		
		int x = a;
		int y = 0;
		int dx = (1 - 2 * a) * b * b;
		int dy = a * a;
		int err = dx + dy;

		while (x >= 0) {
			setFullPixel(x0 + x, y0 - y, color);
			setFullPixel(x0 + x, y0 + y, color);
			setFullPixel(x0 - x, y0 - y, color);
			setFullPixel(x0 - x, y0 + y, color);

			int e2 = 2 * err;
			if (e2 >= dx) {
				x--;
				err += dx += 2 * b * b;
			}
			if (e2 <= dy) {
				y++;
				err += dy += 2 * a * a;
			}
		}
	}

	void drawEllipseFilled(uint16_t x0, uint16_t y0, uint16_t a, uint16_t b, uint32_t color) {
		if(a == b){
			drawCircleFilled(x0, y0, a, color);
			return;
		}
		
		int x = a;
		int y = 0;
		int dx = (1 - 2 * a) * b * b;
		int dy = a * a;
		int err = dx + dy;

		while (x >= 0) {
			for(int i = x0-x; i<=x0+x; i++) {
				setFullPixel(i, y0 - y, color);
				setFullPixel(i, y0 + y, color);
			}

			int e2 = 2 * err;
			if (e2 >= dx) {
				x--;
				err += dx += 2 * b * b;
			}
			if (e2 <= dy) {
				y++;
				err += dy += 2 * a * a;
			}
		}
	}


	void drawRectangleOutline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color) {
		for (uint16_t x = x1; x <= x2; x++) {
			setFullPixel(x, y1, color);
			setFullPixel(x, y2, color);
		}
		for (uint16_t y = y1; y <= y2; y++) {
			setFullPixel(x1, y, color);
			setFullPixel(x2, y, color);
		}
	}

	void drawRectangleFilled(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color) {
		for (uint16_t y = y1; y <= y2; y++) {
			for (uint16_t x = x1; x <= x2; x++) {
				setFullPixel(x, y, color);
			}
		}
	}

	void drawTriangleOutline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint32_t color) {
		drawLine(x1, y1, x2, y2, color);
		drawLine(x2, y2, x3, y3, color);
		drawLine(x3, y3, x1, y1, color);
	}

	void drawTriangleFilled(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint32_t color) {
		auto swap = [](uint16_t &a, uint16_t &b) { uint16_t temp = a; a = b; b = temp; };
		
		// Sort the vertices by y-coordinate
		if (y1 > y2) { swap(x1, x2); swap(y1, y2); }
		if (y1 > y3) { swap(x1, x3); swap(y1, y3); }
		if (y2 > y3) { swap(x2, x3); swap(y2, y3); }

		auto interpolate = [](int y, int x1, int y1, int x2, int y2) -> int {
			if (y1 == y2) return x1;
			return x1 + (y - y1) * (x2 - x1) / (y2 - y1);
		};

		for (uint16_t y = y1; y <= y3; y++) {
			uint16_t xStart = (y < y2) ? interpolate(y, x1, y1, x2, y2) : interpolate(y, x2, y2, x3, y3);
			uint16_t xEnd = interpolate(y, x1, y1, x3, y3);

			if (xStart > xEnd) swap(xStart, xEnd);

			for (uint16_t x = xStart; x <= xEnd; x++) {
				setFullPixel(x, y, color);
			}
		}
	}

	void drawText(uint32_t fontOrigin, int16_t fontWidth, int16_t fontHeight, uint32_t textOrigin, uint16_t x, uint16_t y, uint32_t color) {
		uint32_t oldPalette[2];
		oldPalette[0] = palette1bit[0];
		oldPalette[1] = palette1bit[1];
		palette1bit[0] = transparentColor;
		palette1bit[1] = color;
		auto letterOffset = textOrigin;
		auto xOffset = x;
		do {
			char letter = bus::read8(letterOffset);
			if(letter == 0) {
				break;
			}
			auto letterAddress = fontOrigin + (letter-32)*8;
			if(letter<32 || letter>127) {
				
			} else {
				drawSprite(letterAddress, xOffset, y, fontWidth, fontHeight, 0x0500);
			}
			letterOffset++;
			xOffset+=fontWidth;
			if(xOffset>=screenWidth) {
				break;
			}
		} while(true);
		palette1bit[0] = oldPalette[0];
		palette1bit[1] = oldPalette[1];
	}
	
}