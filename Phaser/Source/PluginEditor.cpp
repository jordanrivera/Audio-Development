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
PhaserAudioProcessorEditor::PhaserAudioProcessorEditor (PhaserAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    
    //background image
    backgroundimg = ImageFileFormat::loadFrom(BinaryData::background_png,BinaryData::background_pngSize);
    
    
    sliderAttachBF = std::make_unique <AudioProcessorValueTreeState::SliderAttachment>(processor.parameterState,PhaserAudioProcessor::baseFrequency, baseFrequencySlider_);
    sliderAttachSW = std::make_unique <AudioProcessorValueTreeState::SliderAttachment>(processor.parameterState,PhaserAudioProcessor::sweepWidth, sweepWidthSlider_);
    sliderAttachDS = std::make_unique <AudioProcessorValueTreeState::SliderAttachment>(processor.parameterState,PhaserAudioProcessor::deptH, depthSlider_);
    sliderAttachFB = std::make_unique <AudioProcessorValueTreeState::SliderAttachment>(processor.parameterState,PhaserAudioProcessor::feedBack, feedbackSlider_);
    sliderAttachLFO = std::make_unique <AudioProcessorValueTreeState::SliderAttachment>(processor.parameterState,PhaserAudioProcessor::lfoFrequency, lfoFrequencySlider_);
    sliderAttachG = std::make_unique <AudioProcessorValueTreeState::SliderAttachment>(processor.parameterState,PhaserAudioProcessor::gaiN,gainSlider_);
    
    comboAttach = std::make_unique <AudioProcessorValueTreeState::ComboBoxAttachment>(processor.parameterState,PhaserAudioProcessor::waveForm,waveformComboBox_);
    comboFilter = std::make_unique <AudioProcessorValueTreeState::ComboBoxAttachment>(processor.parameterState,PhaserAudioProcessor::filteR,filtersComboBox_);
    buttonAttach = std::make_unique <AudioProcessorValueTreeState::ComboBoxAttachment>(processor.parameterState,PhaserAudioProcessor::stereO,stereoToggleButton_);
    
    // Set up the sliders
    addAndMakeVisible (&baseFrequencySlider_);
    baseFrequencySlider_.setSliderStyle (Slider::Rotary);
    baseFrequencySlider_.addListener (this);
    baseFrequencySlider_.setRange (50.0, 1000.0, 0.5);
    baseFrequencySlider_.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    baseFrequencySlider_.setPopupDisplayEnabled(true, false, this);
    
    addAndMakeVisible (&sweepWidthSlider_);
    sweepWidthSlider_.setSliderStyle (Slider::Rotary);
    sweepWidthSlider_.addListener (this);
    sweepWidthSlider_.setRange (50.0, 5000.0, 0.5);
    sweepWidthSlider_.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    sweepWidthSlider_.setPopupDisplayEnabled(true, false, this);
    
    addAndMakeVisible (&depthSlider_);
    depthSlider_.setSliderStyle (Slider::Rotary);
    depthSlider_.addListener (this);
    depthSlider_.setRange (0.0, 1.0, 0.01);
    depthSlider_.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    depthSlider_.setPopupDisplayEnabled(true, false, this);
    
    addAndMakeVisible (&feedbackSlider_);
    feedbackSlider_.setSliderStyle (Slider::Rotary);
    feedbackSlider_.addListener (this);
    feedbackSlider_.setRange (0.0, 0.99, 0.01);
    feedbackSlider_.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    feedbackSlider_.setPopupDisplayEnabled(true, false, this);
    
    addAndMakeVisible (&lfoFrequencySlider_);
    lfoFrequencySlider_.setSliderStyle (Slider::Rotary);
    lfoFrequencySlider_.addListener (this);
    lfoFrequencySlider_.setRange (0.05, 2.0, 0.025);
    lfoFrequencySlider_.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    lfoFrequencySlider_.setPopupDisplayEnabled(true, false, this);
    
    addAndMakeVisible(&gainSlider_);
    gainSlider_.setSliderStyle(Slider::Rotary);
    gainSlider_.addListener(this);
    gainSlider_.setRange(-48.0f, 0.0f);
    gainSlider_.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    gainSlider_.setPopupDisplayEnabled(true, false, this);
    
    //Waveform ComboBox
    addAndMakeVisible(&waveformComboBox_);
    waveformComboBox_.setEditableText(false);
    waveformComboBox_.setJustificationType(Justification::left);
    waveformComboBox_.addItemList({"Sine", "Triangle", "Square", "Sawtooth"},1);
    waveformComboBox_.setSelectedId(1);
    waveformComboBox_.addListener(this);
    
    //Filter ComboBox
    addAndMakeVisible(&filtersComboBox_);
    filtersComboBox_.setEditableText(false);
    filtersComboBox_.setJustificationType(Justification::left);
    filtersComboBox_.addItemList({"2", "4", "6", "8", "10"},1);
    filtersComboBox_.setSelectedId(2);
    filtersComboBox_.addListener(this);
    
    //Stereo ComboBox
    addAndMakeVisible(&stereoToggleButton_);
    stereoToggleButton_.setEditableText(false);
    stereoToggleButton_.setJustificationType(Justification::left);
    stereoToggleButton_.addItemList({"Stereo", "Mono"},1);
    stereoToggleButton_.setSelectedId(1);
    stereoToggleButton_.addListener(this);
    
    //Set Look & feel
    baseFrequencySlider_.setLookAndFeel(&widbothKnobLookAndFeel);
    sweepWidthSlider_.setLookAndFeel(&widbothKnobLookAndFeel);
    depthSlider_.setLookAndFeel(&widbothKnobLookAndFeel);
    feedbackSlider_.setLookAndFeel(&widbothKnobLookAndFeel);
    lfoFrequencySlider_.setLookAndFeel(&widbothKnobLookAndFeel);
    gainSlider_.setLookAndFeel(&widbothKnobLookAndFeel);
    
    //Meter
    levelMeterL.reset (new VUMeterComponent (processor.meterSourceL));
    levelMeterR.reset (new VUMeterComponent(processor.meterSourceR));
    addAndMakeVisible (levelMeterL.get());
    addAndMakeVisible(levelMeterR.get());
    levelMeterL->setName ("new componentL");
    levelMeterR->setName ("new componentR");
    
    //=============================================================================
    // A B O U T  W I N D O W
    aboutButton.reset (new ImageButton ("aboutButton"));
    addAndMakeVisible(aboutButton.get());
    aboutButton->setButtonText (String());
    aboutButton->addListener (this);
    
    aboutButton->setImages (false, true, true,
                            Image(), 1.000f, Colour (0x00000000),
                            Image(), 1.000f, Colour (0x00000000),
                            Image(), 1.000f, Colour (0x00000000));
    aboutComp = new AboutComponent();
    //=============================================================================
    //HYPERLINKS
    addAndMakeVisible(hyperlinkshop = new HyperlinkButton(" ",URL("https://www.audiopluginshop.com")));
    addAndMakeVisible(hyperlinklinked = new HyperlinkButton(" ", URL ("https://nl.linkedin.com/in/yordan-adonias-rivera-rios-1a49b749")));
    
    //=============================================================================
    
    setSize (550, 570);
}

