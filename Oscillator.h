#pragma once

enum WAVE_SHAPE {sine, triangle, saw, square};

class Oscillator
{
public:
	Oscillator();
	~Oscillator();
	void generate(double* buffer, int nFrames);
	void setFrequency(double frequency);
	void setSamplingRate(double samplinRate);
	void setWaveShape(WAVE_SHAPE shape);


private:
	WAVE_SHAPE waveShape;
	double pi;
	double phase;
	double phaseDelta;
	double samplingRate;
	double frequency;
	void updatePhaseDelta();
};

