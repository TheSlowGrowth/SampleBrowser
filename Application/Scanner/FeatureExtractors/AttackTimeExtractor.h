#pragma once
#include "JuceHeader.h"
#include "../FeatureExtractor.h"

class AttackTimeExtractor : public FeatureExtractor
{
public:
    AttackTimeExtractor();
    float extractFeature(const FeatureExtractor::SourceData& sourceData) override;
};