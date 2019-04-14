/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
KadenzeChorusAppAudioProcessor::KadenzeChorusAppAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    m_delay_time_smooth = 0.0f;
    m_feedback_left = 0.0f;
    m_feedback_right = 0.0f;
    m_dry_wet = 0.5f;
    
    m_dry_wet_p = std::make_unique<AudioParameterFloat>("drywet","Dry/Wet",0.01,1.0,0.05);
    m_feedback_p = std::make_unique<AudioParameterFloat>("feedback","Feedback",0.0,0.98f,0.5);
    
    m_phase_offset_p = std::make_unique<AudioParameterFloat>("phase","Phase Offset",0.0, 1.f,0.0f);

    //m_delay_time_p = std::make_unique<AudioParameterFloat>("delaytime","Delay Time",0.01,MAX_DELAY_TIME,0.5);
    m_depth_p = std::make_unique<AudioParameterFloat>("depth","Depth",0.0,1.0f,0.5);
    m_rate_p = std::make_unique<AudioParameterFloat>("rate","Rate",0.1f,20.0f,10.0f);
    m_type_p = std::make_unique<AudioParameterInt>("type","Type",0,1,0);
    
    m_left_circular_buffer = nullptr;
    m_right_circular_buffer = nullptr;
    m_delay_time_smooth = 0;

    addParameter(m_dry_wet_p.get());
    addParameter(m_depth_p.get());
    addParameter(m_rate_p.get());
    addParameter(m_phase_offset_p.get());
    addParameter(m_feedback_p.get());

    addParameter(m_type_p.get());

    //addParameter(m_delay_time_p.get());
}

KadenzeChorusAppAudioProcessor::~KadenzeChorusAppAudioProcessor()
{

}

//==============================================================================
const String KadenzeChorusAppAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool KadenzeChorusAppAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool KadenzeChorusAppAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool KadenzeChorusAppAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double KadenzeChorusAppAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int KadenzeChorusAppAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int KadenzeChorusAppAudioProcessor::getCurrentProgram()
{
    return 0;
}

void KadenzeChorusAppAudioProcessor::setCurrentProgram (int index)
{
}

const String KadenzeChorusAppAudioProcessor::getProgramName (int index)
{
    return {};
}

void KadenzeChorusAppAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void KadenzeChorusAppAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    m_delay_time_smooth = 1;

    circular_buffer_length = sampleRate * MAX_DELAY_TIME;
    //m_delay_in_samples = sampleRate * *m_delay_time_p;
    m_left_circular_buffer = std::make_unique<float[]>(circular_buffer_length);
    
    zeromem(m_left_circular_buffer.get(), circular_buffer_length*sizeof(float));
    
    m_right_circular_buffer = std::make_unique<float[]>(circular_buffer_length);
    
    zeromem(m_right_circular_buffer.get(), circular_buffer_length*sizeof(float));
    circular_buffer_write_head = 0;
    
    
    m_delay_time_smooth = 1;
    
}

void KadenzeChorusAppAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool KadenzeChorusAppAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

std::tuple<int,int,float> KadenzeChorusAppAudioProcessor::update_read_head(float m_delay_in_samples_left,int circular_buffer_length) const{
    float m_delay_read_head_left = static_cast<float>(circular_buffer_write_head) - m_delay_in_samples_left;
    
    if(m_delay_read_head_left < 0){
        m_delay_read_head_left += circular_buffer_length;
    }
    
    int delay_read_head_x = static_cast<int>(m_delay_read_head_left);
    auto delay_read_head_x1 = delay_read_head_x + 1;
    float read_head_float = m_delay_read_head_left - delay_read_head_x;
    if(delay_read_head_x1 >= circular_buffer_length){
        delay_read_head_x1 -= circular_buffer_length;
    }
    return { delay_read_head_x, delay_read_head_x1,read_head_float};
}

float KadenzeChorusAppAudioProcessor::computeDelayTime(unsigned int i,
                                                       float m_depth,
                                                       float m_rate,
                                                       float m_phase_offset,
                                                       int type,
                                                       LFO& lfo) {
    auto updated_lfo = m_depth *lfo.update_lfo(m_rate, getSampleRate(),m_phase_offset);
    //auto lfo_mapped = jmap(updated_lfo, -1.f, 1.f,0.005f, 0.03f);
    auto lfo_mapped = type == 1 ? jmap(updated_lfo, -1.f, 1.f,0.005f, 0.03f) : jmap(updated_lfo, -1.f, 1.f,0.001f, 0.005f);
    float m_delay_in_samples_left = getSampleRate() * lfo_mapped;
    return m_delay_in_samples_left;
}

void KadenzeChorusAppAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    DBG("DRY/WET: " << *m_dry_wet_p);
    DBG("DEPTH: " << *m_depth_p);
    DBG("RATE: " << *m_rate_p);
    DBG("PHASE: " << *m_phase_offset_p);
    DBG("FEEDBACK: " << *m_feedback_p);
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    auto* leftChannel = buffer.getWritePointer (0);
    auto* rightChannel = buffer.getWritePointer (1);

    // ..do something to the data...
    for(unsigned int i = 0; i < buffer.getNumSamples(); ++i){
        auto t = m_type_p->get();
        
        /**
         * Set left channel
         * */
        float m_delay_in_samples_left = computeDelayTime(i,m_depth_p->get(),
                                                         m_rate_p->get(),
                                                         0.0f,
                                                         t,
                                                         lfo_left);
        m_left_circular_buffer[circular_buffer_write_head] = leftChannel[i] + m_feedback_left;

        auto values_left = update_read_head(m_delay_in_samples_left,circular_buffer_length);
        auto delayed_sample_left = lin_interp(m_left_circular_buffer[std::get<0>(values_left)],m_left_circular_buffer[std::get<1>(values_left)],std::get<2>(values_left));
        m_feedback_left = m_feedback_p->get() * delayed_sample_left;
        buffer.setSample(0, i, (1-m_dry_wet_p->get())*buffer.getSample(0,i) + m_dry_wet_p->get()*delayed_sample_left);
        
        /**
         Set right channel
         **/
        
        float m_delay_in_samples_right = computeDelayTime(i,m_depth_p->get(),
                                                         m_rate_p->get(),
                                                         m_phase_offset_p->get(),
                                                         t,
                                                         lfo_right);
        m_right_circular_buffer[circular_buffer_write_head] = rightChannel[i] + m_feedback_right;

        auto values_right = update_read_head(m_delay_in_samples_right,
                                             circular_buffer_length);
        auto delayed_sample_right = lin_interp(m_right_circular_buffer[std::get<0>(values_right)],m_right_circular_buffer[std::get<1>(values_right)],std::get<2>(values_right));
        
        m_feedback_right = m_feedback_p->get() * delayed_sample_right;
        buffer.setSample(1, i, (1-m_dry_wet_p->get())*buffer.getSample(1,i) + m_dry_wet_p->get()*delayed_sample_right);
        
        circular_buffer_write_head = (circular_buffer_write_head + 1) % circular_buffer_length;
    }
}

//==============================================================================
bool KadenzeChorusAppAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* KadenzeChorusAppAudioProcessor::createEditor()
{
    return new KadenzeChorusAppAudioProcessorEditor (*this);
}

//==============================================================================
void KadenzeChorusAppAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void KadenzeChorusAppAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KadenzeChorusAppAudioProcessor();
}

float KadenzeChorusAppAudioProcessor::lin_interp(float sample_x, float sample_x1, float in_phase) const{
    return (1-in_phase)*sample_x + in_phase*sample_x1;
}
