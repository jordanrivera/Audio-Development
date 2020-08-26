/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "AboutComponent.h"
#include "Analyser.h"
#include <array>
#include <numeric>
#include <algorithm>
static float maxDB  = 24.0f;
//==============================================================================
DistortionAudioProcessorEditor::DistortionAudioProcessorEditor (DistortionAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    
    
    //background image
    backgroundimg = ImageFileFormat::loadFrom(BinaryData::background_png,BinaryData::background_pngSize);
    
    tooltipWindow->setMillisecondsBeforeTipAppears (1000);
    updateFrequencyResponses();
    processor.addChangeListener (this);
    
    driveKnob.reset(new Slider("Drive"));
    addAndMakeVisible(driveKnob.get());
	driveKnob->setSliderStyle(Slider::Rotary);
	driveKnob->setTextBoxStyle(Slider::NoTextBox, false, 110, 110);

    rangeKnob.reset(new Slider("Range"));
    addAndMakeVisible(rangeKnob.get());
	rangeKnob->setSliderStyle(Slider::Rotary);
	rangeKnob->setTextBoxStyle(Slider::NoTextBox, false, 110, 110);

    blendKnob.reset(new Slider("Blend"));
    addAndMakeVisible(blendKnob.get());
	blendKnob->setSliderStyle(Slider::Rotary);
	blendKnob->setTextBoxStyle(Slider::NoTextBox, false, 110, 110);

    volumeKnob.reset(new Slider("Volume"));
    addAndMakeVisible(volumeKnob.get());
	volumeKnob->setSliderStyle(Slider::Rotary);
	volumeKnob->setTextBoxStyle(Slider::NoTextBox, false, 110, 110);
    
    driveAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.state, DistortionAudioProcessor::drivE, *driveKnob);
    rangeAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.state, DistortionAudioProcessor::rangE, *rangeKnob);
    blendAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.state, DistortionAudioProcessor::blenD, *blendKnob);
    volumeAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.state, DistortionAudioProcessor::volumE, *volumeKnob);
   
    
	driveKnob->setLookAndFeel(&widbothKnobLookAndFeel);
	rangeKnob->setLookAndFeel(&widbothKnobLookAndFeel);
	blendKnob->setLookAndFeel(&widbothKnobLookAndFeel);
	volumeKnob->setLookAndFeel(&widbothKnobLookAndFeel);

	//VU  Meter
    levelMeterL.reset (new VUMeterComponent (processor.meterSourceL));
    levelMeterR.reset (new VUMeterComponent(processor.meterSourceR));
    addAndMakeVisible (levelMeterL.get());
    addAndMakeVisible(levelMeterR.get());
    levelMeterL->setName ("new componentL");
    levelMeterR->setName ("new componentR");
    //==============================================================================
    resizer.reset(new ResizableCornerComponent (this, &resizeLimits));
    resizeLimits.setSizeLimits (125, 175, 650, 520);
     //=============================================================================
    // A B O U T  W I N D O W
    aboutButton.reset(new ImageButton ("aboutButton"));
    addAndMakeVisible (aboutButton.get());
    aboutButton->setButtonText (String());
    aboutButton->addListener (this);
    
    aboutButton->setImages (false, true, true,
                            Image(), 1.000f, Colour (0x00000000),
                            Image(), 1.000f, Colour (0x00000000),
                            Image(), 1.000f, Colour (0x00000000));
    aboutComp = new AboutComponent();
    //=============================================================================
    //---OPEN GL-------
    #ifdef JUCE_OPENGL
    openGLContext.attachTo (*this);
    #endif
    //=============================================================================
    //=============================================================================
   //HYPERLINKS
    addAndMakeVisible(hyperlinkshop = new HyperlinkButton(" ",URL("https://www.audiopluginshop.com")));
    addAndMakeVisible(hyperlinklinked = new HyperlinkButton(" ", URL ("https://nl.linkedin.com/in/yordan-adonias-rivera-rios-1a49b749"))); 
   //=============================================================================
    setSize (650, 420);
    startTimer (30);
}

DistortionAudioProcessorEditor::~DistortionAudioProcessorEditor()
{   
    processor.removeChangeListener(this);
    aboutButton = nullptr;
    blendAttachment = nullptr;
    blendKnob = nullptr;
    driveAttachment = nullptr;
    driveKnob = nullptr;
    levelMeterL = nullptr;
    levelMeterR = nullptr;
    rangeAttachment = nullptr;
    rangeKnob = nullptr;
    volumeAttachment = nullptr;
    volumeKnob = nullptr;
    #ifdef JUCE_OPENGL
    openGLContext.detach();
    #endif
    
}

