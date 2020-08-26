/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <math.h>

const float FlanGerrAudioProcessor::kMaximumDelay = 0.02;
const float FlanGerrAudioProcessor::kMaximumSweepWidth = 0.02;

String FlanGerrAudioProcessor::delaY ("delay");
String FlanGerrAudioProcessor::sweeP ("sweepwidth");
String FlanGerrAudioProcessor::deptH ("depth");
String FlanGerrAudioProcessor::feeD ("feedback");
String FlanGerrAudioProcessor::freQ ("frequency");
String FlanGerrAudioProcessor::waveF ("waveform");
String FlanGerrAudioProcessor::interP ("interpolation");
String FlanGerrAudioProcessor::stereO ("stereo");

//==============================================================================
FlanGerrAudioProcessor::FlanGerrAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),state(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    
}

FlanGerrAudioProcessor::~FlanGerrAudioProcessor()
{
}
AudioProcessorValueTreeState::ParameterLayout FlanGerrAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<RangedAudioParameter>> params;
    
    
    
    const StringArray waveformArr ({"Sine", "Triangle", "Square", "Sawtooth"});
    const StringArray interArr ({"Nonen", "Linear","Cubic" });
    const StringArray stereoArr ({"Stereo", "Mono"});
    
    auto delayParam = std::make_unique<AudioParameterFloat>(delaY, TRANS ("Delay"), 0.001f, kMaximumDelay,0.005f);
    auto sweepParam = std::make_unique<AudioParameterFloat>(sweeP, TRANS ("Sweep"), 0.001f, kMaximumSweepWidth,0.005f);
    auto depthParam = std::make_unique<AudioParameterFloat>(deptH, TRANS ("Depth"), 0.001f,1.00f,0.001f);
    auto feedParam = std::make_unique<AudioParameterFloat>(feeD, TRANS ("Feedback"),0.001f,1.00f,0.00f);
    auto freqParam = std::make_unique<AudioParameterFloat>(freQ, TRANS ("Frequency"),0.005f,2.00f,0.025f);
    auto waveParam = std::make_unique<AudioParameterChoice>(waveF, TRANS ("Waveform"),waveformArr,4.0);
    auto interParam = std::make_unique<AudioParameterChoice>(interP, TRANS ("Interpolation"), interArr,3.0);
    auto stereParam = std::make_unique<AudioParameterChoice>(stereO,TRANS ("Stereo"), stereoArr,1.0);
    
    
    
    params.push_back(std::move(delayParam));
    params.push_back(std::move(sweepParam));
    params.push_back(std::move(depthParam));
    params.push_back(std::move(feedParam));
    params.push_back(std::move(freqParam));
    params.push_back(std::move(waveParam));
    params.push_back(std::move(interParam));
    params.push_back(std::move(stereParam));
    
    return { params.begin(), params.end() };
}
//==============================================================================
const String FlanGerrAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FlanGerrAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FlanGerrAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FlanGerrAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FlanGerrAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FlanGerrAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FlanGerrAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FlanGerrAudioProcessor::setCurrentProgram (int index)
{
}

const String FlanGerrAudioProcessor::getProgramName (int index)
{
    return {};
}

void FlanGerrAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void FlanGerrAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    delayBufferLength_ = (int)((kMaximumDelay + kMaximumSweepWidth)*sampleRate) + 3;
    delayBuffer_.setSize(2, delayBufferLength_);
    delayBuffer_.clear();
    lfoPhase_ = 0.0;
    
    inverseSampleRate_ = 1.0/sampleRate;

}
void FlanGerrAudioProcessor::reset()
{
    delayBuffer_.clear();
}

void FlanGerrAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FlanGerrAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void FlanGerrAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto numSamples = buffer.getNumSamples();
    
    int channel, dpw; // dpr = delay read pointer; dpw = delay write pointer
    float dpr, currentDelay, ph;
    float channel0EndPhase = lfoPhase_;
    
    float dl = delay_;
    float sw = sweepWidth_;
    float dpt = depth_;
    float fb = feedback_;
    float fq = frequency_;
    
    for (channel = 0; channel < totalNumInputChannels; ++channel)
    {
        // channelData is an array of length numSamples which contains the audio for one channel
        float* channelData = buffer.getWritePointer(channel);
        
        // delayData is the circular buffer for implementing delay on this channel
        float* delayData = delayBuffer_.getWritePointer (jmin (channel, delayBuffer_.getNumChannels() - 1));
        
        // Make a temporary copy of any state variables declared in PluginProcessor.h which need to be
        // maintained between calls to processBlock(). Each channel needs to be processed identically
        // which means that the activity of processing one channel can't affect the state variable for
        // the next channel.
        
        dpw = delayWritePosition_;
        ph = lfoPhase_;
        
        // For stereo flanging, keep the channels 90 degrees out of phase with each other
        if(stereoCH != 0 && channel != 0)
            ph = fmodf(ph + 0.25, 1.0);
        
        for (int i = 0; i < numSamples; ++i)
        {
            const float in = channelData[i];
            float interpolatedSample = 0.0;
            
            // Recalculate the read pointer position with respect to the write pointer. A more efficient
            // implementation might increment the read pointer based on the derivative of the LFO without
            // running the whole equation again, but this format makes the operation clearer.
            
            currentDelay = dl + sw * lfo(ph, waveCH);
            dpr = fmodf((float)dpw - (float)(currentDelay * getSampleRate()) + (float)delayBufferLength_,
                        (float)delayBufferLength_);
            
            // In this example, the output is the input plus the contents of the delay buffer (weighted by delayMix)
            // The last term implements a tremolo (variable amplitude) on the whole thing.
            
            if(interCH == kInterpolationLinear)
            {
                // Find the fraction by which the read pointer sits between two
                // samples and use this to adjust weights of the samples
                float fraction = dpr - floorf(dpr);
                int previousSample = (int)floorf(dpr);
                int nextSample = (previousSample + 1) % delayBufferLength_;
                interpolatedSample = fraction*delayData[nextSample]
                + (1.0f-fraction)*delayData[previousSample];
            }
            else if(interCH == kInterpolationCubic)
            {
                // Cubic interpolation will produce cleaner results at the expense
                // of more computation. This code uses the Catmull-Rom variant of
                // cubic interpolation. To reduce the load, calculate a few quantities
                // in advance that will be used several times in the equation:
                
                int sample1 = (int)floorf(dpr);
                int sample2 = (sample1 + 1) % delayBufferLength_;
                int sample3 = (sample2 + 1) % delayBufferLength_;
                int sample0 = (sample1 - 1 + delayBufferLength_) % delayBufferLength_;
                
                float fraction = dpr - floorf(dpr);
                float frsq = fraction*fraction;
                
                float a0 = -0.5f*delayData[sample0] + 1.5f*delayData[sample1]
                - 1.5f*delayData[sample2] + 0.5f*delayData[sample3];
                float a1 = delayData[sample0] - 2.5f*delayData[sample1]
                + 2.0f*delayData[sample2] - 0.5f*delayData[sample3];
                float a2 = -0.5f*delayData[sample0] + 0.5f*delayData[sample2];
                float a3 = delayData[sample1];
                
                interpolatedSample = a0*fraction*frsq + a1*frsq + a2*fraction + a3;
            }
            else // Nearest neighbour interpolation
            {
                // Find the nearest input sample by rounding the fractional index to the
                // nearest integer. It's possible this will round it to the end of the buffer,
                // in which case we need to roll it back to the beginning.
                int closestSample = (int)floorf(dpr + 0.5f);
                if(closestSample == delayBufferLength_)
                    closestSample = 0;
                interpolatedSample = delayData[closestSample];
            }
            
            // Store the current information in the delay buffer. With feedback, what we read is
            // included in what gets stored in the buffer, otherwise it's just a simple delay line
            // of the input signal.
            
            delayData[dpw] = in + (interpolatedSample * fb);
            
            // Increment the write pointer at a constant rate. The read pointer will move at different
            // rates depending on the settings of the LFO, the delay and the sweep width.
            
            if (++dpw >= delayBufferLength_)
                dpw = 0;
            
            // Store the output sample in the buffer, replacing the input
            channelData[i] = in + dpt * interpolatedSample;
            
            // Update the LFO phase, keeping it in the range 0-1
            ph += fq*inverseSampleRate_;
            if(ph >= 1.0)
                ph -= 1.0;
        }
        
        // Use channel 0 only to keep the phase in sync between calls to processBlock()
        // Otherwise quadrature phase on multiple channels will create problems.
        if(channel == 0)
            channel0EndPhase = ph;
    }
    
    // Having made a local copy of the state variables for each channel, now transfer the result
    // back to the main state variable so they will be preserved for the next call of processBlock()
    
    delayWritePosition_ = dpw;
    lfoPhase_ = channel0EndPhase;
    
    //==================================
    // V U  M E T E R
    meterSourceL.measureBuffer(buffer);
    meterSourceR.measureBuffer(buffer);

    
    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
}

//==============================================================================
bool FlanGerrAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* FlanGerrAudioProcessor::createEditor()
{
    return new FlanGerrAudioProcessorEditor (*this);
}

//==============================================================================
void FlanGerrAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // Create an outer XML element..
    std::unique_ptr<XmlElement> xml (state.state.createXml());
    copyXmlToBinary(*xml, destData);
}

void FlanGerrAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // This getXmlFromBinary() helper function retrieves our XML from the binary blob..
    std::unique_ptr<XmlElement> theParams(getXmlFromBinary(data, sizeInBytes));
    
    if (theParams != nullptr)
    {
        if (theParams->hasTagName(state.state.getType()))
        {
            state.state = ValueTree::fromXml(*theParams);
        }
    }
}
//==============================================================================
// Function for calculating LFO waveforms. Phase runs from 0-1, output is scaled
// from 0 to 1 (note: not -1 to 1 as would be typical of sine).
float FlanGerrAudioProcessor::lfo(float phase, int waveform)
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
//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FlanGerrAudioProcessor();
}
