#pragma once
class Oscillator
{
public:
	Oscillator();
	~Oscillator();
	void generate(double* buffer, int nFrames);
	void setFrequency(double frequency);
	void setSamplingRate(double samplinRate);


private:
	double pi;
	double phase;
	double phaseDelta;
	double samplingRate;
	double frequency;
	void updatePhaseDelta();
};

