/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <math.h>

String PhaserAudioProcessor::baseFrequency ("basefrequency");
String PhaserAudioProcessor::sweepWidth ("sweepwidth");
String PhaserAudioProcessor::deptH ("depth");
String PhaserAudioProcessor::feedBack ("feedback");
String PhaserAudioProcessor::lfoFrequency("lfofrequency");
String PhaserAudioProcessor::filteR ("filter");
String PhaserAudioProcessor::waveForm ("waveform");
String PhaserAudioProcessor::stereO ("stereo");
String PhaserAudioProcessor::gaiN("Gain");

//==============================================================================
PhaserAudioProcessor::PhaserAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
                        parameterState(*this, nullptr, "PARAMETERS",createParameterLayout())
                       
#endif
{
  
    //waveform_ = kWaveformSine;
    
    allpassFilters_ = 0;
    //filtersPerChannel_ = 4;
    totalNumFilters_ = 0;
    lastFilterOutputs_ = 0;
    numLastFilterOutputs_ = 0;
    
    lfoPhase_ = 0.0;
    inverseSampleRate_ = 1.0/44100.0;
    sampleCount_ = 0;
    filterUpdateInterval_ = 8;
    
    stereo_ = *parameterState.getRawParameterValue(stereO);
    waveform_ = *parameterState.getRawParameterValue(waveForm);
    filtersPerChannel_ = *parameterState.getRawParameterValue(filteR);
    
    feedback_ = *parameterState.getRawParameterValue(feedBack);
    sweepWidth_ = *parameterState.getRawParameterValue(sweepWidth);
    baseFrequency_ = *parameterState.getRawParameterValue(baseFrequency);
    depth_ = *parameterState.getRawParameterValue(deptH);
    lfoFrequency_ = *parameterState.getRawParameterValue(lfoFrequency);
    
    //====================================================
    filtersPCH = filtersPerChannel_ < 0.5f ? 1.0f : -1.0f;
    stereoCH = stereo_ < 0.5f ? 1.0f : -1.0f;
    wavefoRM = waveform_ < 0.5f ? 1.0f : -1.0f;
    
    
}
AudioProcessorValueTreeState::ParameterLayout PhaserAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<RangedAudioParameter>> params;
    
    const StringArray waveformArr ({"Sine", "Triangle", "Square", "Sawtooth"});
    auto waveToText = [=](float value) { return waveformArr[(int) (value + 0.5)]; };
    auto textToWave = [=](const String& text) { return (float) waveformArr.indexOf (text); };
    const StringArray filterArr ({"2", "4","6","8","10"});
    auto filterToText = [=](float value) { return filterArr[(int) (value + 0.5)]; };
    auto textToFilter = [=](const String& text) { return (float) filterArr.indexOf (text); };
    const StringArray stereoArr ({"Stereo", "Mono"});
    auto stereoToText = [=] (float value) {return stereoArr[(int) (value + 0.5)];};
    auto textToStereo = [=] (const String& text) {return (float) stereoArr.indexOf (text); };
    
    
    
    NormalisableRange<float> wideRangeFQ (50.0f, 1000.0f, 0.5f);
    NormalisableRange<float> wideRangeSWW (50.0f, 5000.0f, 0.5f);
    NormalisableRange<float> wideRangeDPT (0.0f, 1.0f, 0.01f);
    NormalisableRange<float> wideRangeFB  (0.0f, 0.99f, 0.01f);
    NormalisableRange<float> widwRangeLFOF (0.05f, 2.0f, 0.025f);
    NormalisableRange<float> wideRangeW (0.0f,4.0f,1.0f);
    NormalisableRange<int> wideRangeF (0.0f,5.0f,1.0f);
    NormalisableRange<int> wideRangeS (0.0f,1.0f,1.0f);
    NormalisableRange<float> wideRangeG (-48.0f, 0.0f, -15.0f);
    
    
    auto baseFrequencyParam = std::make_unique<AudioParameterFloat> (baseFrequency,TRANS ("Basefrequency"), wideRangeFQ, 0.5f);
    auto sweepWithParam = std::make_unique<AudioParameterFloat> (sweepWidth,TRANS ("Sweepwidth"), wideRangeSWW,0.5f);
    
    auto deptHParam = std::make_unique<AudioParameterFloat> (deptH,TRANS("depth"), wideRangeDPT, 0.01f);
    
    auto feedBackParam = std::make_unique<AudioParameterFloat> (feedBack,TRANS ("Feedback"), wideRangeFB, 0.01f);
    auto lfoFrequencyParam = std::make_unique<AudioParameterFloat> (lfoFrequency, TRANS("Lfofrequency"), widwRangeLFOF, 0.025);
    auto gaiNParam = std::make_unique<AudioParameterFloat> (gaiN, TRANS ("Gain"), wideRangeG, 0.0f);
    
    auto filteRParam = std::make_unique<AudioParameterChoice> (filteR, TRANS("Filter"), filterArr,4.0,TRANS ("Filter"));
    
    auto waveFormParam = std::make_unique<AudioParameterChoice> (waveForm, TRANS("Waveform"), waveformArr,3.0, TRANS("Waveform"));
    auto stereOParam = std::make_unique<AudioParameterChoice> (stereO, TRANS("Stereo"), stereoArr, 1.0, TRANS("Stereo"));
    
    
    
    params.push_back(std::move(baseFrequencyParam));
    params.push_back(std::move(sweepWithParam));
    params.push_back(std::move(deptHParam));
    params.push_back(std::move(feedBackParam));
    params.push_back(std::move(lfoFrequencyParam));
    params.push_back(std::move(gaiNParam));
    params.push_back(std::move(filteRParam));
    params.push_back(std::move(waveFormParam));
    params.push_back(std::move(stereOParam));
    
    return { params.begin(), params.end() };
}
PhaserAudioProcessor::~PhaserAudioProcessor()
{
    deallocateFilters();
}

