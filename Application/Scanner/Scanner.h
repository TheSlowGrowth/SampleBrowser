#include "JuceHeader.h"

class Scanner
{
public:
    struct ScanResults
    {
        File filePath;
        double lengthInSeconds;
        double sampleRate;
        struct Properties 
        {
            double percussiveness;
            double colour;
            // wip
        } properties;
    };

    class ResultReceiver
    {
    public:
        virtual ~ResultReceiver() {}
        virtual void scanCompleted(const ScanResults& results) = 0;
    };

    Scanner(ResultReceiver& receiver);
    ~Scanner();

    void scanFile(File filePath);

private:
    JUCE_DECLARE_NON_COPYABLE(Scanner);
    class Impl;
    std::unique_ptr<Impl> impl_;
};