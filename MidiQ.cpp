#include "MidiQ.h"
#include <cmath>



MidiQ::MidiQ()
{
}


MidiQ::~MidiQ()
{
}

int MidiQ::Resize(int size)
{
	return iMidiQ.Resize(size);
}

void MidiQ::Add(IMidiMsg * midiMsg)
{
	iMidiQ.Add(midiMsg);
}

void MidiQ::Flush(int nFrames)
{
	iMidiQ.Flush(nFrames);
}

void MidiQ::setOscillatorParams(Oscillator* osc, int offset)
{
	while (!iMidiQ.Empty()) {
		IMidiMsg* midiMsg = iMidiQ.Peek();
		if (midiMsg->mOffset > offset) break;

		IMidiMsg::EStatusMsg status = midiMsg->StatusMsg();
		osc->noteEvent(status);

		if (status == IMidiMsg::kNoteOn) {
			osc->isMuted = false;
			osc->setFrequency(440 * std::pow(2.0, ((midiMsg->NoteNumber() - 69) / 12.0)));

			if (midiMsg->Velocity() == 0) osc->isMuted = true;
			else osc->setVelocity(midiMsg->Velocity() / 127.0);

			lastNotePlayed = midiMsg->NoteNumber();
		}
		/*
		else if (status == IMidiMsg::kNoteOff) {
			if (midiMsg->NoteNumber() == lastNotePlayed) {
				osc->isMuted = true;
			}
		}
		*/
		iMidiQ.Remove();
	}
}
