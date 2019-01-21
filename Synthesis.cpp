#include "Synthesis.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"
#include <iostream>

const int kNumPrograms = 1;

enum EParams
{
  kAttack = 0,
  kDecay,
  kSustain,
  kRelease,
  kNumParams
};

enum ELayout
{
  kWidth = GUI_WIDTH,
  kHeight = GUI_HEIGHT,

  kFrequencyX = 100,
  kFrequencyY = 100,
  kKnobFrames = 60
};

Synthesis::Synthesis(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo), mFrequency(1.)
{
  TRACE;

  IGraphics* pGraphics = MakeGraphics(this, kWidth, kHeight);
  IBitmap knobBitmap = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, 1);
  
  
  GetParam(kAttack)->InitDouble("Attack", 0., 0., 4.0, 0.01, "sec");
  GetParam(kAttack)->SetShape(2.);
  pGraphics->AttachControl(new IKnobRotaterControl(this, 100, 100, kAttack, &knobBitmap));

  GetParam(kDecay)->InitDouble("Decay", 0., 0., 4.0, 0.01, "sec");
  GetParam(kDecay)->SetShape(2.);
  pGraphics->AttachControl(new IKnobRotaterControl(this, 200, 100, kDecay, &knobBitmap));

  GetParam(kSustain)->InitDouble("Sustain", 1., 0., 1., 0.01);
  GetParam(kSustain)->SetShape(1.);
  pGraphics->AttachControl(new IKnobRotaterControl(this, 300, 100, kSustain, &knobBitmap));

  GetParam(kRelease)->InitDouble("Release", 0., 0., 4.0, 0.01, "sec");
  GetParam(kRelease)->SetShape(2.);
  pGraphics->AttachControl(new IKnobRotaterControl(this, 400, 100, kRelease, &knobBitmap));

  pGraphics->AttachBackground(BACKGROUND_ID, BACKGROUND_FN);

  

  

  AttachGraphics(pGraphics);

  //MakePreset("preset 1", ... );
  MakeDefaultPreset((char *) "-", kNumPrograms);
}

Synthesis::~Synthesis() {}

void Synthesis::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
  // Mutex is already locked for us.
	
	double* outL = outputs[0];
	double* outR = outputs[1];

	double attenuation = 0.25118864315; //required to get -12dB on one oscillator

	for (int i = 0; i < nFrames; ++i){
		midiQ.setOscillatorParams(voices, i);
		outL[i] = outR[i] = 0.0;
		for (Oscillator &osc : voices) {
			outL[i] += osc.generate()*attenuation;
			outR[i] = outL[i];
		}
	}
	midiQ.Flush(nFrames);

	
}

void Synthesis::ProcessMidiMsg(IMidiMsg* midiMsg)
{
	IMidiMsg::EStatusMsg status = midiMsg->StatusMsg();
	if (status == IMidiMsg::kNoteOn || status == IMidiMsg::kNoteOff) {
		midiQ.Add(midiMsg);
	}
}

void Synthesis::Reset()
{
  TRACE;
  IMutexLock lock(this);

  Oscillator::setSamplingRate(GetSampleRate());
  for (auto &osc : voices) {
	  osc.updatePhaseDelta();
  }
  midiQ.Resize(GetBlockSize());
}

void Synthesis::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);

	switch (paramIdx){
		case (kAttack):
			Oscillator::setEnvelopeParams(ATTACK, GetParam(paramIdx)->Value());
			break;
		case (kDecay):
			Oscillator::setEnvelopeParams(DECAY, GetParam(paramIdx)->Value());
			break;
		case (kSustain):
			Oscillator::setEnvelopeParams(SUSTAIN, GetParam(paramIdx)->Value());
			break;
		case (kRelease):
			Oscillator::setEnvelopeParams(RELEASE, GetParam(paramIdx)->Value());
			break;
		default:
			break;
  }	
}