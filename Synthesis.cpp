#include "Synthesis.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"
#include <iostream>

const int kNumPrograms = 1;

enum EParams
{
	kWaveform = 0,
	kAttack,
	kDecay,
	kSustain,
	kRelease,
	kLFOPitchBendAmount,
	kLFOWaveform,
	kLFOFrequency,
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
  IBitmap waveformBitmap = pGraphics->LoadIBitmap(WAVEFORM_ID, WAVEFORM_FN, 4);

  //oscillator waveform
  GetParam(kWaveform)->InitEnum("Waveform", sine, numEnums);
  //GetParam(kWaveform)->SetDisplayText(0, "Sine"); // Needed for VST3
  pGraphics->AttachControl(new ISwitchControl(this, 10, 53, kWaveform, &waveformBitmap));

  //attack knob
  GetParam(kAttack)->InitDouble("Attack", 0., 0., 4.0, 0.01, "sec");
  GetParam(kAttack)->SetShape(2.);
  pGraphics->AttachControl(new IKnobRotaterControl(this, 100, 100, kAttack, &knobBitmap));

  //decay knob
  GetParam(kDecay)->InitDouble("Decay", 0., 0., 4.0, 0.01, "sec");
  GetParam(kDecay)->SetShape(2.);
  pGraphics->AttachControl(new IKnobRotaterControl(this, 200, 100, kDecay, &knobBitmap));

  //sustain knob
  GetParam(kSustain)->InitDouble("Sustain", 1., 0., 1., 0.01);
  GetParam(kSustain)->SetShape(1.);
  pGraphics->AttachControl(new IKnobRotaterControl(this, 300, 100, kSustain, &knobBitmap));

  //release knob
  GetParam(kRelease)->InitDouble("Release", 0., 0., 4.0, 0.01, "sec");
  GetParam(kRelease)->SetShape(2.);
  pGraphics->AttachControl(new IKnobRotaterControl(this, 400, 100, kRelease, &knobBitmap));

  //pitch bend amount knob
  GetParam(kLFOPitchBendAmount)->InitDouble("Pitch Bend", 0., 0., 1.0, 0.01);
  GetParam(kLFOPitchBendAmount)->SetShape(1.5);
  pGraphics->AttachControl(new IKnobRotaterControl(this, 400, 200, kLFOPitchBendAmount, &knobBitmap));

  //LFO waveform
  GetParam(kLFOWaveform)->InitEnum("LFO Waveform", sine, numEnums);
  //GetParam(kLFOWaveform)->SetDisplayText(0, "Sine"); // Needed for VST3
  pGraphics->AttachControl(new ISwitchControl(this, 200, 200, kLFOWaveform, &waveformBitmap));

  //LFO frequency knob
  GetParam(kLFOFrequency)->InitDouble("LFO Frequency", 0., 0., 20.0, 0.01, "Hz");
  GetParam(kLFOFrequency)->SetShape(2.);
  pGraphics->AttachControl(new IKnobRotaterControl(this, 300, 200, kLFOFrequency, &knobBitmap));

  pGraphics->AttachBackground(BACKGROUND_ID, BACKGROUND_FN);

  

  

  AttachGraphics(pGraphics);

  //MakePreset("preset 1", ... );
  MakeDefaultPreset((char *) "-", kNumPrograms);

  mLFO.setFrequency(1.0);
  mLFO.isMuted = false;
  mLFO.setCurrentStage(SUSTAIN);
  mLFO.setWaveShape(square);
  for (auto &osc : voices) {
	  osc.setLFOPitchBendPTR(&mLFO.pitchBend);
	  osc.setLFORawPTR(&mLFO.rawResult);
  }
}

Synthesis::~Synthesis() {}

void Synthesis::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
  // Mutex is already locked for us.
	
	double* outL = outputs[0];
	double* outR = outputs[1];

	for (int i = 0; i < nFrames; ++i) {
		midiQ.setOscillatorParams(voices, i);
		outL[i] = outR[i] = 0.0;
		for (int j = 0; j < 127; ++j) {
			if (!voices[j].isMuted) {
				outL[i] += voices[j].generate()*voices[j].velocity*voices[j].envelope.generateEnvelopeValue();
			}			
		}
		mLFO.generate();
		//outL[i] *= mLFO.generate();
		outL[i] *= attenuationFactor;
		outR[i] = outL[i];
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
		case(kWaveform):
			Oscillator::setWaveShape(static_cast<WAVE_SHAPE>(GetParam(kWaveform)->Int()));
			break;
		case(kLFOPitchBendAmount):
			mLFO.setPitchBendAmount(GetParam(paramIdx)->Value());
			break;
		case(kLFOWaveform):
			mLFO.setLFOWaveShape(static_cast<WAVE_SHAPE>(GetParam(kLFOWaveform)->Int()));
			break;
		case(kLFOFrequency):
			mLFO.setFrequency(GetParam(paramIdx)->Value());
			break;
		default:
			break;
  }	
}