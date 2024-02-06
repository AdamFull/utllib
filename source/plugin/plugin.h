#pragma once

#include <standart_library.h>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace utl
{
	namespace plugin_system
	{
		class uplugin
		{
		public:
			~uplugin();

			bool load(const stl::vector<u8>& data);
			bool unload();
		private:
#ifdef _WIN32
			LPVOID m_pModule{ nullptr };
#endif

			stl::vector<intptr_t> m_vFunctionTable{};
		};
	}
}