//==============================================================================
void DistortionAudioProcessorEditor::paint (Graphics& g)
{
    
    
     // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    int x = 1, y = 1, width = 650, heigth = 420;
    g.drawImage(backgroundimg,x,y,width,heigth,0,0,backgroundimg.getWidth(),backgroundimg.getHeight());

    //-------------------------------------------------
    //BEGIN ANALYSER
    const Colour inputColour = Colours::darkblue ;
    //const Colour outputColour = Colours::indianred;
    
    //ANALYSER RAND
    g.setFont (12.0f);
    g.setColour (Colours::silver );
    g.drawRoundedRectangle (plotFrame.toFloat(), 5, 2);
    
    for (int i=0; i < 10; ++i)
    {
        
        //ANALYSER 10 STREPEN LINK HORIZONTAL
        g.setColour (Colours::black);
        auto x = plotFrame.getX() + plotFrame.getWidth() * i * 0.1f;
        if (i > 0) g.drawVerticalLine (roundToInt (x), plotFrame.getY(), plotFrame.getBottom());

        //ANALYSER NUMMERS ONDER HORIZONTAL
        g.setColour (Colours::red);
        auto freq = getFrequencyForPosition (i * 0.1f);
        g.setFont(8.0f);
        g.drawFittedText ((freq < 1000) ? String (freq) + " Hz" : String (freq / 1000, 1) + " kHz",
                          roundToInt (x + 3), plotFrame.getBottom() - 18, 50, 15, Justification::left, 1);
    }
    //ANALYSER 2 STREPEN LINK VERTICAL
    g.setColour (Colours::black);
    g.drawHorizontalLine (roundToInt (plotFrame.getY() + 0.25 * plotFrame.getHeight()), plotFrame.getX(), plotFrame.getRight());
    g.drawHorizontalLine (roundToInt (plotFrame.getY() + 0.75 * plotFrame.getHeight()), plotFrame.getX(), plotFrame.getRight());
    
    //ANALYSER NUMMERS VERTICAL
    g.setColour (Colours::yellow);
    g.drawFittedText (String (maxDB) + " dB", plotFrame.getX() + 3, plotFrame.getY() + 2, 50, 14, Justification::left, 1);
    g.drawFittedText (String (maxDB / 2) + " dB", plotFrame.getX() + 3, roundToInt (plotFrame.getY() + 2 + 0.25 * plotFrame.getHeight()), 50, 14, Justification::left, 1);
    g.drawFittedText (" 0 dB", plotFrame.getX() + 3, roundToInt (plotFrame.getY() + 2 + 0.5  * plotFrame.getHeight()), 50, 14, Justification::left, 1);
    g.drawFittedText (String (- maxDB / 2) + " dB", plotFrame.getX() + 3, roundToInt (plotFrame.getY() + 2 + 0.75 * plotFrame.getHeight()), 50, 14, Justification::left, 1);
   
    g.reduceClipRegion (plotFrame);
    
    g.setFont (16.0f);
    // ANALYSER SELF
    processor.createAnalyserPlot (analyserPath, plotFrame, 20.0f, true);
    g.setColour (inputColour);
    g.strokePath (analyserPath, PathStrokeType (1.0));
    g.strokePath (frequencyResponse, PathStrokeType (1.0));
    //-------------------------------------------------------
    //END ANALYSER

}
//==============================================================================
void DistortionAudioProcessorEditor::updateFrequencyResponses ()
{
    auto pixelsPerDouble = 2.0f * plotFrame.getHeight() / Decibels::decibelsToGain (maxDB);
    
    frequencyResponse.clear();
    processor.createFrequencyPlot (frequencyResponse, processor.getMagnitudes(), plotFrame, pixelsPerDouble);
}
//======================================================================================
void DistortionAudioProcessorEditor::changeListenerCallback (ChangeBroadcaster* sender)
{
    ignoreUnused (sender);
    updateFrequencyResponses();
    repaint();
}
//==============================================================================
float DistortionAudioProcessorEditor::getFrequencyForPosition (float pos)
{
    return 20.0f * std::pow (2.0f, pos * 10.0f);
}
//==============================================================================
float DistortionAudioProcessorEditor::getPositionForFrequency (float freq)
{
    return (std::log (freq / 20.0f) / std::log (2.0f)) / 10.0f;
}
//==============================================================================
float DistortionAudioProcessorEditor::getPositionForGain (float gain, float top, float bottom)
{
    return jmap (Decibels::gainToDecibels (gain, -maxDB), -maxDB, maxDB, bottom, top);
}
//==============================================================================
float DistortionAudioProcessorEditor::getGainForPosition (float pos, float top, float bottom)
{
    return Decibels::decibelsToGain (jmap (pos, bottom, top, -maxDB, maxDB), -maxDB);
}
void DistortionAudioProcessorEditor::resized()
{
   // LEVEL VU METER 
   levelMeterL->setBounds(145,10,180,160);
   levelMeterR->setBounds(330,10,180,160);
   // KNOBS 
   driveKnob->setBounds(20, 30, 110, 110);
   blendKnob->setBounds(20, 210, 110, 110);
    rangeKnob->setBounds(525, 30, 110, 110);
   volumeKnob->setBounds(525, 210, 110, 110);
   //=============================================================================
   //S P E C T R U M  A N A L I Z E R
    plotFrame.setBounds(150, 185 , 355, 160); //= getLocalBounds().reduced (3, 3);
    plotFrame.reduce (3, 3);
    updateFrequencyResponses();
   //=============================================================================
   // A B O U T  W I N D O W
   aboutButton->setBounds (200,390,30,50);
   //=============================================================================
   //HYPERLINKS
   hyperlinkshop->setBounds(550,390,30,20);
   hyperlinklinked->setBounds(450, 390, 30, 20);
   //=============================================================================
    
   
}


//=================================================================================
void DistortionAudioProcessorEditor::buttonClicked (Button* buttonThatWasClicked)
{
    
    if (buttonThatWasClicked == &*aboutButton)
    {
        DialogWindow::showModalDialog("About", aboutComp, this, Colours::black, true);
    }
    
}
//==============================================================================
void DistortionAudioProcessorEditor::timerCallback ()
{
   // meter->addToBuffer(processor.xMeter);
    repaint ();
    //=======================================
    //ANALYSER
    if (processor.checkForNewAnalyserData())
        repaint (plotFrame);
}







