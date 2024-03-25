#include "tcp_socket.h"

#include "socket_manager.h"

using namespace utl::networking;

tcp_socket::tcp_socket()
{
	m_pSocket = socket_manager::getInstance()->alloc();
}

tcp_socket::~tcp_socket()
{
	if (m_pSocket)
		socket_manager::getInstance()->dealloc(&m_pSocket);
}

// TODO: Memory leak
tcp_socket_error tcp_socket::connect(ipv4_address& address, u16 port, u32 flags)
{
	auto error = m_pSocket->connect(address, port, llsocket_flags::tcp_mode);
	if (error != llsocket_error::ok)
		return tcp_socket_error::failed_connect_to_endpoint;

	return process_connection(flags);
}

// TODO: Memory leak
tcp_socket_error tcp_socket::connect(ipv6_address& address, u16 port, u32 flags)
{
	auto error = m_pSocket->connect(address, port, llsocket_flags::tcp_mode);
	if (error != llsocket_error::ok)
		return tcp_socket_error::failed_connect_to_endpoint;

	return process_connection(flags);
}

tcp_socket_error tcp_socket::bind(ipv4_address& address, u16 port, u32 flags)
{

}

tcp_socket_error tcp_socket::bind(ipv6_address& address, u16 port, u32 flags)
{

}

tcp_socket_error tcp_socket::write_until(const std::string& buffer)
{
	u64 bytes_writen{ 0ull };

	u64 bytes_to_write{ buffer.size() > chunk_size ? chunk_size : buffer.size() };
	while (i64 bytes_write = m_pSocket->send(&buffer[bytes_writen], bytes_to_write) > 0)
	{
		bytes_writen += bytes_write;

		u64 bytes_left = buffer.size() - bytes_writen;
		bytes_to_write = bytes_left > chunk_size ? chunk_size : bytes_left;
	}

	return tcp_socket_error::ok;
}

tcp_socket_error tcp_socket::read_until(std::string& buffer, i8 terminate_char)
{
	while (true)
	{
		char current_char;
		i64 bytes_read = m_pSocket->receive(&current_char, 1ull);
		if (bytes_read <= 0ll)
			return tcp_socket_error::failed_to_read_from_socket;

		buffer.push_back(current_char);

		if (current_char == terminate_char)
			break;
	}

	return tcp_socket_error::ok;
}

low_level_socket_interface* tcp_socket::get_low_level_socket() const 
{ 
	return m_pSocket; 
}

tcp_socket_error tcp_socket::process_connection(u32 flags)
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