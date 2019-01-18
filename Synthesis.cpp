#include "Synthesis.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"
#include <iostream>

const int kNumPrograms = 1;

enum EParams
{
  kFrequency = 0,
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

  //arguments are: name, defaultVal, minVal, maxVal, step, label
  GetParam(kFrequency)->InitDouble("Frequency", 440., 20., 10000.0, 0.01, "Hz");
  GetParam(kFrequency)->SetShape(2.);

  IGraphics* pGraphics = MakeGraphics(this, kWidth, kHeight);
  pGraphics->AttachPanelBackground(&COLOR_RED);

  IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, kKnobFrames);

  pGraphics->AttachControl(new IKnobMultiControl(this, kFrequencyX, kFrequencyY, kFrequency, &knob));

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

	osc.generate(outL, nFrames);
	for (int i = 0; i < nFrames; ++i) {
		outR[i] = outL[i];
	}
}

void Synthesis::ProcessMidiMsg(IMidiMsg* midiMsg)
{
	osc.setWaveShape(sine);
}

void Synthesis::Reset()
{
  TRACE;
  IMutexLock lock(this);
  osc.setSamplingRate(GetSampleRate());
}

void Synthesis::OnParamChange(int paramIdx)
{
  IMutexLock lock(this);

  switch (paramIdx)
  {
    case kFrequency:
      osc.setFrequency(GetParam(kFrequency)->Value());
      break;

    default:
      break;
  }
}