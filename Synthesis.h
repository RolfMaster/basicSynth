#ifndef __SYNTHESIS__
#define __SYNTHESIS__

#include "IPlug_include_in_plug_hdr.h"
#include "Oscillator.h"
#include "MidiQ.h"
#include <unordered_set>

class Synthesis : public IPlug
{
public:
  Synthesis(IPlugInstanceInfo instanceInfo);
  ~Synthesis();

  void Reset();
  void OnParamChange(int paramIdx);
  void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);
  void ProcessMidiMsg(IMidiMsg* midiMsg) override;

private:
  double mFrequency;
  MidiQ midiQ;
  std::unordered_set<int> activeVoices;
  Oscillator voices[128];
  const double attenuationFactor = std::pow(10.0, -0.6); //required to get a single oscillator to -12dB
  LFO mLFO;
};

#endif
