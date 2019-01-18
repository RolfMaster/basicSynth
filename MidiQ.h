#pragma once
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wextra-tokens"
#include "IPlug_include_in_plug_hdr.h"
#pragma clang diagnostic pop

#include "IMidiQueue.h"

class MidiQ
{
public:
	MidiQ();
	~MidiQ();

	void processMidiMsg(IMidiMsg* midiMsg);
	int Resize(int size);

private:
	IMidiQueue midiQ;
};

