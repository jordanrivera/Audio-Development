/*
  ==============================================================================

    VUMeterNeedle.h
    Created: 27 Jun 2019 5:08:49pm
    Author:  cex

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class VUMeterNeedle : public Component
{
public:
    VUMeterNeedle() {}
    ~VUMeterNeedle() {}

    void paint (Graphics& g) override
    {
        g.setColour (Colours::silver);
        needlePath.clear();

        float angleDifference = maxAngle - minAngle;
        float angle = minAngle + (angleDifference * (needlePos)) < minAngle ? minAngle : minAngle + (angleDifference * (needlePos));

        float x = needleLength * cos(angle) + getWidth() / 2;
        float y = needleLength * sin(angle);
        needlePath.startNewSubPath(getWidth() / 2, getHeight());
        needlePath.lineTo(x, getHeight() - y);

        g.strokePath(needlePath, PathStrokeType (3.0f));
    }

    void resized() override
    {
        needleLength = (getHeight() - 8) * 0.9;
        float centerX = getWidth() / 2.0;
        minAngle = acos((getWidth() - centerX) / needleLength) + 0.05;
        maxAngle = acos((0 - centerX) / needleLength) - 0.05;
    }

    float needlePos = 1;

private:
    Path needlePath;

    float needleLength;
    float minAngle;
    float maxAngle;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VUMeterNeedle)
};
