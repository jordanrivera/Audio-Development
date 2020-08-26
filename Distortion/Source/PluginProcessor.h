/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "VUMeter.h"
#include <math.h>
#include <vector>
#include <algorithm>
#include <memory>
#include "Analyser.h"
using namespace std;
//==============================================================================
/**
*/
class DistortionAudioProcessor  : public AudioProcessor,
                                  public ChangeBroadcaster
{
public:
    //==============================================================================
    DistortionAudioProcessor();
    ~DistortionAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

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
    //ANALYSER
    static String paramOutput;
    const std::vector<double>& getMagnitudes ();
    void createAnalyserPlot (Path& p, const Rectangle<int> bounds, float minFreq, bool input);
    void createFrequencyPlot (Path& p, const std::vector<double>& mags, const Rectangle<int> bounds, float pixelsPerDouble);
    bool checkForNewAnalyserData();
    //==============================================================================
    //==============================================================================
    struct Band {
        String      name;
        Colour      colour;
        float       frequency = 1000.0f;
        float       quality   = 1.0f;
        float       gain      = 1.0f;
        bool        active    = true;
        std::vector<double> magnitudes;
    };
    //==============================================================================
    
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    static String drivE;
    static String rangE;
    static String blenD;
    static String volumE;

    
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    AudioProcessorValueTreeState state;
    VUMeter meterSourceL;
    VUMeter meterSourceR;
    float lastSampleRate;
    
private:
    //===========================================================
    //ANALYSER
    void updatePlots ();
    bool wasBypassed = true;
    
    std::vector<Band>    bands;
    std::vector<double> frequencies;
    std::vector<double> magnitudes;
    
    int soloed = -1;
    double sampleRate = 0;
    dsp::Gain<float> gaIn;
    Analyser<float> inputAnalyser;
    Analyser<float> outputAnalyser;
    //Filter
    
    //===========================================================
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionAudioProcessor)
};
