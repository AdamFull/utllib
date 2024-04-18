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

			virtual bool exists(const std::string& path) = 0;
			virtual bool fread(const std::string& path, utl::vector<u8>& data) = 0;
			virtual bool fwrite(const std::string& path, const utl::vector<u8>& data) = 0;
			virtual u64 fsize(const std::string& path) = 0;
			virtual bool mkdir(const std::string & path) = 0;
		};

		class basic_filesystem_wrapper : public utl::filesystem::filesystem_wrapper
		{
		public:
			~basic_filesystem_wrapper() = default;

			bool exists(const std::string& path) override;
			bool fread(const std::string& path, utl::vector<u8>& data) override;
			bool fwrite(const std::string& path, const utl::vector<u8>& data) override;
			u64 fsize(const std::string& path) override;
			bool mkdir(const std::string& path) override;
		};
	}
}