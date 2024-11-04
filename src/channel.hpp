#pragma once

#include <cstdint>

extern "C" {
	#include "cmixer.h"
}

class Channel {
	public:
		uint32_t address;
		int length;
		int sampleRate;
		Channel();
		void play();
		void stop();
		void rewind();
		void init();
		void fillBuffer(int16_t* buffer, int bufLength);
	private:
		cm_Source* source;
		int pos;
};

namespace channel {
	
	void init();

}
