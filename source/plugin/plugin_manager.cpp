#include "plugin_manager.h"

using namespace utl::plugin_system;

uplugin_manager::uplugin_manager()
{

}

uplugin_manager::~uplugin_manager()
{
	unloadAll();
}

const stl::unique_ptr<uplugin>& uplugin_manager::load(const stl::string& path)
{
	static stl::unique_ptr<uplugin> empty{ nullptr };

	if (!m_pFSWrapper)
		m_pFSWrapper = stl::make_unique<filesystem::basic_filesystem_wrapper>();

	stl::vector<u8> data{};
	if (m_pFSWrapper->fread(path, data))
	{
		auto& new_plugin = m_vLoadedPluggins.emplace_back(stl::make_unique<uplugin>());
		if (!new_plugin->load(data))
		{
			m_vLoadedPluggins.pop_back();
			return empty;
		}

		return new_plugin;
	}

	return empty;
}

void uplugin_manager::unloadAll()
{
	for (auto& plugin : m_vLoadedPluggins)
		plugin->unload();
	m_vLoadedPluggins.clear();
}