#include "Envelope.h"



Envelope::Envelope()
{
	Envelope::attack = 20000.0;
	Envelope::decay = 5000.0;
	Envelope::sustain = 0.8;
	Envelope::release = 10000.0;
	Envelope::legato = true;

	currentStage = DONE;
}


Envelope::~Envelope()
{
}

double Envelope::attack;
double Envelope::decay;
double Envelope::sustain;
double Envelope::release;
bool Envelope::legato;


void Envelope::setAttack(double att){attack = att;}
void Envelope::setDecay(double dec){decay = dec;}
void Envelope::setSustain(double sus){sustain = sus;}
void Envelope::setRelease(double rel){release = rel;}
void Envelope::setLegato(bool b){legato = b;}

void Envelope::setCurrentStage(STAGE stage)
{
	STAGE a = stage;
	currentStage = stage;
}

STAGE Envelope::getCurrentStage() {return currentStage;}

void Envelope::noteEvent(IMidiMsg::EStatusMsg statusMsg){
	if (statusMsg == IMidiMsg::kNoteOn) {
		if (legato)
			position = generateEnvelopeValue()*attack;
		else
			position = 0.0;

		setCurrentStage(STAGE::ATTACK);
	}
	else if (statusMsg == IMidiMsg::kNoteOff) {
		switch (currentStage) {
		case(ATTACK):
			lastValue = position / attack;
			break;
		case(DECAY):
			lastValue = 1 - (1 - sustain)*(position / decay);
			break;
		case(SUSTAIN):
			lastValue = sustain;
			break;
		case(RELEASE):
			lastValue = sustain - (position / release)*sustain;
		}
		currentStage = STAGE::RELEASE;
		position = 0;
	}
	return;
}

double Envelope::generateEnvelopeValue()
{
	++position;
	double res;
	switch (currentStage) {
	case(ATTACK):
		if (position <= attack)
			res = position / attack;
		else {
			nextStage();
			res = generateEnvelopeValue();
		}
		break;
	case(DECAY):
		if (position <= decay)
			res = 1 - (1 - sustain)*(position / decay);
		else {
			nextStage();
			res = generateEnvelopeValue();
		}
		break;
	case(SUSTAIN):
		res = sustain;
		break;
	case(RELEASE):
		if (position <= release)
			res = lastValue - (position / release)*lastValue;
		else {
			nextStage();
			res = 0.0;
		}
		break;
	case(DONE):
		res = 0.0;
		position = 0.0;
		break;
	}
	return res;
}

void Envelope::nextStage()
{
	position = 0;

	switch (currentStage) {
	case(ATTACK):
		currentStage = DECAY;
		break;
	case(DECAY):
		currentStage = SUSTAIN;
		break;
	case(SUSTAIN):
		currentStage = RELEASE;
		break;
	case(RELEASE):
		currentStage = DONE;
		break;
	}
}
