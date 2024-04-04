#pragma once

#include <standart_library.h>

#ifdef _WIN32
#include <windows.h>
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

			template<class _Signature>
			_Signature getFunctionHandle(const char* function_name)
			{
				return reinterpret_cast<_Signature>(static_cast<void*>(getFunctionAddress(function_name)));
			}

			template<class _Signature>
			_Signature getFunctionHandle(u64 function_index)
			{
				return reinterpret_cast<_Signature>(static_cast<void*>(getFunctionAddress(function_index)));
			}

			intptr_t getFunctionAddress(const char* function_name);
			intptr_t getFunctionAddress(u64 function_index);

			// Find function
		private:
#ifdef _WIN32
			LPVOID m_pModule{ nullptr };
#endif
			stl::unordered_map<u64, u64> m_tableMap{};
			stl::vector<intptr_t> m_vFunctionTable{};
		};
	}
}