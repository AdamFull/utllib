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

			const stl::unique_ptr<uplugin>& load(const stl::string& path);
			void unloadAll();
		private:
			stl::unique_ptr<filesystem::filesystem_wrapper> m_pFSWrapper{};
			std::vector<stl::unique_ptr<uplugin>> m_vLoadedPluggins{};
		};
	}
}