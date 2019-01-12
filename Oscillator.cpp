#include "Oscillator.h"
#include <math.h>



Oscillator::Oscillator()
{
	phase = 0.0;
	pi = 2 * acos(0.0);
	frequency = 440.0;
	samplingRate = 44100.0;
	updatePhaseDelta();
}


Oscillator::~Oscillator()
{
}

void Oscillator::generate(double * buffer, int nFrames)
{
	for (int i = 0; i < nFrames; ++i) {
		buffer[i] = sin(phase);
		phase += phaseDelta;
		while (phase >= 2 * pi) {
			phase -= 2 * pi;
		}
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
