#pragma once
#include <filesystem>

namespace fs = std::filesystem;

class zipextractor
{
public:
    static void extract(const fs::path& from, const fs::path& to);
};