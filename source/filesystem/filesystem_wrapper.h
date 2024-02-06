#pragma once

#include <standart_library.h>

namespace utl
{
	namespace filesystem
	{
		class filesystem_wrapper
		{
		public:
		public:
			virtual ~filesystem_wrapper() = default;

			virtual bool exists(const stl::string& path) = 0;
			virtual bool fread(const stl::string& path, stl::vector<u8>& data) = 0;
			virtual bool fwrite(const stl::string& path, const stl::vector<u8>& data) = 0;
			virtual u64 fsize(const stl::string& path) = 0;
			virtual bool mkdir(const stl::string & path) = 0;
		};

		class basic_filesystem_wrapper : public utl::filesystem::filesystem_wrapper
		{
		public:
			~basic_filesystem_wrapper() = default;

			bool exists(const stl::string& path) override;
			bool fread(const stl::string& path, stl::vector<u8>& data) override;
			bool fwrite(const stl::string& path, const stl::vector<u8>& data) override;
			u64 fsize(const stl::string& path) override;
			bool mkdir(const stl::string& path) override;
		};
	}
}