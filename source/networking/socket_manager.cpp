#include "socket_manager.h"

#if defined(_WIN32) || defined(__linux__)
#include "platform_socket/bsd_socket.h"

#ifdef _WIN32
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#endif // _WIN32

#endif // defined(_WIN32) || defined(__linux__)

using namespace utl::networking;

socket_manager::socket_manager()
{
#ifdef _WIN32
	WSADATA wsa_data;
	auto result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (result != 0)
		throw std::runtime_error("");
#endif // WIN32
}

socket_manager::~socket_manager()
{
#ifdef _WIN32
	WSACleanup();
#endif // WIN32

	for (auto& socket : m_vAllocatedSockets)
	{
		delete socket;
		socket = nullptr;
	}

	m_vAllocatedSockets.clear();
}

low_level_socket_interface* socket_manager::alloc()
{
#if defined(_WIN32) || defined(__linux__)
	return new bsd_socket();
#endif 
}

void socket_manager::dealloc(low_level_socket_interface** socket)
{
	auto found = std::remove_if(m_vAllocatedSockets.begin(), m_vAllocatedSockets.end(), [socket](low_level_socket_interface* allocated_socket) {return allocated_socket == *socket; });
	if (found == m_vAllocatedSockets.end())
		throw std::runtime_error("Trying to free pointer that was external allocated.");

	if (socket)
	{
		delete (*socket);
		*socket = nullptr;
	}

	m_vAllocatedSockets.erase(found);
}