//==============================================================================
const String PhaserAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PhaserAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PhaserAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PhaserAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PhaserAudioProcessor::getTailLengthSeconds() const { return 0.0; }

const String PhaserAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String PhaserAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

int PhaserAudioProcessor::getNumPrograms()
{
    return 1;
    
}

int PhaserAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PhaserAudioProcessor::setCurrentProgram (int index)
{
}

const String PhaserAudioProcessor::getProgramName (int index)
{
    return {};
}

void PhaserAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void PhaserAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
  
    lfoPhase_ = 0.0;
    inverseSampleRate_ = 1.0/sampleRate;
    sampleCount_ = 0;
    
    const ScopedLock sl (lock_);
    allocateFilters();  
    
}

void PhaserAudioProcessor::releaseResources()
{
    const ScopedLock sl (lock_);
    deallocateFilters();
}

void PhaserAudioProcessor::reset()
{
    
    lfoPhase_ = 0.0;
    sampleCount_ = 0;
    for(int i = 0; i < numLastFilterOutputs_; i++)
        lastFilterOutputs_[i] = 0.0f;
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PhaserAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void PhaserAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{   
    const ScopedLock sl (lock_);
    
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    const int numSamples = buffer.getNumSamples();  
    float ph, channel0EndPhase = lfoPhase_;
    unsigned int sc;
    

    //for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    //    buffer.clear (i, 0, buffer.getNumSamples());

    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
     {
        // channelData is an array of length numSamples which contains the audio for one channel
        float* channelData = buffer.getWritePointer(channel);
         ph = lfoPhase_;
         sc = sampleCount_;
         
        
        
        for (int sample = 0; sample < numSamples; ++sample)
        {
            float out = channelData[sample];
            
            // If feedback is enabled, include the feedback from the last sample in the
            // input of the allpass filter chain. This is actually not accurate to how
            // analog phasers work because there is a sample of delay between output and
            // input, which adds a further phase shift of up to 180 degrees at half the
            // sampling frequency. To truly model an analog phaser with feedback involves
            // modelling a delay-free loop, which is beyond the scope of this example.
            
            if(feedback_ != 0.0 && channel < numLastFilterOutputs_)
                out += feedback_ * lastFilterOutputs_[channel];
            
            // See OnePoleAllpassFilter.cpp for calculation of coefficients and application
            // of filter to audio data. The filter processes the audio buffer in place,
            // putting the output sample in place of the input.
            
            
            
            for(int j = 0; j < filtersPCH ; ++j)
            {
                // Safety check
                if(channel * filtersPCH + j >= totalNumFilters_)
                    continue;
                
                // First, update the current allpass filter coefficients depending on the parameter
                // settings and the LFO phase
                
                // Recalculating the filter coefficients is much more expensive than calculating
                // a sample. Only update the coefficients at a fraction of the sample rate; since
                // the LFO moves slowly, the difference won't generally be audible.
                if(sc % filterUpdateInterval_ == 0)
                {
                    allpassFilters_[channel * (int)filtersPCH + j]->makeAllpass(inverseSampleRate_,
                       baseFrequency_ + sweepWidth_*lfo(ph, wavefoRM ));
                }
                out = allpassFilters_[channel * (int)filtersPCH + j]->processSingleSampleRaw(out);
            }
            
            if(channel < numLastFilterOutputs_)
                lastFilterOutputs_[channel] = out;
            
            // Add the allpass signal to the output, though maintaining constant level
            // depth = 0 --> input only ; depth = 1 --> evenly balanced input and output
            channelData[sample] = (1.0f-0.5f*depth_)*channelData[sample] + 0.5f*depth_*out;
        
            // Update the LFO phase, keeping it in the range 0-1
            ph += lfoFrequency_*inverseSampleRate_;
            if(ph >= 1.0)
                ph -= 1.0;
            sc++;
        }

        // Use channel 0 only to keep the phase in sync between calls to processBlock()
        // Otherwise quadrature phase on multiple channels will create problems.
        if(channel == 0)
            channel0EndPhase = ph;
    }
    
    lfoPhase_ = channel0EndPhase;
    sampleCount_ = sc;
    
    // Go through the remaining channels. In case we have more outputs
    // than inputs, or there aren't enough filters, we'll clear any
    // remaining output channels (which could otherwise contain garbage)
    
    for(int channel = totalNumInputChannels; channel < totalNumOutputChannels; ++channel)
    {
        buffer.clear (channel++, 0, buffer.getNumSamples());
    }
    
     for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
       
        
        // For stereo phasing, keep the channels 90 degrees out of phase with each other
        if(stereoCH != 0 && channel != 0)
            ph = fmodf(ph + 0.25, 1.0);
        
        auto* channelData = buffer.getWritePointer(channel);
        gain_ = *parameterState.getRawParameterValue(gaiN);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] = buffer.getSample(channel, sample) * Decibels::decibelsToGain(gain_);
        }
    }
    
    meterSourceL.measureBuffer(buffer);
    meterSourceR.measureBuffer(buffer);
}

