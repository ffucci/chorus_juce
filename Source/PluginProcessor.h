/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <memory>
#include "LFO.h"

#define MAX_DELAY_TIME 2

//==============================================================================
/**
*/
class KadenzeChorusAppAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    KadenzeChorusAppAudioProcessor();
    ~KadenzeChorusAppAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void extracted(float m_delay_in_samples_left);
    
    void extracted(AudioBuffer<float> &buffer, unsigned int i, float *leftChannel);
    
    float extracted(unsigned int i, float *leftChannel);
    
void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    
    
    float lin_interp(float sample_x, float sample_x1, float in_phase) const;
    std::tuple<int,int,float> update_read_head(float m_delay_in_samples_left,int circular_buffer_length) const;
    
    float computeDelayTime(unsigned int i,
                           float m_depth,
                           float m_rate,
                           float m_phase_offset,
                           int type,
                           LFO& lfo);
    
    //==============================================================================
    std::unique_ptr<float[]> m_left_circular_buffer;
    std::unique_ptr<float[]> m_right_circular_buffer;
    
    int circular_buffer_write_head;
    int circular_buffer_length;
    
    float m_feedback_left;
    float m_feedback_right;
    float m_delay_read_head;
    float m_delay_in_samples;
    
    float m_delay_time_smooth;
    
    std::unique_ptr<AudioParameterFloat> m_dry_wet_p;
    std::unique_ptr<AudioParameterFloat> m_feedback_p;
    std::unique_ptr<AudioParameterFloat> m_phase_offset_p;

    //std::unique_ptr<AudioParameterFloat> m_delay_time_p;
    std::unique_ptr<AudioParameterFloat> m_depth_p;
    std::unique_ptr<AudioParameterFloat> m_rate_p;
    std::unique_ptr<AudioParameterInt> m_type_p;

    LFO lfo_left;
    LFO lfo_right;
    
    float m_dry_wet;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KadenzeChorusAppAudioProcessor)
};
