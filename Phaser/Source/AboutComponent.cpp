/*
  ==============================================================================

    AboutComponent.cpp
    Created: 23 Jul 2019 4:08:32pm
    Author:  cex

  ==============================================================================
*/

#include "AboutComponent.h"
//==============================================================================
AboutComponent::AboutComponent ()
{

    picture = ImageFileFormat::loadFrom(BinaryData::maxresdefaultBen_png,(size_t)BinaryData::maxresdefaultBen_pngSize);
    label.reset (new Label ("new label",TRANS("Audiopluginshop")));
    addAndMakeVisible(label.get());
    label->setFont (Font (22.00f, Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (Justification::centred);
    label->setEditable (false, false, false);
    label->setColour (Label::textColourId, Colours::white);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label2.reset ( new Label ("new label",TRANS("")));
    addAndMakeVisible(label2.get());
    label2->setFont (Font (14.00f, Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (Justification::left);
    label2->setEditable (false, false, false);
    label2->setColour (Label::textColourId, Colours::white);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label4.reset (new Label
                       ("new label",
                        TRANS("It all began in the 1960s at MIT, origin of the term hacker, where extremely skilled individuals practiced hardcore programming in FORTRAN and other older languages. Some may ignorantly dub them nerds or geeks but these individuals were, by far, the most intelligent, individual, and intellectually advanced people who happen to be the pioneers and forefathers of the talented individuals that are today the true hackers. The true hackers amongst our societies have an unquenchable thirst for knowledge. Boredom is never an object of challenge for hackers. They have an almost anomalous ability to absorb, retain, and exert vast amounts of knowledge with regard to intricate details. In 1969, Bell Labs employee Ken Thompson invented UNIX and permanently changed the future of the computer industry. Then in the very early 1970s, Dennis Ritchie invented the computer programming language C which was specifically invented to be used with UNIX. Programmers ceased to use assembler, while developing an appreciation for the portability of C.Nowadays we try to hack life true sound and vibration, try to open portals to other dimensions with sound frequencys! If u get stuck don't hasitate to call us at 1111" )));
    addAndMakeVisible(label4.get());
    label4->setFont (Font (14.00f, Font::plain).withTypefaceStyle ("Regular"));
    label4->setJustificationType (Justification::topLeft);
    label4->setEditable (false, false, false);
    label4->setColour (Label::textColourId, Colours::white);
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    githubLink.reset (new HyperlinkButton (TRANS("https://www.audiopluginshop.com"),
                                                         URL ("http://www.audiopluginshop.com")));
    addAndMakeVisible(githubLink.get());
    githubLink->setTooltip (TRANS("http://www.juce.com"));
    githubLink->setButtonText (TRANS("https://audiopluginshop.com/"));
    githubLink->setColour (HyperlinkButton::textColourId, Colour (0xccffffff));

    versionLabel.reset (new Label ("versionLabel",
                                                 TRANS("Version:")));
    addAndMakeVisible(versionLabel.get());
    versionLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    versionLabel->setJustificationType (Justification::centred);
    versionLabel->setEditable (false, false, false);
    versionLabel->setColour (Label::textColourId, Colours::white);
    versionLabel->setColour (TextEditor::textColourId, Colours::black);
    versionLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    String versionText = String("Version: ") + String(ProjectInfo::versionString);
    String buildDate = " (Build: " + String(__DATE__) + " " + String(__TIME__) + ")";
    versionLabel->setText(versionText + buildDate, dontSendNotification);
    setSize (400, 600);
}
//==============================================================================
AboutComponent::~AboutComponent()
{
  
    label = nullptr;
    label2 = nullptr;
    label4 = nullptr;
    githubLink = nullptr;
    versionLabel = nullptr;
}

//==============================================================================
void AboutComponent::paint (Graphics& g)
{

    g.fillAll (Colours::black);
    g.drawImage(picture, 20, 0, 350, 200, 0, 0, picture.getWidth(), picture.getHeight());
    
    Rectangle<float> areaScreenUp (8,210,370,40);
    g.setColour(Colours::skyblue);
    g.drawRoundedRectangle(areaScreenUp, 20.f, 2.0f);

}
void AboutComponent::resized()
{

    label->setBounds (8, 210, 384, 32);
    //label2->setBounds (15, 200, 96, 14);
    label4->setBounds (15, 254, 350, 440);
    githubLink->setBounds (8, 548, 384, 24);
    versionLabel->setBounds (8, 578, 384, 24);
 
}
