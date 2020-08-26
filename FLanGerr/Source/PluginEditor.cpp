/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "AboutComponent.h"
#include <array>
#include <numeric>
#include <algorithm>

//==============================================================================
FlanGerrAudioProcessorEditor::FlanGerrAudioProcessorEditor (FlanGerrAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    
   
    
    sliderAttachD = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.state,FlanGerrAudioProcessor::delaY, delaySlider_);
    sliderAttachS = std::make_unique <AudioProcessorValueTreeState::SliderAttachment>(processor.state,FlanGerrAudioProcessor::sweeP, sweepWidthSlider_);
    sliderAttachDP = std::make_unique <AudioProcessorValueTreeState::SliderAttachment>(processor.state, FlanGerrAudioProcessor::deptH, depthSlider_);
    sliderAttachF = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.state, FlanGerrAudioProcessor::feeD, feedbackSlider_);
    sliderAttachFQ = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.state,FlanGerrAudioProcessor::freQ, frequencySlider_);
    
    ComboBoxAttach = std::make_unique <AudioProcessorValueTreeState::ComboBoxAttachment>(processor.state,FlanGerrAudioProcessor::waveF,waveformComboBox_);
    ComboBoxAttach2 = std::make_unique <AudioProcessorValueTreeState::ComboBoxAttachment>(processor.state,FlanGerrAudioProcessor::interP,interpolationComboBox_);
    buttonAttach = std::make_unique <AudioProcessorValueTreeState::ComboBoxAttachment>(processor.state,FlanGerrAudioProcessor::stereO,stereoToggleButton_);
    
    //================Slider
	addAndMakeVisible(delaySlider_);
	delaySlider_.setRange(0.0001,FlanGerrAudioProcessor::kMaximumDelay, 0.0005);
	delaySlider_.setSliderStyle(Slider::Rotary);
	delaySlider_.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	delaySlider_.setPopupDisplayEnabled(true, false, this);
	delaySlider_.addListener(this);
    
    addAndMakeVisible(sweepWidthSlider_);
	sweepWidthSlider_.setRange(0.001, FlanGerrAudioProcessor::kMaximumSweepWidth, 0.0005);
	sweepWidthSlider_.setSliderStyle(Slider::Rotary);
	sweepWidthSlider_.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	sweepWidthSlider_.setPopupDisplayEnabled(true, false, this);
	sweepWidthSlider_.addListener(this);
	
	addAndMakeVisible(depthSlider_);
	depthSlider_.setRange(0.0, 1.0, 0.01);
	depthSlider_.setSliderStyle(Slider::Rotary);
	depthSlider_.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	depthSlider_.setPopupDisplayEnabled(true, false, this);
	depthSlider_.addListener(this);
    
    addAndMakeVisible(feedbackSlider_);
	feedbackSlider_.setRange(0.0, 0.5, 0.01);
	feedbackSlider_.setSliderStyle(Slider::Rotary);
	feedbackSlider_.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	feedbackSlider_.setPopupDisplayEnabled(true, false, this);
	feedbackSlider_.addListener(this);
	
	addAndMakeVisible(frequencySlider_);
	frequencySlider_.setRange(0.0, 2.0, 0.025);
	frequencySlider_.setSliderStyle(Slider::Rotary);
	frequencySlider_.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
	frequencySlider_.setPopupDisplayEnabled(true, false, this);
	frequencySlider_.addListener(this);
    
	//===============BOX
    addAndMakeVisible(&waveformComboBox_);
    waveformComboBox_.setEditableText(false);
    waveformComboBox_.setJustificationType(Justification::left);
    waveformComboBox_.addItemList({"Sine", "Triangle", "Square", "Sawtooth"},1);
    waveformComboBox_.setSelectedId(1);
    waveformComboBox_.addListener(this);
    
    addAndMakeVisible(&interpolationComboBox_);
    interpolationComboBox_.setEditableText(false);
    interpolationComboBox_.setJustificationType(Justification::left);
    interpolationComboBox_.addItemList({"None", "Linear", "Cubic"},1);
    interpolationComboBox_.setSelectedId(1);
    interpolationComboBox_.addListener(this);
    
    addAndMakeVisible(&stereoToggleButton_);
    stereoToggleButton_.addItemList({"Stereo", "Mono"},1);
    stereoToggleButton_.setSelectedId(2);
    stereoToggleButton_.addListener(this);
    
    //Set Look & feel
    delaySlider_.setLookAndFeel(&widbothKnobLookAndFeel);
    sweepWidthSlider_.setLookAndFeel(&widbothKnobLookAndFeel);
    depthSlider_.setLookAndFeel(&widbothKnobLookAndFeel);
    feedbackSlider_.setLookAndFeel(&widbothKnobLookAndFeel);
    frequencySlider_.setLookAndFeel(&widbothKnobLookAndFeel);
    
    //VU  Meter
    levelMeterL.reset (new VUMeterComponent (processor.meterSourceL));
    levelMeterR.reset (new VUMeterComponent(processor.meterSourceR));
    addAndMakeVisible (levelMeterL.get());
    addAndMakeVisible(levelMeterR.get());
    levelMeterL->setName ("new componentL");
    levelMeterR->setName ("new componentR");
    //==============================================================================
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
    //=============================================================================
    //HYPERLINKS
    addAndMakeVisible(hyperlinkshop = new HyperlinkButton(" ",URL("https://www.audiopluginshop.com")));
    addAndMakeVisible(hyperlinklinked = new HyperlinkButton(" ", URL ("https://nl.linkedin.com/in/yordan-adonias-rivera-rios-1a49b749")));
    //=============================================================================
    setSize (630, 500);
    startTimer(30);
}
//===========================================================
FlanGerrAudioProcessorEditor::~FlanGerrAudioProcessorEditor()
{
    buttonAttach = nullptr;
    aboutButton = nullptr;
    levelMeterL = nullptr;
    levelMeterR = nullptr;
    ComboBoxAttach = nullptr;
    ComboBoxAttach2 = nullptr;
    sliderAttachD = nullptr;
    sliderAttachS = nullptr;
    sliderAttachDP = nullptr;
    sliderAttachF = nullptr;
    sliderAttachFQ = nullptr;
}
//===================================================================
void FlanGerrAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    
    if (slider == &delaySlider_)
	{
		processor.delay_ = (float)delaySlider_.getValue();
	}
    if (slider == &sweepWidthSlider_)
	{
        processor.sweepWidth_= (float)sweepWidthSlider_.getValue();
	}
	
    if (slider == &depthSlider_)
	{
		processor.depth_= (float)depthSlider_.getValue();
	}
	if (slider == &feedbackSlider_)
	{
		processor.feedback_ = (float)feedbackSlider_.getValue();
	}
	if (slider == &frequencySlider_)
	{
		processor.frequency_ = (float)frequencySlider_.getValue();
	}

}
//==============================================================================
void FlanGerrAudioProcessorEditor::paint (Graphics& g)
{
   g.fillAll (Colours::grey);
}
//===========================================
void FlanGerrAudioProcessorEditor::resized()
{
    // LEVEL VU METER
    levelMeterL->setBounds(125,10,180,160);
    levelMeterR->setBounds(320,10,180,160);
    
    delaySlider_.setBounds(125, 200, 100, 100);
    sweepWidthSlider_.setBounds(125, 310, 100, 100);
    depthSlider_.setBounds(265, 200, 100, 100);
    feedbackSlider_.setBounds(395, 310, 100, 100);
    frequencySlider_.setBounds(395, 200, 100, 100);
    
    waveformComboBox_.setBounds(125, 430, 100, 30);
    interpolationComboBox_.setBounds(400, 430, 100, 30);
    stereoToggleButton_.setBounds(265, 320, 100, 30);
    
    // A B O U T  W I N D O W
    aboutButton->setBounds (320,400,20,20);
    //=============================================================================
    //HYPERLINKS
    hyperlinkshop->setBounds(100,400,20,20);
    hyperlinklinked->setBounds(220, 400, 20, 20);
    //==============================================================================
   
    
}
void FlanGerrAudioProcessorEditor::timerCallback()
{
    delaySlider_.setValue(processor.delay_, dontSendNotification);
    sweepWidthSlider_.setValue(processor.sweepWidth_, dontSendNotification);
    depthSlider_.setValue(processor.depth_, dontSendNotification);
    feedbackSlider_.setValue(processor.feedback_, dontSendNotification);
    frequencySlider_.setValue(processor.frequency_, dontSendNotification);
   
}
//==============================================================================
// Similar callback to sliderValueChanged for ComboBox updates
void FlanGerrAudioProcessorEditor::comboBoxChanged (ComboBox* comboBox)
{
    
    auto index = comboBox->getSelectedItemIndex();
    
    if(comboBox == &waveformComboBox_)
        {
            
            int waveForm = (int) index;
            if (waveForm == 1 )
                {
                    processor.waveCH =  FlanGerrAudioProcessor::kWaveformSine ;
                }
            else if (waveForm == 2)
                {
                    processor.waveCH = FlanGerrAudioProcessor::kWaveformTriangle ;
                }
            else if (waveForm == 3)
                {
                    processor.waveCH = FlanGerrAudioProcessor::kWaveformSquare ;
                }
            else if (waveForm == 4)
                {
                    processor.waveCH = FlanGerrAudioProcessor::kWaveformSawtooth ;
                }
        }
    else if(comboBox == &interpolationComboBox_)
        {
            int interPoli = (int) index;
            if (interPoli == 1)
                {
                    processor.interCH = FlanGerrAudioProcessor::kInterpolationNearestNeighbour;
                }
            else if (interPoli == 2)
                {
                    processor.interCH = FlanGerrAudioProcessor::kInterpolationLinear;
                }
            else if (interPoli == 3)
                {
                    processor.interCH = FlanGerrAudioProcessor::kInterpolationCubic;
                }
        }
    else if (comboBox == &stereoToggleButton_)
        {
            bool reverse = (int) index;
            if (reverse)
                processor.stereoCH = 1.0;
            else
                processor.stereoCH = 0.0;
            
        }
}
//=================================================================================
void FlanGerrAudioProcessorEditor::buttonClicked (Button* buttonThatWasClicked)
{
    
    if (buttonThatWasClicked == &*aboutButton)
    {
        DialogWindow::showModalDialog("About", aboutComp, this, Colours::black, true);
    }
    
}
//==============================================================================


