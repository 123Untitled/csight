#include "cs/network/client.hpp"
#include "cs/network/dispatch.hpp"
#include "cs/type_traits/move.hpp"
#include "cs/hint.hpp"
#include "cs/reader.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

// tmp: to be implemented in another file
auto read_file(const std::string& path) -> std::string {
	std::ifstream file(path);
	if (file) {
		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();
		return content;
	}
	return "";
}

//static constexpr const char* files[] {
//	"/",
//	"/style.css",
//	"/issues.json",
//};



// tmp: to be implemented in another file
auto cs::client::serve_file(const std::string& path,
							const std::string& content_type) -> void {

	std::string content = read_file(path);


	if (content.empty()) {
		std::cout << "404 Not Found" << std::endl;
		content = "HTTP/1.1 404 Not Found\n\n404 Not Found";
	}

	else {
		std::stringstream response;
		// 200 OK
		response << "HTTP/1.1 200 OK\n";
		// Content-Type
		response << "Content-Type: " << content_type << "\n";
		// cache-control
		response << "Cache-Control: no-cache, no-store, must-revalidate\n";
		// Content-Length
		response << "Content-Length: " << content.size() << "\n\n";
		// Connection: close (
		//response << "Connection: close\n\n";
		response << content;
		content = response.str();
	}

	::write(_socket, content.data(), content.size());
}



// -- public lifecycle --------------------------------------------------------

/* socket constructor */
cs::client::client(cs::socket&& ___so) noexcept
: _socket{cs::move(___so)} {
}



// -- public overriden methods ------------------------------------------------

/* read */
auto cs::client::read(void) -> void {

	cs::hint::info("reading from client");

	// new reader
	cs::reader<BUFFER_SIZE> reader;

	// read from socket
	reader.read(_socket);

	//::write(STDOUT_FILENO, reader.data(), reader.size());
	//::write(STDOUT_FILENO, "\n", 1U);

	// check for disconnection
	if (reader.empty()) {
		cs::dispatch::remove(*this);
		cs::hint::success("client disconnected");
		return;
	}

	// parse request
	_parser.parse(_request, reader);

	if (_parser.complete()) {
		//_request.print();

		if (_request.method().type() == cs::http::method::GET) {
			std::string req{_request.method().uri().data(), _request.method().uri().size()};
			if (req == "/") {
				std::string path = "site/index.html";
				serve_file(path, "text/html");
			}
			else if (req == "/style.css") {
				std::string path = "site/style.css";
				serve_file(path, "text/css");
			}
			else if (req == "/issues.json") {
				std::cout << "sending issues.json" << std::endl;
				std::string path = "site/issues.json";
				serve_file(path, "application/json");
			}
			else {
				std::string content = "HTTP/1.1 404 Not Found\n\n404 Not Found";
				::write(_socket, content.data(), content.size());
			}
		}
		_request.clear();
		_parser.clear();
	}
}

/* send */
auto cs::client::send(void) -> void {
	cs::hint::info("writing to client");
}

/* descriptor */
auto cs::client::descriptor(void) const noexcept -> int {
	return _socket;
}
