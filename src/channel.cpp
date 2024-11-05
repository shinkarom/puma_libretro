#include "channel.hpp"

#include <iostream>
#include "bus.hpp"

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
	init(0, 32, 14080);
}

void Channel::init(uint32_t start, uint32_t end, int smplRate) {
	std::cout<<"Channel init with "<<start<<" "<<end<<" "<<smplRate<<std::endl;
	sampleRate = smplRate;
	startAddress = start;
	endAddress = end;
	length = end-start;
	cm_SourceInfo sourceInfo = {.handler = &cmEventHandler, .udata = this, .samplerate = sampleRate, .length = this->length};
	if(source != nullptr){
		cm_destroy_source(source);
	}
	source = cm_new_source(&sourceInfo);
	cm_set_master_gain(1);
	//cm_set_pitch(source, 1);
	//cm_set_loop(source, 1);	
	pos = startAddress;
}

void Channel::play() {
	cm_play(source);
}

void Channel::stop() {
	cm_stop(source);
}

void Channel::rewind() {
	pos = startAddress;
}

static int t = 0;

int16_t Channel::nextSample() {
	uint16_t s = bus::read16(pos);
	pos+=2;
	/*if(t<2){
		std::cout<<std::hex<<pos<<" "<<s<<std::dec<<std::endl;
		t++;
	}*/
	if(pos >= endAddress) {
		pos = startAddress;
	}
	return s;
}

void Channel::fillBuffer(int16_t* buffer, int bufLength) {
	for(int i = 0; i<bufLength; i+=2) {
		auto s = nextSample();
		buffer[i] = s;
		buffer[i+1] = s;
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
