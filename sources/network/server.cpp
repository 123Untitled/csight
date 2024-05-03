#include "cs/network/address.hpp"
#include "cs/network/server.hpp"
#include "cs/network/dispatch.hpp"
#include "cs/network/client.hpp"
#include "cs/type_traits/move.hpp"


#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


#include <iostream>



// -- public lifecycle --------------------------------------------------------

/* default constructor */
cs::server::server(void)
: server{static_cast<::in_port_t>(8080)} {
}

/* port constructor */
cs::server::server(const ::in_port_t ___port)
: /*_running{true}, _port{___port}, */
	_socket{cs::make_socket<AF_INET>()} {

	// reuse address
	cs::reuse_address(_socket);

	// non-blocking
	cs::non_blocking(_socket);

	// create address
	cs::address addr{::in_addr{INADDR_ANY}, ___port};

	// bind
	cs::bind(_socket, addr);

	// listen
	cs::listen(_socket, 1);

	std::cout << "\x1b[32mserver listening on port " << ___port << "\x1b[0m" << std::endl;

}


// -- public overriden methods ------------------------------------------------

/* read */
auto cs::server::read(void) -> void {

	std::cout << "\x1b[31mserver accepting client\x1b[0m" << std::endl;

	// address
	cs::address addr;

	// accept
	auto ___cli = cs::accept(_socket, addr);

	// shutdown if not localhost
	if (not addr.is_localhost()) {
		cs::shutdown(___cli, SHUT_RDWR);
		return;
	}

	std::cout << "new client: " << ___cli << std::endl;

	// non-blocking
	cs::non_blocking(___cli);

	// add to dispatcher
	cs::dispatch::add(
			cs::make_unique<cs::client>(
				cs::move(___cli))
	);
}

/* send */
auto cs::server::send(void) -> void {
}

/* descriptor */
auto cs::server::descriptor(void) const noexcept -> int {
	return _socket;
}
