#pragma once

#include "../llsocket.h"

#ifdef WIN32
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

using socket_t = SOCKET;
static constexpr const socket_t invalid_socket{ INVALID_SOCKET };
#else
using socket_t = i32;
static constexpr const socket_t invalid_socket{ -1 };
#endif // WIN32


namespace utl
{
	namespace networking
	{
		class bsd_socket : public low_level_socket_interface
		{
		public:
			bsd_socket();
			~bsd_socket();

			llsocket_error connect(const ipv4_address& address, u16 port, u32 flags) override;
			llsocket_error connect(const ipv6_address& address, u16 port, u32 flags) override;
			llsocket_error bind(const ipv4_address& address) override;
			llsocket_error bind(const ipv6_address& address) override;
			llsocket_error listen(i32 backlog) override;
			low_level_socket_interface* accept(ipv4_address& connection_address) override;
			low_level_socket_interface* accept(ipv6_address& connection_address) override;
			llsocket_error close() override;
			i64 send(const void* buffer, u64 length) override;
			i64 receive(void* buffer, u64 length) override;
			llsocket_error set_flag(u32 flags) override;
		protected:
			llsocket_error init_connection(struct addrinfo* address_info);
		private:
			socket_t m_socket{ invalid_socket };
		};
	}
}