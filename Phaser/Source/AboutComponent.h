/*
  ==============================================================================

    AboutComponent.h
    Created: 23 Jul 2019 4:08:32pm
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
    std::unique_ptr <Label> label;
    std::unique_ptr <Label> label2;
    std::unique_ptr <Label> label4;
    std::unique_ptr<HyperlinkButton> githubLink;
    std::unique_ptr<Label> versionLabel;
    Image picture;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AboutComponent)
};
