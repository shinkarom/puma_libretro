#pragma once

#include <cstdint>

namespace ppu {
	
	void init();
	void deinit();
	void afterLoad();
	void beforeFrame();
	void afterFrame();
	
	void setPixel(int x, int y, uint16_t color);
	uint16_t getPixel(int x, int y);
	void queueDimensionsChange(int w, int h);
	void clear(uint32_t color);
	void drawSprite(uint32_t address, int x, int y, int w, int h, uint16_t options);
	void set1bitPalette(uint32_t color1, uint32_t color2);
	void set2bitPalette(uint32_t color1, uint32_t color2, uint32_t color3, uint32_t color4);
	void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color);
	
	uint32_t* getBuffer();
}