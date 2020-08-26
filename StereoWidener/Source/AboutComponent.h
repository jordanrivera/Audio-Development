/*
  ==============================================================================

    AboutComponent.h
    Created: 11 Jul 2019 4:15:42pm
    Author:  cex

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
class AboutComponent  : public Component
{
public:
    //==============================================================================
    AboutComponent ();
    ~AboutComponent();
    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;



private:
    //==============================================================================
    std::unique_ptr<Label> label;
    std::unique_ptr<Label> label2;
    std::unique_ptr<Label> label4;
    Image picture;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AboutComponent)
};
