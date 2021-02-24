#include "Scanner/FeatureExtractors/AttackTimeExtractor.h"
#include <gtest/gtest.h>

class AttackTimeExtractor_Fixture : public ::testing::Test
{
protected:
    AttackTimeExtractor_Fixture()
    {
    }

    AttackTimeExtractor extractor_;
};
