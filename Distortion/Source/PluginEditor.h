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

		Image widthKnobStrip = ImageCache::getFromMemory(BinaryData::ABS2_png, BinaryData::ABS2_pngSize);

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


//==============================================================================
/**
*/
class DistortionAudioProcessorEditor  : public AudioProcessorEditor,
                                        public Button::Listener,
                                        public ChangeListener,
                                        public Timer
{
public:
    DistortionAudioProcessorEditor (DistortionAudioProcessor&);
    ~DistortionAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void timerCallback () override;
    //==============================================================================
    //ANALYSER FUNCTIONS
    static float getFrequencyForPosition (float pos);
    static float getPositionForFrequency (float freq);
    static float getPositionForGain (float gain, float top, float bottom);
    static float getGainForPosition (float pos, float top, float bottom);
    void changeListenerCallback (ChangeBroadcaster* sender) override;
    //==============================================================================
    
private:
    //==============================================================================
    #ifdef JUCE_OPENGL
    OpenGLContext           openGLContext;
    #endif
    
    void updateFrequencyResponses ();
    
    Path                    frequencyResponse;
    Path                    analyserPath;
    Rectangle<int>          plotFrame;
    SharedResourcePointer<TooltipWindow> tooltipWindow;
    std::unique_ptr<ResizableCornerComponent> resizer;
    ComponentBoundsConstrainer resizeLimits;
    //==============================================================================
    WidthKnobLookAndFeel widbothKnobLookAndFeel;
    std::unique_ptr<Slider> driveKnob;
	std::unique_ptr<Slider> rangeKnob;
	std::unique_ptr<Slider> blendKnob;
	std::unique_ptr<Slider> volumeKnob;
 
    //==============================================================================
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> rangeAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> blendAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;
    //==============================================================================
    HyperlinkButton* hyperlinklinked;
    HyperlinkButton* hyperlinkshop;

	
	//==============================================================================
	std::unique_ptr<VUMeterComponent> levelMeterL, levelMeterR;
	Image backgroundimg;
    AboutComponent *aboutComp;
    std::unique_ptr<ImageButton> aboutButton;
	Label about;

    DistortionAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionAudioProcessorEditor)
};
