/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

String DistortionAudioProcessor::drivE ("drive_");
String DistortionAudioProcessor::rangE ("range_");
String DistortionAudioProcessor::blenD ("blend_");
String DistortionAudioProcessor::volumE ("volume_");

//==============================================================================
DistortionAudioProcessor::DistortionAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
                        state(*this, nullptr, "PARAMETERS", createParameterLayout())
                       
                       
#endif
{
    //ANALYSER
    frequencies.resize (160);
    for (size_t i=0; i < frequencies.size(); ++i) {
        frequencies [i] = 20.0 * std::pow (2.0, i / 30.0);
    }
    magnitudes.resize (frequencies.size());
    
}

DistortionAudioProcessor::~DistortionAudioProcessor()
{
   inputAnalyser.stopThread (1000);
   outputAnalyser.stopThread (1000);
}

AudioProcessorValueTreeState::ParameterLayout DistortionAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<RangedAudioParameter>> params;
    
    auto volumeParam = std::make_unique<AudioParameterFloat>(volumE, TRANS ("Volume"), 0.0f, 1.0f, 0.0001f);
    auto rangeParam = std::make_unique<AudioParameterFloat>(rangE, TRANS ("Range"), 0.0f,300.0f,0.0001f);
    auto blendParam = std::make_unique<AudioParameterFloat>(blenD, TRANS ("Blend"), 0.0f, 1.0, 0.0001);
    auto driveParam = std::make_unique<AudioParameterFloat>(drivE, TRANS ("Drive"), 0.0f,3.f, 0.0001f);


    
    params.push_back(std::move(volumeParam));
    params.push_back(std::move(rangeParam));
    params.push_back(std::move(blendParam));
    params.push_back(std::move(driveParam));
    
    return { params.begin(), params.end() };
}


//==============================================================================
const String DistortionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DistortionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DistortionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DistortionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DistortionAudioProcessor::getTailLengthSeconds() const{return 0.0;}
//============================================================
int DistortionAudioProcessor::getNumPrograms(){ return 1; }
//============================================================
int DistortionAudioProcessor::getCurrentProgram(){return 0;}
//============================================================
void DistortionAudioProcessor::setCurrentProgram (int index){}
//=============================================================
const String DistortionAudioProcessor::getProgramName (int index){return {};}
//============================================================================
void DistortionAudioProcessor::changeProgramName (int index, const String& newName){}
//===========================================================================================

//==============================================================================
void DistortionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
    lastSampleRate = sampleRate;
    dsp::ProcessSpec spec;
    spec.sampleRate = lastSampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    //ANALYSER
    updatePlots();
    gaIn.prepare(spec);
    inputAnalyser.setupAnalyser  (int (sampleRate), float (sampleRate));
    outputAnalyser.setupAnalyser (int (sampleRate), float (sampleRate));
}

void DistortionAudioProcessor::releaseResources()
{
    inputAnalyser.stopThread (1000);
    outputAnalyser.stopThread (1000);
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DistortionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

//===============================================
// P R O C E S S   B L O C K
void DistortionAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    
    float drive = *state.getRawParameterValue(drivE);
    float range = *state.getRawParameterValue(rangE);
	float blend = *state.getRawParameterValue(blenD);
	float volume = *state.getRawParameterValue(volumE);
	
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
       
        
        for (int sample = 0; sample < buffer.getNumSamples(); sample++) 
        {
			
			float cleanData = *channelData;

			*channelData *= drive * range;
			
			*channelData = (((((2.f / float_Pi) * atan(*channelData)) * blend) + (cleanData * (1.f - blend))) / 2.f) * volume;

			channelData++;
            
		}
    
    }
    
    //==================================
    // V U  M E T E R
    meterSourceL.measureBuffer(buffer);
    meterSourceR.measureBuffer(buffer);
    //A N A L Y S E R
    inputAnalyser.addAudioData (buffer, 0, getTotalNumInputChannels());
    outputAnalyser.addAudioData (buffer, 0, getTotalNumOutputChannels());
    //==========================================================================
  
}
//==============================================================================
//BEGIN ANALYSER FUNCTIONS
//==============================================================================

const std::vector<double>& DistortionAudioProcessor::getMagnitudes ()
{
    return magnitudes;
}
//====================================================================
void DistortionAudioProcessor::createFrequencyPlot (Path& p, const std::vector<double>& mags, const Rectangle<int> bounds, float pixelsPerDouble)
{
    p.startNewSubPath (bounds.getX(), roundToInt (bounds.getCentreY() - pixelsPerDouble * std::log (mags [0]) / std::log (2)));
    const double xFactor = static_cast<double> (bounds.getWidth()) / frequencies.size();
    for (size_t i=1; i < frequencies.size(); ++i)
    {
        p.lineTo (roundToInt (bounds.getX() + i * xFactor),
                  roundToInt (bounds.getCentreY() - pixelsPerDouble * std::log (mags [i]) / std::log (2)));
    }
}
//==============================================================================
void DistortionAudioProcessor::createAnalyserPlot (Path& p, const Rectangle<int> bounds, float minFreq, bool input)
{
    if (input)
        inputAnalyser.createPath (p, bounds.toFloat(), minFreq);
    else
        outputAnalyser.createPath (p, bounds.toFloat(), minFreq);
}
//==============================================================================
bool DistortionAudioProcessor::checkForNewAnalyserData()
{
    return inputAnalyser.checkForNewData() || outputAnalyser.checkForNewData();
}
//==============================================================================
void DistortionAudioProcessor::updatePlots ()
{
    
    auto gain = gaIn.getGainLinear();
    std::fill (magnitudes.begin(), magnitudes.end(), gain);

    if (isPositiveAndBelow (soloed, bands.size())) {
        FloatVectorOperations::multiply (magnitudes.data(), bands [size_t (soloed)].magnitudes.data(), static_cast<int> (magnitudes.size()));
    }
    else
    {
        for (size_t i=0; i < bands.size(); ++i)
            if (bands[i].active)
                FloatVectorOperations::multiply (magnitudes.data(), bands [i].magnitudes.data(), static_cast<int> (magnitudes.size()));
    }

    sendChangeMessage();
}
//==============================================================================
//END ANALYSER FUNCTIONS
//==============================================================================

bool DistortionAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DistortionAudioProcessor::createEditor()
{
    return new DistortionAudioProcessorEditor (*this);
}
//==============================================================================
void DistortionAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // Create an outer XML element..
    std::unique_ptr<XmlElement> xml (state.state.createXml());
    copyXmlToBinary(*xml, destData);
}

void DistortionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistortionAudioProcessor();
}
