#include "channel.hpp"

#include <iostream>

extern "C" {
	#include "cmixer.h"
}

int16_t waveform[32];

void cmEventHandler(cm_Event *e) {
	auto chan = (Channel*)(e->udata);
	switch(e->type) {
		case CM_EVENT_SAMPLES: {
			chan->fillBuffer(e->buffer, e->length);	
		}
		case CM_EVENT_REWIND: {
			chan->rewind();
		}
		default:
			break;
	}
}

Channel::Channel() {
	init();
}

void Channel::init() {
	sampleRate = 14080;
	length = 32;
	cm_SourceInfo sourceInfo = {.handler = &cmEventHandler, .udata = this, .samplerate = sampleRate, .length = this->length};
	source = cm_new_source(&sourceInfo);
	cm_set_master_gain(1);
	cm_set_pitch(source, 1);
	cm_set_loop(source, 1);	
}

void Channel::play() {
	cm_play(source);
}

void Channel::stop() {
	cm_stop(source);
}

void Channel::rewind() {
	pos = 0;
}

void Channel::fillBuffer(int16_t* buffer, int bufLength) {
	for(int i = 0; i<bufLength/2; i++) {
		buffer[i*2] = waveform[pos];
		buffer[i*2+1] = waveform[pos];
		pos = (pos+1)%length;
	}
	/*
   for(int i = 0; i<bufLength;i++){
		std::cout<<buffer[i*2]<<std::endl;
	}
	std::cout<<"---"<<std::endl;
	*/

}

namespace channel {
	
	void init() {
		for(int i = 0; i<32; i++) {
			waveform[i] = (i<16) ? -32767 : 32767;
		}
	}
	
}
