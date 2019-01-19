#pragma once
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wextra-tokens"
#include "IPlug_include_in_plug_hdr.h"
#pragma clang diagnostic pop

#include "IMidiQueue.h"
#include "Oscillator.h"

class MidiQ
{
public:
	MidiQ();
	~MidiQ();

	int Resize(int size);
	void Add(IMidiMsg* midiMsg);
	void Flush(int nFrames);
	void setOscillatorParams(Oscillator* osc, int offset);

private:
	IMidiQueue iMidiQ;
	int lastNotePlayed;
};

