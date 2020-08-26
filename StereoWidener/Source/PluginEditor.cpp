/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "AboutComponent.h"
//==============================================================================
StereoWidenerAudioProcessorEditor::StereoWidenerAudioProcessorEditor (StereoWidenerAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    sliderAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameterState, StereoWidenerAudioProcessor::gaiN, gainSlider_);
    sliderAttach2 = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameterState,StereoWidenerAudioProcessor::widtH, widthSlider_);
    
    
    //background image
    backgroundimg = ImageFileFormat::loadFrom(BinaryData::background_png,BinaryData::background_pngSize);
    
	//Slider
	addAndMakeVisible( widthSlider_);
    widthSlider_.setRange(0.0f, 2.0f,0.1f);
    widthSlider_.setSliderStyle(Slider::Rotary);
    widthSlider_.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    widthSlider_.setPopupDisplayEnabled(true, false, this);
    //widthSlider_.addListener(this);
    
    addAndMakeVisible (gainSlider_);
    gainSlider_.setRange(-48.0f,0.0f);
    gainSlider_.setSliderStyle(Slider::Rotary);
    gainSlider_.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    gainSlider_.setPopupDisplayEnabled(true, false, this);
    //widthSlider_.addListener(this);

	//Meter
    levelMeter.reset (new VUMeterComponent (processor.meterSource));
    addAndMakeVisible (levelMeter.get());
    levelMeter->setName ("new component");
	
	
	//Set Look & feel
    widthSlider_.setLookAndFeel(&widbothKnobLookAndFeel);
    gainSlider_.setLookAndFeel(&widbothKnobLookAndFeel);
    //=============================================================================
    // A B O U T  W I N D O W
    addAndMakeVisible (aboutButton);
    aboutButton.setName ("AboutButton");
    aboutButton.setButtonText (String());
    aboutButton.addListener (this);
    
    aboutButton.setImages (false, true, true,
                            Image(), 1.000f, Colour (0x00000000),
                            Image(), 1.000f, Colour (0x00000000),
                            Image(), 1.000f, Colour (0x00000000));
    aboutComp = new AboutComponent();
    //=============================================================================
    setSize (390, 450);
	startTimer(50);
}

StereoWidenerAudioProcessorEditor::~StereoWidenerAudioProcessorEditor()
{
    sliderAttach = nullptr;
    sliderAttach2 = nullptr;
	levelMeter = nullptr;
	//aboutButton = nullptr;
}

//==============================================================================
void StereoWidenerAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    int x = 1, y = 1, width = 390, heigth = 450;
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.drawImage(backgroundimg,x,y,width,heigth,0,0,backgroundimg.getWidth(),backgroundimg.getHeight());
}

void StereoWidenerAudioProcessorEditor::resized()
{
    
    widthSlider_.setBounds(61, 250, 110, 110);
    gainSlider_.setBounds(216,250,110,110);
	levelMeter->setBounds(40,25,300,200);
    aboutButton.setBounds (180,420,30,100);
}
void StereoWidenerAudioProcessorEditor::timerCallback(){}
//==============================================================================
void StereoWidenerAudioProcessorEditor::buttonClicked (Button* buttonThatWasClicked)
{
    
    if (buttonThatWasClicked == &aboutButton)
    {
        
        DialogWindow::showModalDialog("About", aboutComp, this, Colours::black, true);
    }
    
}
//==============================================================================
void StereoWidenerAudioProcessorEditor::sliderValueChanged(Slider* sliderThatWasMoved) {}


