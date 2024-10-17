#include <JuceHeader.h>

class AudioEffectsProcessor : public juce::AudioProcessor
{
public:
    AudioEffectsProcessor()
        : parameters(*this, nullptr, "Parameters",
            {
                std::make_unique<juce::AudioParameterFloat>("delayTime", "Delay Time", 0.01f, 1.0f, 0.5f),
                std::make_unique<juce::AudioParameterFloat>("feedback", "Feedback", 0.0f, 0.95f, 0.5f),
                std::make_unique<juce::AudioParameterFloat>("roomSize", "Room Size", 0.0f, 1.0f, 0.8f),
                std::make_unique<juce::AudioParameterFloat>("threshold", "Noise Gate Threshold", 0.0f, 1.0f, 0.1f)
            })
    {
        delayBuffer.setSize(2, 44100); // 1 second buffer at 44100 Hz
    }

    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        this->sampleRate = sampleRate;
        delayBuffer.clear();
        reverb.setSampleRate(sampleRate);
    }

    void releaseResources() override {}

    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override
    {
        processEcho(buffer);
        processReverb(buffer);
        processNoiseGate(buffer);
    }

    juce::AudioProcessorEditor* createEditor() override { return new juce::GenericAudioProcessorEditor(*this); }
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "Audio Effects Plugin"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock& destData) override
    {
        juce::MemoryOutputStream(destData, true).writeFloat(*parameters.getRawParameterValue("delayTime"));
    }

    void setStateInformation(const void* data, int sizeInBytes) override
    {
        juce::MemoryInputStream stream(data, static_cast<size_t> (sizeInBytes), false);
        *parameters.getRawParameterValue("delayTime") = stream.readFloat();
    }

private:
    juce::AudioProcessorValueTreeState parameters;
    juce::AudioBuffer<float> delayBuffer;
    int delayIndex = 0;
    double sampleRate = 44100.0;

    juce::Reverb reverb;

    void processEcho(juce::AudioBuffer<float>& buffer)
    {
        auto delayTime = parameters.getRawParameterValue("delayTime")->load();
        auto feedback = parameters.getRawParameterValue("feedback")->load();
        const int delaySamples = static_cast<int>(sampleRate * delayTime);
        auto numSamples = buffer.getNumSamples();
        auto numChannels = buffer.getNumChannels();

        for (int channel = 0; channel < numChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer(channel);
            auto* delayData = delayBuffer.getWritePointer(jmin(channel, delayBuffer.getNumChannels() - 1));

            for (int i = 0; i < numSamples; ++i)
            {
                float in = channelData[i];
                float delayedSample = delayData[delayIndex];
                channelData[i] += delayedSample * feedback;
                delayData[delayIndex] = in + delayedSample * feedback;

                delayIndex = (delayIndex + 1) % delayBuffer.getNumSamples();
            }
        }
    }

    void processReverb(juce::AudioBuffer<float>& buffer)
    {
        juce::Reverb::Parameters reverbParams;
        reverbParams.roomSize = parameters.getRawParameterValue("roomSize")->load();
        reverbParams.damping = 0.5f;
        reverbParams.wetLevel = 0.3f;
        reverb.setParameters(reverbParams);

        reverb.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples());
    }

    void processNoiseGate(juce::AudioBuffer<float>& buffer)
    {
        const float threshold = parameters.getRawParameterValue("threshold")->load();
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            float* channelData = buffer.getWritePointer(channel);
            for (int i = 0; i < buffer.getNumSamples(); ++i)
            {
                if (std::abs(channelData[i]) < threshold)
                {
                    channelData[i] = 0.0f;
                }
            }
        }
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioEffectsProcessor)
};

//==============================================================================
class AudioEffectsProcessorPlugin : public juce::AudioProcessor
{
public:
    static juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
    {
        return new AudioEffectsProcessor();
    }
};
