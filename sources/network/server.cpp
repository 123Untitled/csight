#include "cs/network/server.hpp"
#include "cs/diagnostics/exception.hpp"
#include "cs/type_traits/move.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


#include <iostream>

#include "cs/network/dispatch.hpp"
#include "cs/network/address.hpp"


// get environ
extern char** environ;


auto open_browser(const ::in_port_t ___port) -> void {


	// search path in env PATH

	// fork
	::pid_t pid = ::fork();

	if (pid == -1) {
		perror("fork");
		throw cs::runtime_error{"failed to fork"};
	}

	// child
	if (pid == 0) {

		// 16-bit max digits (5)
		char ___url[] {
			'h', 't', 't', 'p', ':', '/', '/',
			'l', 'o', 'c', 'a', 'l', 'h', 'o', 's', 't', ':',
			static_cast<char>((___port / 10000)     + '0'),
			static_cast<char>((___port / 1000) % 10 + '0'),
			static_cast<char>((___port / 100)  % 10 + '0'),
			static_cast<char>((___port / 10)   % 10 + '0'),
			static_cast<char>((___port % 10)        + '0'),
			'\0'
		};

		char ___cmd[] = "/usr/bin/open";

		char* ___args[] = {___cmd, ___url, nullptr};

		if (::execl(___cmd, ___cmd, ___url, nullptr) == -1)
			perror("execvp");
		exit(EXIT_FAILURE);
	}

	// parent
	else {

		// wait for child
		int status;
		if (::waitpid(pid, &status, 0) == -1)
			perror("waitpid");

		if (WIFEXITED(status)) {
			if (WEXITSTATUS(status) != 0) {
				std::cerr << "failed to open browser" << std::endl;
				throw cs::runtime_error{"failed to open browser"};
			}
			else {
				std::cout << "browser opened" << std::endl;
			}
		}
		else {
			std::cerr << "failed to open browser" << std::endl;
			throw cs::runtime_error{"failed to open browser"};
		}

	}
}


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
	___dispatch::add<EVFILT_READ>(_dispatch, *this);
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
		//std::cout << "\x1b[32maccepting client\x1b[0m" << std::endl;

/* dispatch */
auto cs::server::dispatch(const int ___evnts) -> void {


	if (cs::is_read_event(___evnts)) {


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

		// non-blocking
		cs::non_blocking(___cli);

		// client get ownership
		_client.socket(cs::move(___cli));

		// add to dispatcher
		___dispatch::add<EVFILT_READ | EVFILT_WRITE
			| EVFILT_EXCEPT
			>(_dispatch, _client);

	}
}

/* descriptor */
auto cs::server::descriptor(void) const noexcept -> int {
	return _socket;
}
