#pragma once

#include "llsocket.h"

namespace utl
{
	namespace networking
	{
		enum class tcp_socket_error
		{
			ok,
			failed_connect_to_endpoint,
			failed_to_enable_listening,
			failed_to_read_from_socket
		};

		enum tcp_socket_flags
		{
			can_listen = 1 << 0
		};

		// High level socket implementation
		class tcp_socket
		{
		public:
			static constexpr u64 chunk_size{ 65000 };

			tcp_socket();
			~tcp_socket();

			tcp_socket_error connect(ipv4_address& address, u16 port, u32 flags);
			tcp_socket_error connect(ipv6_address& address, u16 port, u32 flags);

			tcp_socket_error bind(ipv4_address& address, u16 port, u32 flags);
			tcp_socket_error bind(ipv6_address& address, u16 port, u32 flags);

			tcp_socket_error write_until(const std::string& buffer);
			tcp_socket_error read_until(std::string& buffer, i8 terminate_char);

			low_level_socket_interface* get_low_level_socket() const;
		protected:
			tcp_socket_error process_connection(u32 flags);
		private:
			low_level_socket_interface* m_pSocket{ nullptr };
			std::array<u8, chunk_size> m_chunkBuffer{};
		};
	}
}