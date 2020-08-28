# Audio-Development
 Audioplugins

This repository is a collection of audio effects plugins.

#Flanger 
simulates a delay-based audio effect where a copy of the input signal is delayed with a variable delay time, and mixed with the original sound, thus producing the characteristic "swooshing" sound of this classic audio effect. 

#Phaser 
uses all-pass filters in cascade configuration to introduce phase shifts to the input signal. These shifts create notches in the frequency spectrum when the filtered signal is mixed with the original one. The phaser produces a similar effect to the flanger, but there is potentially more control on the location of the notches.

#Distortion 
applies a non-linear transformation to the input sound which increases its gain to limits that create a harsh, fuzzy, or gritty sound. Different non-linear functions can be selected and the output gain can be controlled individually to restore the original loudness level. A high-shelf filter can be used to control the tone of the output sound as well. 

#Clone and build

These plugins are implemented using JUCE, which provides a framework for building audio plugins for various platforms (Windows and macOS), and in multiple formats (VST, AU, RTAS, AAX, etc.), as well as standalone applications.
