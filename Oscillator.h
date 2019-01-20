#pragma once
#include "Envelope.h"

enum WAVE_SHAPE {sine, triangle, saw, square};

class Oscillator
{
public:
	Oscillator();
	~Oscillator();
	double generate();
	void setFrequency(double frequency);
	void setSamplingRate(double samplinRate);
	void setWaveShape(WAVE_SHAPE shape);
	bool isMuted = true;
	void setVelocity(double velocity);
	void noteEvent(IMidiMsg::EStatusMsg status);

private:
	WAVE_SHAPE waveShape;
	double twoPi;
	double phase;
	double phaseDelta;
	double samplingRate;
	double frequency;
	void updatePhaseDelta();
	double velocity;
	Envelope envelope;
};

