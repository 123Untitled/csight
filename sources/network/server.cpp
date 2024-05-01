#include "cs/network/server.hpp"
#include "cs/diagnostics/exception.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdio>

#include "cs/network/dispatch.hpp"


// -- public lifecycle --------------------------------------------------------

/* default constructor */
cs::server::server(void)
: _running{false}, _socket{cs::make_socket<AF_INET>()} {

	int opt = 1;
	// Forcefully attaching socket to the port 8080
	if (setsockopt(_socket, SOL_SOCKET,
				SO_REUSEADDR /*| SO_REUSEPORT*/, &opt, sizeof(opt))) {
		perror("setsockopt");
		throw cs::runtime_error{"failed to set socket options"};
	}

	// create address
	struct sockaddr_in ___addr;

	// set address
	___addr.sin_family = AF_INET;
	___addr.sin_port = htons(8080);
	___addr.sin_addr.s_addr = INADDR_ANY;


	// bind
	if (bind(_socket, (struct sockaddr*)&___addr, sizeof(___addr)) == -1) {
		perror("bind");
		throw cs::runtime_error{"failed to bind socket"};
	}

	// listen
	if (::listen(_socket, 1) == -1) {
		perror("listen");
		throw cs::runtime_error{"failed to listen on socket"};
	}

}


// -- public overriden methods ------------------------------------------------

/* dispatch */
auto cs::server::dispatch(const int ___evnts) -> void {

	if (cs::is_read_event(___evnts)) {

		// address
		struct sockaddr_in ___addr{};

		// accept
		cs::descriptor ___cli = ::accept(_socket, (struct sockaddr*)&___addr, (socklen_t*)&___addr);

		// check for error
		if (not ___cli) {
			perror("accept");
			throw cs::runtime_error{"failed to accept client"};
		}

	}

}
