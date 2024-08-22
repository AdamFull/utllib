#pragma once

namespace utl
{
	class socket
	{
	public:
		class native_socket;
		native_socket* _socket{ nullptr };
	};
}