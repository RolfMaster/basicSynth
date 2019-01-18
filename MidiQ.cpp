#include "MidiQ.h"



MidiQ::MidiQ()
{
}


MidiQ::~MidiQ()
{
}

void MidiQ::processMidiMsg(IMidiMsg* midiMsg){

}

int MidiQ::Resize(int size)
{
	return midiQ.Resize(size);
}
