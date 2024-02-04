#include "OutputCOUT.h"

#include <iostream>
#include "formatter.h"

using namespace utl;

COutputCOUT::~COutputCOUT()
{

}

void COutputCOUT::log(const std::string& message, ELogLevel eLevel)
{
#ifdef UTILITY_LOGGER_USE_COLORIZE_OUTPUT
    write(formatter::colorize(eLevel, message));
#else
    write(message);
#endif
}

void COutputCOUT::write(const std::string& write)
{
    std::cout << write << std::endl;
}