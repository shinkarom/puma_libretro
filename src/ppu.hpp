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
	void clear(uint32_t color);
	void drawSprite(uint32_t address, int x, int y, int w, int h, uint16_t options);
	void set1bitPalette(uint32_t color1, uint32_t color2);
	void set2bitPalette(uint32_t color1, uint32_t color2, uint32_t color3, uint32_t color4);
	void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color);
	void drawCircleOutline(uint16_t x0, uint16_t y0, uint16_t radius, uint32_t color);
	void drawCircleFilled(uint16_t x0, uint16_t y0, uint16_t radius, uint32_t color);
	void drawEllipseOutline(uint16_t x0, uint16_t y0, uint16_t a, uint16_t b, uint32_t color);
	void drawEllipseFilled(uint16_t x0, uint16_t y0, uint16_t a, uint16_t b, uint32_t color);
	void drawRectangleOutline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color);
	void drawRectangleFilled(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color);
	void drawTriangleOutline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint32_t color);
	void drawTriangleFilled(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint32_t color);
	void drawText(uint32_t fontOrigin, int16_t fontWidth, int16_t fontHeight, uint32_t textOrigin, uint16_t x, uint16_t y, uint32_t color);
	
	uint32_t* getBuffer();
}