//==============================================================================
bool PhaserAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* PhaserAudioProcessor::createEditor()
{
    return new PhaserAudioProcessorEditor (*this);
}
//=============================================================================
//==============================================================================
void PhaserAudioProcessor::getStateInformation (MemoryBlock& destData)
{
   // Create an outer XML element..
    std::unique_ptr<XmlElement> xml (parameterState.state.createXml());
    copyXmlToBinary(*xml, destData);
}

void PhaserAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // This getXmlFromBinary() helper function retrieves our XML from the binary blob..
    std::unique_ptr<XmlElement> theParams(getXmlFromBinary(data, sizeInBytes));
    
    if (theParams != nullptr)
    {
        if (theParams->hasTagName(parameterState.state.getType()))
        {
            parameterState.state = ValueTree::fromXml(*theParams);
        }
    }

   
}

//==============================================================================
// Function for calculating LFO waveforms. Phase runs from 0-1, output is scaled
// from 0 to 1 (note: not -1 to 1 as would be typical of sine).
float PhaserAudioProcessor::lfo(float phase, int waveform)
{
    switch(waveform)
    {
        case kWaveformTriangle:
            if(phase < 0.25f)
                return 0.5f + 2.0f*phase;
            else if(phase < 0.75f)
                return 1.0f - 2.0f*(phase - 0.25f);
            else
                return 2.0f*(phase-0.75f);
        case kWaveformSquare:
            if(phase < 0.5f)
                return 1.0f;
            else
                return 0.0f;
        case kWaveformSawtooth:
            if(phase < 0.5f)
                return 0.5f + phase;
            else
                return phase - 0.5f;
        case kWaveformSine:
        default:
            return 0.5f + 0.5f*sinf(2.0 * M_PI * phase);
    }
}
//==============================================================================
void PhaserAudioProcessor::allocateFilters()
{
    // Create any filters we need; depends on number of channels and number of
    // filters per channel
    totalNumFilters_ = getTotalNumInputChannels() * filtersPCH;
    if(totalNumFilters_ > 0) {
        allpassFilters_ = (OnePoleAllpassFilter**)malloc(totalNumFilters_ * sizeof(OnePoleAllpassFilter*));
        if(allpassFilters_ == 0)
            totalNumFilters_ = 0;
        else {
            for(int i = 0; i < totalNumFilters_; i++)
                allpassFilters_[i] = new OnePoleAllpassFilter;
        }
    }
    
    numLastFilterOutputs_ = getTotalNumInputChannels();
    lastFilterOutputs_ = (float *)malloc(numLastFilterOutputs_ * sizeof(float));
    for(int i = 0; i < numLastFilterOutputs_; i++)
        lastFilterOutputs_[i] = 0.0f;
    
    // Coefficients of allpass filters will get updated in processBlock()
}
//==============================================================================
void PhaserAudioProcessor::deallocateFilters()
{
    // Release the filters that were created in prepareToPlay()
    
    for(int i = 0; i < totalNumFilters_; i++)
        delete allpassFilters_[i];
    if(totalNumFilters_ != 0)
        free(allpassFilters_);
    totalNumFilters_ = 0;
    allpassFilters_ = 0;
    
    if(numLastFilterOutputs_ != 0)
        free(lastFilterOutputs_);
    numLastFilterOutputs_ = 0;
    lastFilterOutputs_ = 0;
}
//==============================================================================
// Release and recreate the filters in one atomic operation:
// the ScopedLock will not let the audio thread run between
// release and allocation
void PhaserAudioProcessor::reallocateFilters()
{
    const ScopedLock sl (lock_);
    deallocateFilters();
    allocateFilters();
}
//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PhaserAudioProcessor();
}
