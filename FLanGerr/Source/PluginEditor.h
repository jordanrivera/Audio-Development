/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
class AboutComponent;
//==============================================================================
/**
*/

class WidthKnobLookAndFeel : public LookAndFeel_V4
{
public:
	WidthKnobLookAndFeel()
	{

	}

	void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
		const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
	{

		// This is the binary image data that uses very little CPU when rotating

		Image widthKnobStrip = ImageCache::getFromMemory(BinaryData::ABS_png, BinaryData::ABS_pngSize);

		//Image myStrip = ImageCache::getFromMemory(BinaryData::ParamsRotaryKnob_png, BinaryData::ParamsRotaryKnob_pngSize);

		const double fractRotation = (slider.getValue() - slider.getMinimum()) / (slider.getMaximum() - slider.getMinimum()); //value between 0 and 1 for current amount of rotation
		const int nFrames = widthKnobStrip.getHeight() / widthKnobStrip.getWidth(); // number of frames for vertical film strip
		const int frameIdx = (int)ceil(fractRotation * ((double)nFrames - 1.0)); // current index from 0 --> nFrames-1

		const float radius = jmin(width / 2.0f, height / 2.0f);
		const float centreX = x + width * 0.5f;
		const float centreY = y + height * 0.5f;
		const float rx = centreX - radius - 1.0f;
		const float ry = centreY - radius - 1.0f;

		g.drawImage(widthKnobStrip,
			(int)rx,
			(int)ry,
			2 * (int)radius,
			2 * (int)radius,   //Dest
			0,
			frameIdx*widthKnobStrip.getWidth(),
			widthKnobStrip.getWidth(),
			widthKnobStrip.getWidth()); //Source
	}
};

//====================================================================================================
class FlanGerrAudioProcessorEditor  : public AudioProcessorEditor,
                                     public Slider::Listener,
                                     public ComboBox::Listener,
                                     public Button::Listener,
                                     public Timer
{
public:
    FlanGerrAudioProcessorEditor (FlanGerrAudioProcessor&);
    ~FlanGerrAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void timerCallback() override;
    void sliderValueChanged (Slider* slider) override;
    void comboBoxChanged (ComboBox* comboBox) override;
    void buttonClicked (Button* buttonThatWasClicked) override;

    //============================================================================
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> sliderAttachD;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> sliderAttachS;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> sliderAttachDP;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> sliderAttachF;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> sliderAttachFQ;
    
    std::unique_ptr <AudioProcessorValueTreeState::ComboBoxAttachment> ComboBoxAttach;
    std::unique_ptr <AudioProcessorValueTreeState::ComboBoxAttachment> ComboBoxAttach2;
    std::unique_ptr <AudioProcessorValueTreeState::ComboBoxAttachment> buttonAttach;
    
    
private:
    //==============================================================================
    FlanGerrAudioProcessor& processor;
    WidthKnobLookAndFeel widbothKnobLookAndFeel;
    
    HyperlinkButton* hyperlinklinked;
    HyperlinkButton* hyperlinkshop;
   
    std::unique_ptr<VUMeterComponent> levelMeterL, levelMeterR;
    Image backgroundimg;
    
    AboutComponent *aboutComp;
    std::unique_ptr<ImageButton> aboutButton;
    Label about;
    
    Slider delaySlider_;
    Slider sweepWidthSlider_;
    Slider depthSlider_;
    Slider feedbackSlider_;
    Slider frequencySlider_;
    
    ComboBox waveformComboBox_, interpolationComboBox_, stereoToggleButton_;
    
    FlanGerrAudioProcessor* getProcessor() const
    {
        return static_cast <FlanGerrAudioProcessor*> (getAudioProcessor());
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FlanGerrAudioProcessorEditor)
};
