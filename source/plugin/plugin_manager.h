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

			const stl::unique_ptr<uplugin>& load(const char* name, const stl::string& path);
			void unload(const char* name);

			void unloadAll();
		private:
			stl::unique_ptr<filesystem::filesystem_wrapper> m_pFSWrapper{};
			stl::unordered_map<u64, stl::unique_ptr<uplugin>> m_pluginMap{};
		};
	}
}