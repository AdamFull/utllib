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
			static constexpr u64 chunk_size{ 65535 };

			tcp_socket_error connect(ipv4_address& address, u32 flags)
			{
				auto error = m_pSocket->connect(address, llsocket_flags::tcp_mode);
				if (error != llsocket_error::ok)
					return tcp_socket_error::failed_connect_to_endpoint;

				return process_connection(flags);
			}

			tcp_socket_error connect(ipv6_address& address, u32 flags)
			{
				auto error = m_pSocket->connect(address, llsocket_flags::tcp_mode);
				if (error != llsocket_error::ok)
					return tcp_socket_error::failed_connect_to_endpoint;

				return process_connection(flags);
			}

			tcp_socket_error read_until(std::string& buffer, char terminate_char)
			{
				u64 buf_idx{ 0ull };
				bool is_buffer_overflow{ false };
				bool is_terminate_symbol_found{ false };

				while (!is_terminate_symbol_found)
				{
					while (buf_idx < chunk_size && )
					{
						i64 bytes_read = m_pSocket->receive(&m_chunkBuffer[buf_idx], 1ull);
						if (bytes_read <= 0ll)
							return tcp_socket_error::failed_to_read_from_socket;

						const auto& current_char = m_chunkBuffer[buf_idx];

						if (buf_idx > 0 && current_char == terminate_char)
						{
							is_terminate_symbol_found = true;
							break;
						}

						buf_idx++;
					}

					if (buf_idx >= chunk_size || buf_idx > 0)
					{
						buffer.insert(buffer.end(), &m_chunkBuffer[0ull], &m_chunkBuffer[0ull] + buf_idx);
						buf_idx = 0ull;
					}
				}

				return tcp_socket_error::ok;
			}

			const std::unique_ptr<low_level_socket_interface>& get_low_level_socket() const { return m_pSocket; }
		protected:
			tcp_socket_error process_connection(u32 flags)
			{
				llsocket_error error{};

				if (flags & tcp_socket_flags::can_listen)
				{
					error = m_pSocket->listen(1024);
					if (error != llsocket_error::ok)
						return tcp_socket_error::failed_to_enable_listening;
				}

				return tcp_socket_error::ok;
			}
		private:
			std::unique_ptr<low_level_socket_interface> m_pSocket{};
			std::array<u8, chunk_size> m_chunkBuffer{};
		};
	}
}