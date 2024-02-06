#include "file_output.h"

using namespace utl::logger;

COutputFile::COutputFile(const stl::string& filepath)
{
    file.open(filepath, stl::ios_base::out | stl::ios_base::binary);
}

COutputFile::~COutputFile()
{
    if (file.is_open())
        file.close();
}

void COutputFile::log(const stl::string& message, ELogLevel eLevel)
{
    write(message);
}

void COutputFile::write(const stl::string& write)
{
    file << write << stl::endl;
    file.flush();
}