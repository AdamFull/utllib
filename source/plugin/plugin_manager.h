#pragma once

#include "plugin.h"
#include <filesystem/filesystem_wrapper.h>

namespace utl
{
	namespace plugin_system
	{
		class uplugin_manager
		{
		public:
			uplugin_manager();
			~uplugin_manager();

			const std::unique_ptr<uplugin>& load(const char* name, const std::string& path);
			void unload(const char* name);

			void unloadAll();
		private:
			std::unique_ptr<filesystem::filesystem_wrapper> m_pFSWrapper{};
			std::unordered_map<u64, std::unique_ptr<uplugin>> m_pluginMap{};
		};
	}
}