/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

String StereoWidenerAudioProcessor::widtH ("width");
String StereoWidenerAudioProcessor::gaiN ("gain");

//==============================================================================
StereoWidenerAudioProcessor::StereoWidenerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
                       parameterState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    

   
}
//==============================================================================
StereoWidenerAudioProcessor::~StereoWidenerAudioProcessor()
{
   
}
//==============================================================================
AudioProcessorValueTreeState::ParameterLayout StereoWidenerAudioProcessor::createParameterLayout()
{
	std::vector <std::unique_ptr<RangedAudioParameter>> params;

	auto gainParam = std::make_unique<AudioParameterFloat>(gaiN, TRANS ("Gain"), -48.0f, 0.0f, -15.0f);
    auto widthParam = std::make_unique<AudioParameterFloat>(widtH, TRANS ("Width"), 0.0f,2.0f,0.1f);
	
	params.push_back(std::move(gainParam));
    params.push_back(std::move(widthParam));

	return { params.begin(), params.end() };
}

//==============================================================================
const String StereoWidenerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool StereoWidenerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool StereoWidenerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool StereoWidenerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double StereoWidenerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int StereoWidenerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int StereoWidenerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void StereoWidenerAudioProcessor::setCurrentProgram (int index)
{
}

const String StereoWidenerAudioProcessor::getProgramName (int index)
{
    return {};
}

void StereoWidenerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void StereoWidenerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void StereoWidenerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool StereoWidenerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void StereoWidenerAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        
        float* leftChannel = buffer.getWritePointer(0);
		float* rightChannel = buffer.getWritePointer(getTotalNumInputChannels() < 2 ? 0 : 1);
       
        
        float width_ = *parameterState.getRawParameterValue(widtH);
        
        for (int i = 0; i < buffer.getNumSamples(); ++i)
		{
			
            //float widthVal = width_ * 0.5;
            float tmp = 1 / fmax((1 + width_), 2);
			float coef_M = 1 * tmp;
            float coef_S = width_ * tmp;

			float mid = (leftChannel[i] + rightChannel[i])*coef_M;
			float sides = (rightChannel[i] - leftChannel[i])*coef_S;

			leftChannel[i] = mid - sides;
			rightChannel[i] = mid + sides;
            
		}
        
	
    }
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        float gain_ = *parameterState.getRawParameterValue(gaiN);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] = buffer.getSample(channel, sample) * Decibels::decibelsToGain(gain_);
        }
        
        meterSource.measureBuffer(buffer);
    }
}

//==============================================================================
bool StereoWidenerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* StereoWidenerAudioProcessor::createEditor()
{
    return new StereoWidenerAudioProcessorEditor (*this);
}
//==============================================================================
//==============================================================================
void StereoWidenerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    std::unique_ptr<XmlElement> xml (parameterState.state.createXml());
	copyXmlToBinary(*xml, destData);
}

void StereoWidenerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
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
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StereoWidenerAudioProcessor();
}
