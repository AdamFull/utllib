#pragma once

#include "llsocket.h"
#include "../design_pattern/pattern.h"

namespace utl
{
	namespace networking
	{
		class socket_manager : public pattern::singleton<socket_manager>
		{
		public:
			socket_manager();
			~socket_manager();

			low_level_socket_interface* alloc();
			void dealloc(low_level_socket_interface** socket);

		private:
			std::vector<low_level_socket_interface*> m_vAllocatedSockets{};
		};
	}
}