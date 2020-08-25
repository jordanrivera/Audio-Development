/*
  ==============================================================================

    TwoPoleAllpassFilter.cpp
    Created: 4 Sep 2018 3:34:51pm
    Author:  cex

  ==============================================================================
*/

#include "TwoPoleAllpassFilter.h"

/* The actual audio processing is handled by the Juce IIRFilter parent
 * class. This subclass is used to define the coefficients for our
 * implementation of an allpass filter
 */

void TwoPoleAllpassFilter::makeAllpass (const double inverseSampleRate,
                                        const double centreFrequency,
                                        const double Q) noexcept
{
    jassert (Q > 0);

    const double discreteFrequency = 2.0 * M_PI * centreFrequency * inverseSampleRate;
    const double alpha = sin(discreteFrequency) / (2.0 * Q);
    const double cos_w0 = -2.0 * cos(discreteFrequency);
    
    // setCoefficients() takes arguments: b0, b1, b2, a0, a1, a2
    // It will normalise the filter according to the value of a0
    // to allow standard time-domain implementations
    
    coefficients = IIRCoefficients(1.0 - alpha, /* b0 */
                     cos_w0, /* b1 */
                     1.0 + alpha, /* b2 */
                     1.0 + alpha, /* a0 */
                     cos_w0, /* a1 */
                     1.0 - alpha /* a2 */);
    
    setCoefficients(coefficients);
}

/* Copy coefficients from another object of the same class */

void TwoPoleAllpassFilter::copyCoefficientsFrom (const TwoPoleAllpassFilter& other) noexcept
{
    setCoefficients(other.coefficients);
    active = other.active;
}
