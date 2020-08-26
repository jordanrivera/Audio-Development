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

		Image widthKnobStrip = ImageCache::getFromMemory(BinaryData::ABSP1_png, BinaryData::ABSP1_pngSize);

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


class StereoWidenerAudioProcessorEditor  : public AudioProcessorEditor,
                                           public Slider::Listener,
                                           public Button::Listener,
                                           public Timer

{
public:
    StereoWidenerAudioProcessorEditor (StereoWidenerAudioProcessor&);
    ~StereoWidenerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void timerCallback() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sliderAttach;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sliderAttach2;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    StereoWidenerAudioProcessor& processor;
    
    WidthKnobLookAndFeel widbothKnobLookAndFeel;
    std::unique_ptr<VUMeterComponent> levelMeter;
    Slider widthSlider_, gainSlider_;
    Image backgroundimg;
    AboutComponent *aboutComp;
    ImageButton aboutButton;
    Label about;
    
    
   
	//GroupComponent mainGroup_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereoWidenerAudioProcessorEditor)
};
