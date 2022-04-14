
#pragma once
#include "Oscillator.h"

// ===========================
// ===========================
// SOUND
class MySynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote      (int) override      { return true; }
    //--------------------------------------------------------------------------
    bool appliesToChannel   (int) override      { return true; }
};




// =================================
// =================================
// Synthesiser Voice - My synth code goes in here

/*!
 @class MySynthVoice
 @abstract struct defining the DSP associated with a specific voice.
 @discussion multiple MySynthVoice objects will be created by the Synthesiser so that it can be played polyphicially
 
 @namespace none
 @updated 2019-06-18
 */
class MySynthVoice : public juce::SynthesiserVoice
{
public:
    MySynthVoice() {}
    //--------------------------------------------------------------------------
    
    void init(float sampleRate)
    {
        tri.setSampleRate(sampleRate);
        detuneOsc.setSampleRate(sampleRate);
        square.setSampleRate(sampleRate);
        env.setSampleRate(sampleRate);
        
        
        lfo1.setSampleRate(sampleRate);
        
        juce::ADSR::Parameters envParams;
        
        envParams.attack = 0.05f;
        envParams.decay = 0.25f;
        envParams.sustain = 0.5f;
        envParams.release = 1.0f;
        
        env.setParameters(envParams);
    }
    
    void setDetune(float detuneIn)
    {
        detuneAmount = detuneIn;
    }
    /**
     What should be done when a note starts

     @param midiNoteNumber
     @param velocity
     @param SynthesiserSound unused variable
     @param / unused variable
     */
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        
        
        playing = true;
        ending = false;
        float freq = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        tri.setFrequency(freq);
        square.setFrequency(freq);
        
        lfo1.setFrequency(0.005f);
        
        
        
        
        env.reset();
        env.noteOn();
        
    }
    //--------------------------------------------------------------------------
    /// Called when a MIDI noteOff message is received
    /**
     What should be done when a note stops

     @param / unused variable
     @param allowTailOff bool to decie if the should be any volume decay
     */
    void stopNote(float /*velocity*/, bool allowTailOff) override
    {
        env.noteOff();
        ending = true;
        clearCurrentNote();
        playing = false;
    }
    
    //--------------------------------------------------------------------------
    /**
     The Main DSP Block: Put My DSP code in here
     
     If the sound that the voice is playing finishes during the course of this rendered block, it must call clearCurrentNote(), to tell the synthesiser that it has finished

     @param outputBuffer pointer to output
     @param startSample position of first sample in buffer
     @param numSamples number of smaples in output buffer
     */
    void renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {
        if (playing) // check to see if this voice should be playing
        {
            detuneOsc.setFrequency(freq - detuneAmount);
            // iterate through the necessary number of samples (from startSample up to startSample + numSamples)
            for (int sampleIndex = startSample;   sampleIndex < (startSample+numSamples);   sampleIndex++)
            {
                
                float envVal = env.getNextSample();
                
                
                // sample-by-sample DSP code here!cassette audio books                                                                              
                float currentSample = (tri.process3() + detuneOsc.process3())*0.5f * envVal ;
                
                // for each channel, write the currentSample float to the output
                for (int chan = 0; chan<outputBuffer.getNumChannels(); chan++)
                {
                    // The output sample is scaled by 0.2 so that it is not too loud by default
                    outputBuffer.addSample (chan, sampleIndex, currentSample * 0.2);
                }
                
                if (ending)
                {
                    if (envVal < 0.0001f)
                    {
                        clearCurrentNote();
                        playing = false;
                    }
                }
            }
        }
    }
    //--------------------------------------------------------------------------
    void pitchWheelMoved(int) override {}
    //--------------------------------------------------------------------------
    void controllerMoved(int, int) override {}
    //--------------------------------------------------------------------------
    /**
     Can this voice play a sound. I wouldn't worry about this for the time being

     @param sound a juce::SynthesiserSound* base class pointer
     @return sound cast as a pointer to an instance of MySynthSound
     */
    bool canPlaySound (juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<MySynthSound*> (sound) != nullptr;
    }
    //--------------------------------------------------------------------------
private:
    //--------------------------------------------------------------------------
    // Set up any necessary variables here
    /// Should the voice be playing?
    bool playing = false;
    bool ending = false;
    /// a random object for use in our test noise function
    juce::Random random;
    
    Oscillator tri, detuneOsc;
   
    Oscillator square, rampOsc1;
    
    Oscillator lfo1;
    
    /**
     @Param DetunedOsc
     */
    
    float detuneAmount = 2.0f;

    juce::ADSR env;
    
    float freq;
    
    /**
    @Param  rampOsc :
    */
    //float maxFreq = 1760.0f;
   // float minFreq = 55.0f;
    //float rampRate = 2.5f;
    
};
