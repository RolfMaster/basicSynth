#pragma once
#include "Envelope.h"

enum WAVE_SHAPE {sine, triangle, saw, square, numEnums};

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
	void setPhaseDelta(double freq);
	void setMuted(bool);
	int currentNote;
	void setCurrentStage(STAGE);
	Envelope envelope;
	static double velocity;
	void setLFOPitchBendPTR(double*);
	void setLFORawPTR(double*);

private:
	static WAVE_SHAPE waveShape;
	double* LFOPitchBend;
	double* LFORaw;

protected:
	double twoPi;
	double phase;
	double phaseDelta;
	static double samplingRate;
	double frequency;
	
};

class LFO : public Oscillator {
public:
	LFO();
	double generate();
	double pitchBend;
	double rawResult;
	void setPitchBendAmount(double);
	void setLFOWaveShape(WAVE_SHAPE);

private:
	WAVE_SHAPE LFOWaveShape;
	double pitchBendAmount;
};