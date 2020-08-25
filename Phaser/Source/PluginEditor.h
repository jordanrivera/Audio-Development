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

		Image widthKnobStrip = ImageCache::getFromMemory(BinaryData::ABS1_png, BinaryData::ABS1_pngSize);

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
			0.0f,
			frameIdx*widthKnobStrip.getWidth(),
			widthKnobStrip.getWidth(),
			widthKnobStrip.getWidth()); //Source
	}
};

//===============================================================



class PhaserAudioProcessorEditor  : public AudioProcessorEditor,
                                    public Slider::Listener,
                                    public ComboBox::Listener,
                                    public Button::Listener,
                                    public Timer
                                    
{
public:
    PhaserAudioProcessorEditor (PhaserAudioProcessor&);
    ~PhaserAudioProcessorEditor();

    //==============================================================================
    void timerCallback() override;
    void paint (Graphics&) override;
    void resized() override;
    void sliderValueChanged (Slider*) override;
    void comboBoxChanged (ComboBox *) override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    
    
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> sliderAttachBF;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> sliderAttachSW;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> sliderAttachDS;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> sliderAttachFB;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> sliderAttachLFO;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> sliderAttachG;
    
    std::unique_ptr <AudioProcessorValueTreeState::ComboBoxAttachment> comboAttach;
    std::unique_ptr <AudioProcessorValueTreeState::ComboBoxAttachment> comboFilter;
    std::unique_ptr <AudioProcessorValueTreeState::ComboBoxAttachment> buttonAttach;

private:
    
    HyperlinkButton* hyperlinklinked;
    HyperlinkButton* hyperlinkshop;
    
    PhaserAudioProcessor& processor;
    WidthKnobLookAndFeel widbothKnobLookAndFeel;
    std::unique_ptr<VUMeterComponent> levelMeterL, levelMeterR;
    Image backgroundimg;
    AboutComponent *aboutComp;
    std::unique_ptr <ImageButton> aboutButton;
    Label about;
    Slider baseFrequencySlider_,sweepWidthSlider_, depthSlider_, feedbackSlider_, lfoFrequencySlider_, gainSlider_;
    ComboBox waveformComboBox_, filtersComboBox_, stereoToggleButton_;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhaserAudioProcessorEditor)
};