PhaserAudioProcessorEditor::~PhaserAudioProcessorEditor()
{
    sliderAttachBF = nullptr;
    sliderAttachSW = nullptr;
    sliderAttachDS = nullptr;
    sliderAttachFB = nullptr;
    sliderAttachLFO = nullptr;
    sliderAttachG= nullptr;
    comboAttach = nullptr;
    comboFilter = nullptr;
    buttonAttach = nullptr;
    levelMeterL = nullptr;
    levelMeterR = nullptr;
    aboutButton = nullptr;
}

//==============================================================================
void PhaserAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    int x = 1, y = 1, width = 550, heigth = 570;
    g.drawImage(backgroundimg,x,y,width,heigth,0,0,backgroundimg.getWidth(),backgroundimg.getHeight());
}

void PhaserAudioProcessorEditor::resized()
{
    baseFrequencySlider_.setBounds (53, 180,130,130);
    feedbackSlider_.setBounds(53, 320, 130, 130);
    sweepWidthSlider_.setBounds (213, 180, 130, 130);
    lfoFrequencySlider_.setBounds(213, 320, 130, 130);
    depthSlider_.setBounds(373, 180, 130,130);
    gainSlider_.setBounds(373,320,130,130);
   
    waveformComboBox_.setBounds(73, 470, 90, 30);
    filtersComboBox_.setBounds(233, 470,90, 30);
    stereoToggleButton_.setBounds(393, 470, 90, 30);
    
    levelMeterL->setBounds(40,10,230,160);
    levelMeterR->setBounds(280,10,230,160);
    
    aboutButton->setBounds (160,530,30,50);
    hyperlinkshop->setBounds(470,530,30,20);
    hyperlinklinked->setBounds(370, 530, 30, 20);
}
//==============================================================================
// This timer periodically checks whether any of the filter's parameters have changed...
void PhaserAudioProcessorEditor::timerCallback()
{
    
    
    baseFrequencySlider_.setValue(processor.baseFrequency_, dontSendNotification);
    sweepWidthSlider_.setValue(processor.sweepWidth_, dontSendNotification);
    depthSlider_.setValue(processor.depth_, dontSendNotification);
    feedbackSlider_.setValue(processor.feedback_, dontSendNotification);
    lfoFrequencySlider_.setValue(processor.lfoFrequency_, dontSendNotification);
    gainSlider_.setValue(processor.gain_,dontSendNotification);
    
    waveformComboBox_.setSelectedId(processor.waveform_, dontSendNotification);
    filtersComboBox_.setSelectedId(processor.filtersPerChannel_, dontSendNotification);
    stereoToggleButton_.setSelectedId((processor.stereo_ != 0), dontSendNotification);
}

