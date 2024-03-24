#include "bsd_socket.h"

#include "../socket_manager.h"

using namespace utl::networking;

void set_sockaddr(struct sockaddr_in* addr, i32& family, const ipv4_address& ip, uint16_t port) 
{
	family = addr->sin_family = AF_INET;
	addr->sin_port = htons(port);
	std::memcpy(&addr->sin_addr, &ip[0], sizeof(ipv4_address::base_type) * ipv4_address::element_count);
}

void set_sockaddr(struct sockaddr_in6* addr, i32& family, const ipv6_address& ip, uint16_t port)
{
	family = addr->sin6_family = AF_INET6;
	addr->sin6_port = htons(port);
	std::memcpy(&addr->sin6_addr, &ip[0], sizeof(ipv6_address::base_type) * ipv6_address::element_count);
}

template<class _AddressType, class _SockAddrType>
llsocket_error make_address_info(struct addrinfo** ainfo, const _AddressType& address, u16 port, u32 flags)
{
	struct addrinfo hints;
	_SockAddrType* addr{ nullptr };

	std::memset(&hints, 0, sizeof(hints));

	hints.ai_flags = AI_PASSIVE;

	if ((flags & llsocket_flags::tcp_mode) && (flags & llsocket_flags::udp_mode))
		return llsocket_error::invalid_parameters;

	if (flags & llsocket_flags::tcp_mode)
	{
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
	}
	else if (flags & llsocket_flags::udp_mode)
	{
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_protocol = IPPROTO_UDP;
	}

	*ainfo = static_cast<struct addrinfo*>(malloc(sizeof(struct addrinfo)));
	if (!(*ainfo))
		return llsocket_error::invalid_address;

	addr = static_cast<_SockAddrType*>(malloc(sizeof(_SockAddrType)));
	if (!addr)
	{
		free(*ainfo);
		return llsocket_error::invalid_address;
	}

	std::memset(addr, 0, sizeof(_SockAddrType));
	set_sockaddr(addr, hints.ai_family, address, port);

	std::memcpy(*ainfo, &hints, sizeof(hints));

	(*ainfo)->ai_addr = (_SockAddrType*)addr;
	(*ainfo)->ai_addrlen = sizeof(_SockAddrType);

	return llsocket_error::ok;
}

bsd_socket::bsd_socket()
{
	socket_manager::getInstance(); 
}

bsd_socket::~bsd_socket()
{
	
}

llsocket_error bsd_socket::connect(const ipv4_address& address, u16 port, u32 flags)
{
	struct addrinfo* ainfo;
	auto error = make_address_info<ipv4_address, struct sockaddr_in>(&ainfo, address, port, flags);
	if (error != llsocket_error::ok)
		return error;

	return init_connection(ainfo);
}

llsocket_error bsd_socket::connect(const ipv6_address& address, u16 port, u32 flags)
{
	struct addrinfo* ainfo;
	auto error = make_address_info<ipv6_address, struct sockaddr_in6>(&ainfo, address, port, flags);
	if (error != llsocket_error::ok)
		return error;

	return init_connection(ainfo);
}

llsocket_error bsd_socket::bind(const ipv4_address& address)
{
	return llsocket_error::ok;
}

llsocket_error bsd_socket::bind(const ipv6_address& address)
{
	return llsocket_error::ok;
}

llsocket_error bsd_socket::listen(i32 backlog)
{
	return llsocket_error::ok;
}

low_level_socket_interface* bsd_socket::accept(ipv4_address& connection_address)
{
	auto* new_socket = static_cast<bsd_socket*>(socket_manager::getInstance()->alloc());

	return new_socket;
}

low_level_socket_interface* bsd_socket::accept(ipv6_address& connection_address)
{
	auto* new_socket = static_cast<bsd_socket*>(socket_manager::getInstance()->alloc());

	return new_socket;
}

llsocket_error bsd_socket::close()
{

	return llsocket_error::ok;
}

i64 bsd_socket::send(const void* buffer, u64 length)
{
	return ::send(m_socket, static_cast<const char*>(buffer), length, 0);
}

i64 bsd_socket::receive(void* buffer, u64 length)
{
	return ::recv(m_socket, static_cast<char*>(buffer), length, 0);
}

llsocket_error bsd_socket::set_flag(u32 flags)
{
	return llsocket_error::ok;
}

llsocket_error bsd_socket::init_connection(struct addrinfo* address_info)
{
	return llsocket_error::ok;
}