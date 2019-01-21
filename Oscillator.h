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
	static void setSamplingRate(double samplinRate);
	static void setWaveShape(WAVE_SHAPE shape);
	bool isMuted = true;
	static void setVelocity(double velocity);
	void noteEvent(IMidiMsg::EStatusMsg status);
	static void setEnvelopeParams(STAGE stage, double value);
	void updatePhaseDelta();
	void setMuted(bool);

private:
	static WAVE_SHAPE waveShape;
	double twoPi;
	double phase;
	double phaseDelta;
	static double samplingRate;
	double frequency;
	static double velocity;
	Envelope envelope;
};

