#include "win_output.h"

#include <logger/formatter.h>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace utl::logger;

COutputWinCmd::~COutputWinCmd()
{

}

void COutputWinCmd::log(const std::string& message, ELogLevel eLevel)
{
    write(message);
}

void COutputWinCmd::write(const std::string& write)
{
#ifdef _WIN32
    OutputDebugStringA((write + "\n").c_str());
#endif
}