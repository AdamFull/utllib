#include "filesystem_wrapper.h"

using namespace utl::filesystem;

bool basic_filesystem_wrapper::exists(const stl::string& path)
{
	return stl::filesystem::exists(path);
}

bool basic_filesystem_wrapper::fread(const stl::string& path, stl::vector<u8>& data)
{
	std::ifstream instream(path, std::ios::in | std::ios::binary);
	std::copy(std::istreambuf_iterator<char>(instream), std::istreambuf_iterator<char>(), std::back_inserter(data));

	return instream.is_open() && !data.empty();
}

bool basic_filesystem_wrapper::fwrite(const stl::string& path, const stl::vector<u8>& data)
{
	std::ofstream outstream(path, std::ios::out | std::ios::binary);
	outstream.write(reinterpret_cast<const char*>(data.data()), data.size());

	return outstream.is_open() && outstream.good();
}

u64 basic_filesystem_wrapper::fsize(const stl::string& path)
{
	return std::filesystem::file_size(path);
}

bool basic_filesystem_wrapper::mkdir(const stl::string& path)
{
	return std::filesystem::create_directories(path);
}