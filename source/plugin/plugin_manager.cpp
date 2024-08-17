#include "plugin_manager.h"

#include <hash.hpp>

using namespace utl::plugin_system;

uplugin_manager::uplugin_manager()
{

}

uplugin_manager::~uplugin_manager()
{
	unloadAll();
}

const std::unique_ptr<uplugin>& uplugin_manager::load(const char* name, const std::string& path)
{
	static std::unique_ptr<uplugin> empty{ nullptr };

	if (!m_pFSWrapper)
		m_pFSWrapper = std::make_unique<filesystem::basic_filesystem_wrapper>();

	utl::vector<u8> data{};
	if (m_pFSWrapper->fread(path, data))
	{
		auto new_plugin = std::make_unique<uplugin>();
		if (new_plugin->load(data))
		{
			auto name_hash = murmur_hash_64(name);
			m_pluginMap.emplace(name_hash, std::move(new_plugin));
			return m_pluginMap[name_hash];
		}
	}

	return empty;
}

void uplugin_manager::unload(const char* name)
{
	if (auto found = m_pluginMap.find(murmur_hash_64(name)); found != m_pluginMap.end())
	{
		found->second->unload();
		m_pluginMap.erase(found);
	}
}

void uplugin_manager::unloadAll()
{
	for (auto& [name_hash, plugin] : m_pluginMap)
		plugin->unload();
	m_pluginMap.clear();
}