/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/


#ifndef __PLUGINPROCESSOR_H_4693CB6E__
#define __PLUGINPROCESSOR_H_4693CB6E__

#define _USE_MATH_DEFINES

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "OnePoleAllpassFilter.h"
#include "TwoPoleAllpassFilter.h"
#include <math.h>
#include "VUMeter.h"
//==============================================================================
/**
*/
class PhaserAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    PhaserAudioProcessor();
    ~PhaserAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void reset() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;
    //==============================================================================
    const String getInputChannelName (int channelIndex) const override;
    const String getOutputChannelName (int channelIndex) const override;
    //==============================================================================
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
    
    
    
    enum Waveforms
    {
        kWaveformSine = 1,
        kWaveformTriangle,
        kWaveformSquare,
        kWaveformSawtooth,
        kNumWaveforms
    };
    
    // Adjustable parameters:
    float baseFrequency_; // Lowest frequency of allpass filters
    float sweepWidth_;    // Amount of change from min to max delay
    float depth_;         // Mix level for phase-shifted signal (0-1)
    float feedback_;      // Feedback level for feedback phaser (0-<1)
    float lfoFrequency_;
    float gain_;            // LFO frequency (Hz)
   
    int   filtersPerChannel_; // How many allpass filters to use
    int   waveform_;      // What shape should be used for the LFO
    int   stereo_;        // Whether to use stereo phasing
    
    float filtersPCH;
    float stereoCH;
    float wavefoRM;
    
    static String baseFrequency;
    static String sweepWidth;
    static String deptH;
    static String feedBack;
    static String lfoFrequency;
    static String filteR;
    static String waveForm;
    static String stereO;
    static String gaiN;
    
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    AudioProcessorValueTreeState parameterState;
    VUMeter meterSourceL;
    VUMeter meterSourceR;

private:

    
    float lfo(float phase, int waveform);
    void allocateFilters();   // Create the filter objects...
    void deallocateFilters(); // Delete them...
    void reallocateFilters(); // Delete and rebuild in one combined operation
    
    CriticalSection lock_; // Lock for updating number of filters
    
    float lfoPhase_;   // Phase of the low-frequency oscillator
    double inverseSampleRate_; // It's more efficient to multiply than divide, so
                               // cache the inverse of the sample rate
    unsigned int sampleCount_; // Counter to keep track of samples elapsed, to
                               // keep track of filter updates
    unsigned int filterUpdateInterval_; // How often to update filter coefficients
    
    // Bank of allpass filters that do the phasing; N filters x M channels
    OnePoleAllpassFilter **allpassFilters_;
    
    // Storage of the last output sample from each bank of filters, for use in
    // feedback loop
    float *lastFilterOutputs_;
    int numLastFilterOutputs_;
    int totalNumFilters_;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhaserAudioProcessor)
};

#endif  // __PLUGINPROCESSOR_H_4693CB6E__
