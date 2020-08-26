/*
  ==============================================================================

    VUMeter.cpp
    Created: 30 Jul 2019 8:39:04pm
    Author:  cex

  ==============================================================================
*/

#include "VUMeter.h"

VUMeter::VUMeter()
{
    gradientTime.reset(REFRESH_RATE, RAMP_TIME_S);
    gradientTime.setTargetValue(1.0);
    gradientTime.skip(REFRESH_RATE * RAMP_TIME_S);
    test.setSize(1, 512*8);
    test.clear();
}

VUMeter::~VUMeter()
{
}

void VUMeter::measureBuffer(AudioSampleBuffer &buffer)
{
    double newRMS = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
    double squaredRMS = newRMS * newRMS;

    rmsSum = rmsSum + squaredRMS - rmsHistory[rmsWrite];
    rmsHistory[rmsWrite] = squaredRMS;
    rmsWrite = (rmsWrite + 1) % RMS_HISTORY_LENGTH;

    float dBNormalised = Decibels::gainToDecibels(sqrtf(rmsSum / RMS_HISTORY_LENGTH), -70.0f) / -70.0f;
    gradientTime.setTargetValue(dBNormalised);
}

float VUMeter::getValue()
{
    return gradientTime.getNextValue();
}
