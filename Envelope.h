#pragma once
#include "IPlugStructs.h"

enum STAGE { ATTACK, DECAY, SUSTAIN, RELEASE, DONE};

class Envelope
{
public:
	Envelope();
	~Envelope();

	static void setAttack(double);
	static void setDecay(double);
	static void setSustain(double);
	static void setRelease(double);
	static void setLegato(bool);

	void setCurrentStage(STAGE);
	STAGE getCurrentStage();
	STAGE currentStage;

	void noteEvent(IMidiMsg::EStatusMsg statusMsg);

	double generateEnvelopeValue();

private:
	static double attack;
	static double decay;
	static double sustain;
	static double release;
	static bool legato;

	
	void nextStage();
	double position = 0.0;
	double lastValue;

};

