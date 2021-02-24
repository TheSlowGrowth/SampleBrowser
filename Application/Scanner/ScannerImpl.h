#pragma once
#include "JuceHeader.h"
#include "Scanner.h"

class Scanner::Impl
{
public:
    Impl(ResultReceiver& receiver);
    ~Impl();

    void scanFile(File filePath);

private:
    ResultReceiver& receiver_;
};