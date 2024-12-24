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

			bool load(const std::vector<uint8_t>& data);
			bool unload();

			template<class _Signature>
			_Signature getFunctionHandle(const char* function_name)
			{
				return reinterpret_cast<_Signature>(static_cast<void*>(getFunctionAddress(function_name)));
			}

			template<class _Signature>
			_Signature getFunctionHandle(uint64_t function_index)
			{
				return reinterpret_cast<_Signature>(static_cast<void*>(getFunctionAddress(function_index)));
			}

			intptr_t getFunctionAddress(const char* function_name);
			intptr_t getFunctionAddress(uint64_t function_index);

			// Find function
		private:
#ifdef _WIN32
			LPVOID m_pModule{ nullptr };
#endif
			std::unordered_map<uint64_t, uint64_t> m_tableMap{};
			std::vector<intptr_t> m_vFunctionTable{};
		};
	}
}