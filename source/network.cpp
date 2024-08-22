#include "network.hpp"

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996) 
#endif

using namespace utl;

struct FPlatformSocketData
{
#ifdef _WIN32
	WSADATA ws;
	SOCKET sock;
#else
	int32_t sock;
#endif
};

class socket::native_socket
{
public:
private:
};