#include "logger.h"

using namespace utl::logger;

void CLogger::init(const stl::string& app_name, const stl::string& app_version)
{
    for (auto& output : vOutputs)
        output->init(app_name, app_version);
}

void CLogger::addOutput(stl::shared_ptr<COutputBase>&& output)
{
    vOutputs.emplace_back(stl::move(output));
}