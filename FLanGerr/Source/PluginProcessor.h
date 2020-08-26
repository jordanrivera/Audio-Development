/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <math.h>
#include "VUMeter.h"
#include <vector>
#include <algorithm>
#include <memory>
using namespace std;
//==============================================================================
/**
*/
class FlanGerrAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    FlanGerrAudioProcessor();
    ~FlanGerrAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    void reset() override;
    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
     //==============================================================================
    enum Parameters
    {
        kDelayParam = 0,
        kSweepWidthParam,
        kDepthParam,
        kFeedbackParam,
        kFrequencyParam,
        
        kWaveformParam,
        kInterpolationParam,
        kStereoParam,
        kNumParameters
    };
    
    enum Waveforms
    {
        kWaveformSine = 1,
        kWaveformTriangle,
        kWaveformSquare,
        kWaveformSawtooth,
        kNumWaveforms
    };
    
    enum Interpolation
    {
        kInterpolationNearestNeighbour = 1,
        kInterpolationLinear,
        kInterpolationCubic,
        kNumInterpolations
    };
    
    static const float kMaximumDelay;
    static const float kMaximumSweepWidth;
    
    static String delaY;
    static String sweeP;
    static String deptH;
    static String feeD;
    static String freQ;
    static String waveF;
    static String interP;
    static String stereO;
    
    float delay_;      // Minimum length of delay line in seconds
    float sweepWidth_; // Amount of change from min to max delay
    float depth_;      // Mix level of delayed signal (0-1)
    float feedback_;   // Feedback level for feedback flanger (0-<1)
    float frequency_;  // LFO frequency (Hz)
    
    int   waveform_;   // What shape should be used for the LFO
    int   interpolation_; // What type of interpolation to use
    int   stereo_;     // Whether to use stereo (quadrature-phase) flanging
   
    int stereoCH;
    int waveCH;
    int interCH;
    
    AudioProcessorValueTreeState& getValueTreeState();
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    AudioProcessorValueTreeState state;
    
     //==============================================================================
   
    VUMeter meterSourceL;
    VUMeter meterSourceR;
    //==============================================================================
private:

    float lfo(float phase, int waveform);
     
     // Circular buffer variables for implementing delay
    AudioSampleBuffer delayBuffer_;
    int delayBufferLength_;
    int delayWritePosition_;
    
    float lfoPhase_;   // Phase of the low-frequency oscillator
    double inverseSampleRate_; // It's more efficient to multiply than divide, so
                               // cache the inverse of the sample rate
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FlanGerrAudioProcessor)
};
