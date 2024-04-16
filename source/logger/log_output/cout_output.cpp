#include "cout_output.h"
#include <logger/formatter.h>

#include <iostream>

using namespace utl::logger;

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