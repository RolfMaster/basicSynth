#include "Oscillator.h"
#include <math.h>



Oscillator::Oscillator()
{
	phase = 0.0;
	twoPi = 4 * acos(0.0);
	frequency = 440.0;
	samplingRate = 44100.0;
	updatePhaseDelta();
	waveShape = square;
}


Oscillator::~Oscillator()
{
}

double Oscillator::generate()
{
	if (isMuted) return 0.0;

	double result;

	switch (waveShape) {
	case(sine):
		result = sin(phase);
		phase += phaseDelta;
		break;
	case(saw):
		result = 2 * (phase / twoPi) - 1.0;
		phase += phaseDelta;
		break;
	case(triangle):
		result = 4 * fabs((phase / twoPi) - 0.5) - 1.0;
		phase += phaseDelta;
		break;
	case(square):
		result = (phase > (twoPi/2)) ? 1.0 : -1.0;
		phase += phaseDelta;
		break;
	}

	while (phase >= twoPi) {
		phase -= twoPi;
	}

	//result *= velocity;
	result *= envelope.generateEnvelopeValue();
	
	return result;
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

void Oscillator::setWaveShape(WAVE_SHAPE shape)
{
	waveShape = shape;
}

void Oscillator::setVelocity(double vel)
{
	velocity = vel;
}

void Oscillator::noteEvent(IMidiMsg::EStatusMsg status){ envelope.noteEvent(status); }

void Oscillator::updatePhaseDelta()
{
	phaseDelta = twoPi * frequency / samplingRate;
}
