#include "cs/network/socket.hpp"
#include "cs/diagnostics/exception.hpp"
#include <fcntl.h>


/* bind */
auto cs::bind(const cs::socket& ___sock, const cs::address& ___addr) -> void {

	// bind socket to address
	const auto ret = ::bind(___sock, &___addr.as_sockaddr(), ___addr.size());

	if (ret != 0)
		throw cs::runtime_error{"failed to bind socket to address"};
}

/* listen */
auto cs::listen(const cs::socket& ___sock, const int ___backlog) -> void {

	// listen on socket
	const auto ret = ::listen(___sock, ___backlog);

	if (ret != 0)
		throw cs::runtime_error{"failed to listen on socket"};
}

/* accept */
auto cs::accept(const cs::socket& ___sock, cs::address& ___addr) -> cs::socket {

	// accept
	const auto ___ns = ::accept(___sock, &___addr.as_sockaddr(),
										 &___addr.size());

	// check for error
	if (___ns == cs::socket::INVALID_SOCKET)
		throw cs::runtime_error{"failed to accept client"};

	return cs::socket{___ns};
}

/* non-blocking */
auto cs::non_blocking(const cs::socket& ___so) -> void {

	const int state = ::fcntl(___so, F_SETFL, O_NONBLOCK);

	if (state == -1)
		throw cs::runtime_error{"non_blocking failed"};
}

/* blocking */
auto blocking(const cs::socket& ___so) -> void {

	const int state = ::fcntl(___so, F_SETFL, 0);

	if (state == -1)
		throw cs::runtime_error{"blocking failed"};
}

/* shutdown */
auto cs::shutdown(const cs::socket& ___so, const int ___how) -> void {

	const int state = ::shutdown(___so, ___how);

	if (state != 0)
		throw cs::runtime_error{"shutdown failed"};
}
/* reuse address */
auto cs::reuse_address(const cs::socket& ___so) -> void {

	int opt = 1;

	if (::setsockopt(___so, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw cs::runtime_error{"failed to set socket options"};
}



