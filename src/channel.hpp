#pragma once

#include <cstdint>

extern "C" {
	#include "cmixer.h"
}

class Channel {
	public:
		uint32_t startAddress;
		uint32_t endAddress;
		int length;
		int sampleRate;
		Channel();
		void play();
		void stop();
		void rewind();
		void init(uint32_t start, uint32_t end, int smplRate);
		void fillBuffer(int16_t* buffer, int bufLength);
	private:
		cm_Source* source;
		int pos;
		int16_t nextSample();
};

namespace channel {
	
	void init();

}
