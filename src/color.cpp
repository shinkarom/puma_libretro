#include "color.hpp"

#include <iostream>

namespace color {
	
	uint32_t palette16bit[65536];
	uint32_t palette8bit[256];
	uint32_t palette4bit[64];
	
	constexpr auto transparentColor = 0xFF000000;
	 
	void init() {
		for(int i = 0; i < 65536; i++) {
			 palette16bit[i] = convert16to32color(i);
		 }
		 for(int i = 0; i < 256; i++) {
			 palette8bit[i] = convert8to32color(i);
		 }
		 for(int i = 0; i < 16; i++) {
			 palette4bit[i] = convert4to32color(i);
		 }
	}
	
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

	// Convert ARGB8888 to 8-bit color (1-bit transparency, 1 unused bit, RGB222)
	uint8_t convert32to8color(uint32_t color) {
		uint8_t a = (color >> 24) & 0xFF;
		uint8_t r = (color >> 16) & 0xFF;
		uint8_t g = (color >> 8) & 0xFF;
		uint8_t b = color & 0xFF;

		uint8_t a_bit = (a >= 128) ? 1 : 0; // 1-bit transparency (opaque if alpha >= 128)
		uint8_t r_2bit = (r >> 6) & 0x03; // 2-bit red
		uint8_t g_2bit = (g >> 6) & 0x03; // 2-bit green
		uint8_t b_2bit = (b >> 6) & 0x03; // 2-bit blue

		return (a_bit << 7) | (r_2bit << 4) | (g_2bit << 2) | b_2bit;
	}

	// Convert 8-bit color (1-bit transparency, 1 unused bit, RGB222) to ARGB8888
	uint32_t convert8to32color(uint8_t color) {
		uint8_t a_bit = (color >> 7) & 0x01;
		uint8_t r_2bit = (color >> 4) & 0x03;
		uint8_t g_2bit = (color >> 2) & 0x03;
		uint8_t b_2bit = color & 0x03;

		uint8_t a = a_bit ? 0x00 : 0xFF; // Full transparency or fully opaque
		uint8_t r = (r_2bit << 6) | (r_2bit << 4) | (r_2bit << 2) | r_2bit;
		uint8_t g = (g_2bit << 6) | (g_2bit << 4) | (g_2bit << 2) | g_2bit;
		uint8_t b = (b_2bit << 6) | (b_2bit << 4) | (b_2bit << 2) | b_2bit;
		
		uint32_t result = (a << 24) | (r << 16) | (g << 8) | b;
		//std::cout<<std::hex<<(int)(color)<<" "<<(int)(a_bit)<<" "<<(int)(r_2bit)<<" "<<(int)(g_2bit)<<" "<<(int)(b_2bit)<<" "<<result<<std::dec<<std::endl;
		return result;
	}

	// Convert ARGB8888 to 4-bit color (ARGB1111)
	uint8_t convert32to4color(uint32_t color) {
		uint8_t a = (color >> 24) & 0xFF;
		uint8_t r = (color >> 16) & 0xFF;
		uint8_t g = (color >> 8) & 0xFF;
		uint8_t b = color & 0xFF;

		uint8_t a_bit = (a >= 128) ? 1 : 0; // 1-bit alpha
		uint8_t r_bit = (r >= 128) ? 1 : 0; // 1-bit red
		uint8_t g_bit = (g >= 128) ? 1 : 0; // 1-bit green
		uint8_t b_bit = (b >= 128) ? 1 : 0; // 1-bit blue

		return (a_bit << 3) | (r_bit << 2) | (g_bit << 1) | b_bit;
	}

	// Convert 4-bit color (ARGB1111) to ARGB8888
	uint32_t convert4to32color(uint8_t color) {
		uint8_t a_bit = (color >> 3) & 0x01;
		uint8_t r_bit = (color >> 2) & 0x01;
		uint8_t g_bit = (color >> 1) & 0x01;
		uint8_t b_bit = color & 0x01;

		uint8_t a = a_bit ? 0xFF : 0x00; // Full transparency or fully opaque
		uint8_t r = r_bit ? 0xFF : 0x00; // 0xFF if 1, else 0x00
		uint8_t g = g_bit ? 0xFF : 0x00;
		uint8_t b = b_bit ? 0xFF : 0x00;

		return (a << 24) | (r << 16) | (g << 8) | b;
	}
	
}
