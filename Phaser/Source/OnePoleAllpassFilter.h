/*
  ==============================================================================

    OnePoleAllpassFilter.h
    Created: 4 Sep 2018 3:31:40pm
    Author:  cex

  ==============================================================================
*/

#ifndef __ONEPOLEALLPASSFILTER_H_6E48F605__
#define __ONEPOLEALLPASSFILTER_H_6E48F605__

#include "../JuceLibraryCode/JuceHeader.h"
#define _USE_MATH_DEFINES
#pragma once
#include <math.h>

class OnePoleAllpassFilter
{
public:
    //==============================================================================
	// Create an unititialised filter.
    // This will not perform any filtering until the allpass coefficients have been set.
	OnePoleAllpassFilter() noexcept;
	
	// Destructor.
	~OnePoleAllpassFilter() noexcept;
	
	//Process a number of samples in one go.
    // This is the most effecient method of filtering.
    // Note that the samples passed to it actually get changed.
	void processSamples (float* const samples,
						 const int numSamples) noexcept;
	
	// Process a single sample.
    // Less efficient method but leaves the sample unchanged,
    // returning a filtered copy of it.
    float processSingleSampleRaw (const float sampleToProcess) noexcept;
	
	// Set the centre frequency of the allpass with respect to the sample rate
	void makeAllpass (const double inverseSampleRate,
                      const double centreFrequency) noexcept;
  
private:
    //==============================================================================
    CriticalSection lock;
    
	float x1, y1, b0, b1, a1;
	
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OnePoleAllpassFilter);
};
#endif /* __ONEPOLEALLPASSFILTER_H_6E48F605__ */

