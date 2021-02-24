#pragma once
#include "JuceHeader.h"

class FeatureExtractor
{
public:
    virtual ~FeatureExtractor() {}

    struct SourceData
    {
    };
    
    virtual float extractFeature(const SourceData& sourceData) = 0;
};