//==========================This is our Slider::Listener callback, when the user drags a slider.
void PhaserAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    
    
    if (slider == &baseFrequencySlider_)
    {
        processor.baseFrequency_ = (float)baseFrequencySlider_.getValue();
    }
    else if (slider == &sweepWidthSlider_)
    {
        processor.sweepWidth_ = (float)sweepWidthSlider_.getValue();
    }
    else if (slider == &depthSlider_)
    {
        processor.depth_ = (float)depthSlider_.getValue();
    }
    else if (slider == &feedbackSlider_)
    {
        processor.feedback_ = (float)feedbackSlider_.getValue();
    }
    else if (slider == &lfoFrequencySlider_)
    {
        processor.lfoFrequency_  = (float)lfoFrequencySlider_.getValue();
    }
    else if (slider == &gainSlider_)
    {
        processor.gain_ = (float)gainSlider_.getValue();
    }
     
}

//==================Similar callback to sliderValueChanged for ComboBox updates
//=============================================================================
void PhaserAudioProcessorEditor::comboBoxChanged (ComboBox *comboBox)
{
    
    auto index = comboBox->getSelectedItemIndex();
    
    if(comboBox == &waveformComboBox_)
    {
       
        int soms = (int)index;
        if (soms == 1)
        {
            processor.wavefoRM = PhaserAudioProcessor::kWaveformSine;
        }    
        else if (soms == 2)
        {
            processor.wavefoRM = PhaserAudioProcessor::kWaveformTriangle;
        }   
        else if (soms == 3)
        {
            processor.wavefoRM = PhaserAudioProcessor::kWaveformSquare;
        }
        else if (soms == 4)
        {
            processor.wavefoRM = PhaserAudioProcessor::kWaveformSawtooth;
        }
       
       
    }
    else if(comboBox == &filtersComboBox_)
    {
       
        int timeS = (int)index;
        if (timeS == 1)
         {
            processor.filtersPCH =  2;
         }
        else if (timeS == 2)
        {
            processor.filtersPCH = 4;
        }
        else if (timeS == 3)
        {
            processor.filtersPCH = 6;
        }
        else if (timeS == 4)
        {
            processor.filtersPCH = 8;
        }
        else if (timeS == 5)
        {
            processor.filtersPCH = 10;
        }
     
    }
    else if (comboBox == &stereoToggleButton_)
    {
        bool reverse = (int)index;
        if (reverse)
            processor.stereoCH = 1.0;
        else 
            processor.stereoCH = 0.0;
    }
}
//=================================================================================
void PhaserAudioProcessorEditor::buttonClicked (Button* buttonThatWasClicked)
{
    
    if (buttonThatWasClicked == &*aboutButton)
    {
        DialogWindow::showModalDialog("About", aboutComp, this, Colours::black, true);
    }
    
}
