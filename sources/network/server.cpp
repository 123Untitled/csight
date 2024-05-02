#include "cs/network/server.hpp"
#include "cs/diagnostics/exception.hpp"
#include "cs/type_traits/move.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


#include <iostream>

#include "cs/network/dispatch.hpp"
#include "cs/network/address.hpp"
#include "cs/browser.hpp"





// -- public lifecycle --------------------------------------------------------

/* default constructor */
cs::server::server(void)
: server{static_cast<::in_port_t>(8080)} {
}

/* port constructor */
cs::server::server(const ::in_port_t ___port)
: _running{true}, _port{___port}, _socket{cs::make_socket<AF_INET>()} {

	// reuse address
	cs::reuse_address(_socket);

	// non-blocking
	cs::non_blocking(_socket);

	// create address
	cs::address addr{::in_addr{INADDR_ANY}, _port};

	// bind
	cs::bind(_socket, addr);

	// listen
	cs::listen(_socket, 1);

	// add server to dispatcher
	___dispatch::add<cs::dispatch_event::EV_READ>(_dispatch, *this);
}


// -- public methods ----------------------------------------------------------

/* run */
auto cs::server::run(void) -> void {

	open_browser(_port);

	// wait
	while (_running) {
		//std::cout << "\x1b[33mwaiting for events\x1b[0m" << std::endl;
		_dispatch.wait();

		auto& ___lst = cs::io_event::remover();
		for (cs::vector<cs::io_event*>::size_type i = 0; i < ___lst.size(); ++i) {
			___dispatch::remove(_dispatch, *___lst[i]);
		}
		___lst.clear();
	}
}


// -- public overriden methods ------------------------------------------------

/* dispatch */
auto cs::server::dispatch(const cs::ev_flag ___ev) -> void {


	if (cs::ev_read(___ev)) {

		std::cout << "\x1b[32mserver accepting client\x1b[0m" << std::endl;

		// address
		cs::address addr;

		// accept
		auto ___cli = cs::accept(_socket, addr);

		// shutdown if not localhost
		if (not addr.is_localhost()) {
			::write(___cli, "wesh, t'es qui ?", 16);
			cs::shutdown(___cli, SHUT_RDWR);
			return;
		}

		std::cout << "new client: " << ___cli << std::endl;

		// non-blocking
		cs::non_blocking(___cli);

		// client get ownership
		_client.socket(cs::move(___cli));

		// add to dispatcher
		___dispatch::add<cs::dispatch_event::EV_READ
					   //| cs::dispatch_event::EV_WRITE
			>(_dispatch, _client);
	}
}

/* descriptor */
auto cs::server::descriptor(void) const noexcept -> int {
	return _socket;
}
