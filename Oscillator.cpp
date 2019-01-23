#include "Oscillator.h"
#include <math.h>
#include <cmath>

WAVE_SHAPE Oscillator::waveShape;
double Oscillator::velocity;
double Oscillator::samplingRate;

Oscillator::Oscillator()
{
	phase = 0.0;
	twoPi = 4 * acos(0.0);
	frequency = 440.0;
	samplingRate = 44100.0;
	updatePhaseDelta();
	isMuted = true;
	waveShape = sine;
}


Oscillator::~Oscillator()
{
}

double Oscillator::generate()
{
	isMuted = (envelope.currentStage == STAGE::DONE);
	if (isMuted) return 0.0;

	setPhaseDelta(frequency * (*LFOPitchBend));

	double result = 0.0;

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
	//result *= envelope.generateEnvelopeValue();
	
	return result;
}

void Oscillator::setFrequency(double freq)
{
	frequency = freq;
	updatePhaseDelta();
}

void Oscillator::setSamplingRate(double samplinRate) { samplingRate = samplinRate; }

void Oscillator::setWaveShape(WAVE_SHAPE shape) { waveShape = shape; }

void Oscillator::setVelocity(double vel) { velocity = vel; }

void Oscillator::noteEvent(IMidiMsg::EStatusMsg status){ envelope.noteEvent(status); }

void Oscillator::setEnvelopeParams(STAGE stage, double value)
{
	switch (stage) {
		case(ATTACK): Envelope::setAttack(value*samplingRate); break;
		case(DECAY): Envelope::setDecay(value*samplingRate); break;
		case(SUSTAIN): Envelope::setSustain(value); break;
		case(RELEASE): Envelope::setRelease(value*samplingRate); break;
	}
}

void Oscillator::updatePhaseDelta()
{
	phaseDelta = twoPi * frequency / samplingRate;
}

void Oscillator::setPhaseDelta(double freq)
{
	phaseDelta = twoPi * freq / samplingRate;
}

void Oscillator::setMuted(bool b) { isMuted = b; }

void Oscillator::setCurrentStage(STAGE stage) { envelope.currentStage = stage; }

void Oscillator::setLFOPitchBendPTR(double* d) { LFOPitchBend = d; }

void Oscillator::setLFORawPTR(double* d) { LFORaw = d; }

LFO::LFO()
{
	phase = 0.0;
	twoPi = 4 * acos(0.0);
	frequency = 440.0;
	samplingRate = 44100.0;
	updatePhaseDelta();
	isMuted = false;
	LFOWaveShape = sine;
	pitchBendAmount = 0;
}

double LFO::generate()
{
	double result = 0.0;

	switch (LFOWaveShape) {
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
		result = (phase > (twoPi / 2)) ? 1.0 : -1.0;
		phase += phaseDelta;
		break;
	}

	while (phase >= twoPi) {
		phase -= twoPi;
	}

	rawResult = result;

	//result *= amplitude;
	pitchBend = std::pow(2.0, result*pitchBendAmount);
	return result;
}

void LFO::setPitchBendAmount(double d) { pitchBendAmount = d; }

void LFO::setLFOWaveShape(WAVE_SHAPE shape) { LFOWaveShape = shape; }
