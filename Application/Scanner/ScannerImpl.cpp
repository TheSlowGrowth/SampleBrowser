#include "ScannerImpl.h"

Scanner::Impl::Impl(ResultReceiver& receiver) :
    receiver_(receiver)
{
}

Scanner::Impl::~Impl()
{
}

void Scanner::Impl::scanFile(File filePath)
{
}
