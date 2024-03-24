#pragma once

#include "address.h"

namespace utl
{
	namespace networking
	{
		enum class llsocket_error
		{
			ok,
			connection_failed,
			binding_failed,
			listening_failed,
			closing_failed,
			invalid_parameters,
			invalid_address
		};

		enum llsocket_flags
		{
			non_blocking = 1 << 0,
			tcp_mode = 1 << 1,
			udp_mode = 1 << 2
		};

		class low_level_socket_interface
		{
		public:
			virtual ~low_level_socket_interface() = default;

			virtual llsocket_error connect(const ipv4_address& address, u16 port, u32 flags) = 0;
			virtual llsocket_error connect(const ipv6_address& address, u16 port, u32 flags) = 0;
			virtual llsocket_error bind(const ipv4_address& address) = 0;
			virtual llsocket_error bind(const ipv6_address& address) = 0;
			virtual llsocket_error listen(i32 backlog) = 0;
			virtual low_level_socket_interface* accept(ipv4_address& connection_address) = 0;
			virtual low_level_socket_interface* accept(ipv6_address& connection_address) = 0;
			virtual llsocket_error close() = 0;
			virtual i64 send(const void* buffer, u64 length) = 0;
			virtual i64 receive(void* buffer, u64 length) = 0;
			virtual llsocket_error set_flag(u32 flags) = 0;
		};
	}
}