#pragma once
#include "JuceHeader.h"

class AmplitudeEnvelope
{
public:
    AmplitudeEnvelope(std::vector<float> amplitudes, double sampleRateInHz) :
        amplitudes_(amplitudes),
        sampleRate_(sampleRateInHz)
    {
    }

    void append(juce::dsp::AudioBlock<const float> sourceAudio, double sourceSampleRate)
    {
        // add initial zero
        if (amplitudes_.empty())
            amplitudes_.push_back(0.0f);

        // initialize RMS detector
        float initialRmsValue = amplitudes_.back();
        RmsDetector<float> rmsDetector(float(sourceSampleRate), initialRmsValue);

        // reserve memory
        const int numChannels = int(sourceAudio.getNumChannels());
        const int numSamples = int(sourceAudio.getNumSamples());
        const auto decimationFactor = sourceSampleRate / sampleRate_;
        const int numSamplesRequiredAdditionally = int(std::ceil(float(numSamples) / decimationFactor));
        amplitudes_.reserve(amplitudes_.size() + numSamplesRequiredAdditionally);

        // extract rms values
        const float numChannelsInv = 1.0f / float(numChannels);
        auto decimationCounter = 0.0;
        for (int i = 0; i < numSamples; i++)
        {
            // Get squared average across all channels
            float sqAvg = 0.0f;
            for (int ch = 0; ch < numChannels; ch++)
            {
                const auto sample = sourceAudio.getSample(ch, i);
                sqAvg += sample * sample;
            }
            sqAvg *= numChannelsInv;

            // get RMS value
            const auto rmsSq = rmsDetector.getNextRmsSq(sqAvg);

            // should we store this value?
            decimationCounter += 1.0;
            if (decimationCounter >= decimationFactor)
            {
                amplitudes_.push_back(std::sqrt(rmsSq));
                decimationCounter -= decimationFactor;
            }
        }
    }

    float getInterpolatedAmplitudeAt(float timeInS) const
    {
        const auto indexFractional = float(timeInS * sampleRate_);
        const auto indexInt = size_t(std::floor(indexFractional));
        if (indexInt <= amplitudes_.size() - 1)
        {
            // can interpolate a result
            const auto a = amplitudes_[indexInt];
            const auto b = amplitudes_[indexInt + 1u];
            auto frac = indexFractional - float(indexInt);
            return a * (1.0f - frac) + b * frac;
        }
        return std::numeric_limits<float>::quiet_NaN();
    }

    float getMaximumAmplitude() const
    {
        return juce::FloatVectorOperations::findMaximum(amplitudes_.data(), int(amplitudes_.size()));
    }

    double getSampleRateInHz() const { sampleRate_; }
    float getLengthInS() const { float(amplitudes_.size() / sampleRate_); }

private:
    template <typename FloatType>
    class RmsDetector
    {
    public:
        RmsDetector(FloatType sampleRate, FloatType initialValue = FloatType(0), FloatType timeConstantInS = FloatType(0.005)) :
            sampleRate_(sampleRate),
            lastRmsSq_(initialValue * initialValue),
            // according to Udo Zoelzer: DAFX, 2nd. edition, chapter 4.2, p. 109
            tau_(FloatType(1) - std::exp(FloatType(-2.2) / timeConstantInS / sampleRate))
        {
        }

        FloatType getNextRmsSq(FloatType inputSq)
        {
            const auto rmsSq = (inputSq - lastRmsSq_) * tau_ + lastRmsSq_;
            lastRmsSq_ = rmsSq;
            return rmsSq;
        }

    private:
        FloatType sampleRate_;
        FloatType lastRmsSq_;
        FloatType tau_;
    };

    std::vector<float> amplitudes_;
    double sampleRate_;
};
