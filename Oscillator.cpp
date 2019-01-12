#include "Oscillator.h"
#include <math.h>



Oscillator::Oscillator()
{
	phase = 0.0;
	pi = 2 * acos(0.0);
	frequency = 440.0;
	samplingRate = 44100.0;
	updatePhaseDelta();
	waveShape = square;
}


Oscillator::~Oscillator()
{
}

void Oscillator::generate(double * buffer, int nFrames)
{
	double twoPi = 2 * pi;

	switch (waveShape) {
	case(sine):
		for (int i = 0; i < nFrames; ++i) {
			buffer[i] = sin(phase);
			phase += phaseDelta;
			while (phase >= twoPi) {
				phase -= twoPi;
			}
		}
		break;
	case(saw):
		for (int i = 0; i < nFrames; ++i) {
			buffer[i] = 2*(phase / twoPi) - 1.0;
			phase += phaseDelta;
			while (phase >= twoPi){
				phase -= twoPi;
			}
		}
		break;
	case(triangle):
		for (int i = 0; i < nFrames; ++i) {
			buffer[i] = 4*fabs((phase/twoPi)-0.5)-1.0;
			phase += phaseDelta;
			while (phase >= twoPi) {
				phase -= twoPi;
			}
		}
		break;
	case(square):
		for (int i = 0; i < nFrames; ++i) {
			
			buffer[i] = (phase > 0) ? 1.0 : -1.0;
			phase += phaseDelta;
			while (phase >= pi) {
				phase -= twoPi;
			}
		}
		break;
	}

	
}

void Oscillator::setFrequency(double freq)
{
	frequency = freq;
	updatePhaseDelta();
}

void Oscillator::setSamplingRate(double samplinRate)
{
	samplingRate = samplinRate;
	updatePhaseDelta();
}

void Oscillator::updatePhaseDelta()
{
	phaseDelta = 2 * pi * frequency / samplingRate;
}
