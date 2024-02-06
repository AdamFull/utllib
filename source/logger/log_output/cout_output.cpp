#include "cout_output.h"
#include <logger/formatter.h>

#include <iostream>

using namespace utl::logger;

COutputCOUT::~COutputCOUT()
{

}

void COutputCOUT::log(const stl::string& message, ELogLevel eLevel)
{
#ifdef UTILITY_LOGGER_USE_COLORIZE_OUTPUT
    write(formatter::colorize(eLevel, message));
#else
    write(message);
#endif
}

void COutputCOUT::write(const stl::string& write)
{
    stl::cout << write << stl::endl;
}