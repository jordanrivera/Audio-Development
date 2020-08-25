/*
  ==============================================================================

    TwoPoleAllpassFilter.h
    Created: 4 Sep 2018 3:34:51pm
    Author:  cex

  ==============================================================================
*/
#ifndef __ONEPOLEALLPASSFILTER_H_6E48F605__
#define __ONEPOLEALLPASSFILTER_H_6E48F605__

#define _USE_MATH_DEFINES
#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
 * This class implements a 2nd-order allpass filter used in the phaser effect.
 */

class TwoPoleAllpassFilter : public IIRFilter
{
public:
    //==============================================================================
	
    /* Makes an allpass section at the given centre frequency and Q */
	void makeAllpass (const double inverseSampleRate,
                      const double centreFrequency,
                      const double Q) noexcept;
	
    /** Makes this filter duplicate the set-up of another one.
	 */
    void copyCoefficientsFrom (const TwoPoleAllpassFilter& other) noexcept;
	
private:
    //==============================================================================
	JUCE_LEAK_DETECTOR (TwoPoleAllpassFilter);
};


#endif // __TWOPOLEALLPASSFILTER_H_6E48F605__