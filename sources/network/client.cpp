#include "cs/network/client.hpp"
#include "cs/network/dispatch.hpp"
#include "cs/type_traits/move.hpp"
#include "cs/hint.hpp"
#include "cs/reader.hpp"
#include "cs/string.hpp"
#include "cs/system/open.hpp"
#include "cs/system/write.hpp"

#include <iostream>


// tmp: to be implemented in another file
auto read_file(const cs::string& path) -> cs::string {

	// open file
	auto fd = cs::open(path, O_RDONLY);

	cs::string content;

	// get file size
	const auto size = cs::file_size(fd);

	// read file
	content.resize(size);

	// read file
	const auto ___ret = ::read(fd, content.data(), size);

	if (___ret == -1 || static_cast<cs::size_t>(___ret) != size)
		throw cs::runtime_error("failed to read file");

	return content;
}


namespace cs{

namespace http {


	struct text_html {
		static constexpr cs::string_view view{"text/html"};
	};

	struct text_css {
		static constexpr cs::string_view view{"text/css"};
	};

	struct application_json {
		static constexpr cs::string_view view{"application/json"};
	};



	// tmp: to be implemented in another file
	template <typename ___content_type>
	auto serve(const cs::socket& ___so,
			   const cs::string& path) -> void {

		const cs::string content = read_file(path);

		cs::string resp;

		if (content.empty()) {
			cs::write(___so, "HTTP/1.1 404 Not Found\n\n404 Not Found");
			return;
		}

		resp.append(
				// 200 OK
				"HTTP/1.1 200 OK\r\n",
				// Content-Type
				"Content-Type: ", ___content_type::view, "\r\n",
				// cache-control
				"Cache-Control: no-cache, no-store, must-revalidate\r\n",
				// Content-Length
				"Content-Length: ", content.size(), "\r\n",
				// Connection: close
				"Connection: close\r\n\r\n",
				// content
				content
		);

		// send response
		cs::write(___so, resp);
	}

} // namespace http

} // namespace cs


// -- public lifecycle --------------------------------------------------------

/* socket constructor */
cs::client::client(cs::socket&& ___so) noexcept
: _socket{cs::move(___so)} {
}



// -- public overriden methods ------------------------------------------------

/* read */
auto cs::client::read(void) -> void {

	cs::hint::info("reading from client");

	// new buffer
	cs::reader<BUFFER_SIZE> reader;

	// read from socket
	reader.read(_socket);

	// check for disconnection
	if (reader.empty()) {
		cs::dispatch::remove(*this);
		cs::hint::success("client disconnected");
		return;
	}

	// parse buffer
	_parser.parse(_request, reader);

	if (not _parser.complete())
		return;

	___self::_handle_request();

	_request.clear();
	_parser.clear();
}

/* send */
auto cs::client::send(void) -> void {
	cs::hint::info("writing to client");
}

/* descriptor */
auto cs::client::descriptor(void) const noexcept -> int {
	return _socket;
}


// -- private methods ---------------------------------------------------------

/* handle request */
auto cs::client::_handle_request(void) -> void {

	if (_request.method().type() != cs::http::method::GET)
		return;

	std::string req{_request.method().uri().data(), _request.method().uri().size()};

	using namespace cs;

	if (req == "/") {
		http::serve<http::text_html>(
				_socket, cs::string{"site/index.html"});
	}
	else if (req == "/style.css") {
		http::serve<http::text_css>(
				_socket, cs::string{"site/style.css"});
	}
	else if (req == "/issues.json") {
		http::serve<http::application_json>(
				_socket, cs::string{"site/issues.json"});

	}
	else {
		cs::write(_socket, "HTTP/1.1 404 Not Found\n\n404 Not Found");
	}
}

