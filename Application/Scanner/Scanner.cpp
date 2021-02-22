#include "Scanner.h"
#include "ScannerImpl.h"

Scanner::Scanner(ResultReceiver& receiver)
{
    impl_ = std::make_unique<Scanner::Impl>(receiver);
}

Scanner::~Scanner()
{
}

void Scanner::scanFile(File filePath)
{
    impl_->scanFile(filePath);
}