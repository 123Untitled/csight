#include "cs/network/client.hpp"
#include "cs/network/dispatch.hpp"
#include "cs/type_traits/move.hpp"

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

const char _index_html[] {
R"(
<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<title>Compilation Errors</title>
	<style>
		body {
			font-family: Arial, sans-serif;
		}
		.error {
			margin-bottom: 20px;
		}
		.menu {
			background: #f0f0f0;
			padding: 10px;
		}
		select {
			padding: 5px;
			margin-right: 10px;
		}
		pre {
			background-color: #eeeeee;
			padding: 10px;
		}
	</style>

</head>
<body>
	<h1>Compilation Errors</h1>
	<div class="menu">
		<label for="fileSelect">Choose a file:</label>
		<select id="fileSelect" onchange="showErrors(this.value);">
			<option value="">Select a file</option>
		</select>
	</div>
	<script>
		function showErrors(fileName) {
			var errors = document.querySelectorAll('.errors');
			errors.forEach(e => e.style.display = 'none');
			if (fileName) {
				document.getElementById(fileName).style.display = 'block';
			}
		}
	</script>
</body>
</html>
)"
};

// tmp: to be implemented in another file
auto cs::client::serve_file(const std::string& path,
							const std::string& content_type) -> void {

	std::string content = read_file(path);

	if (content.empty())
		content = "HTTP/1.1 404 Not Found\n\n404 Not Found";

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

auto cs::client::serve_index(void) -> void {

	std::string content = _index_html;

	if (content.empty())
		content = "HTTP/1.1 404 Not Found\n\n404 Not Found";

	else {
		std::stringstream response;
		// 200 OK
		response << "HTTP/1.1 200 OK\n";
		// Content-Type
		response << "Content-Type: " << "text/html" << "\n";
		// cache-control
		response << "Cache-Control: no-cache, no-store, must-revalidate\n";
		// Content-Length
		response << "Content-Length: " << content.size() << "\n\n";

		// content
		response << content;

		content = response.str();
	}

	::write(_socket, content.data(), content.size());
}

// tmp: to be implemented in another file
auto generate_favicon_png(void) -> std::string {
	std::string fav;
	fav.reserve(64 * 64 * 4);

	for (int y = 0; y < 64; y++) {
		for (int x = 0; x < 64; x++) {
			if (x < 32 && y < 32) {
				fav.push_back((char)0x00);
				fav.push_back((char)0x00);
				fav.push_back((char)0x00);
				fav.push_back((char)0xFF);
			} else {
				fav.push_back((char)0xFF);
				fav.push_back((char)0xFF);
				fav.push_back((char)0xFF);
				fav.push_back((char)0xFF);
			}
		}
	}

	return fav;
}

// -- public lifecycle --------------------------------------------------------

/* socket constructor */
cs::client::client(cs::socket&& ___so) noexcept
: _socket{cs::move(___so)} {
}


// -- public modifiers --------------------------------------------------------

/* socket */
auto cs::client::socket(cs::socket&& ___so) noexcept -> void {
	_socket = cs::move(___so);
}

// -- public overriden methods ------------------------------------------------

/* dispatch */
auto cs::client::dispatch(const int ___evnts) -> void {

	std::string buffer{};

	if (___evnts & EVFILT_READ) {

		std::cout << "\x1b[32mreading from client\x1b[0m" << std::endl;

		char buff[1024];

		while (true) {

			const auto ret = ::recv(_socket, buff, sizeof(buff), 0);

			if (ret == -1)
				throw cs::runtime_error{"failed to read from client"};

			if (ret == 0) {
				std::cout << "\x1b[31mclient disconnected\x1b[0m" << std::endl;
				cs::io_event::register_for_removal(*this);
				return;
			}

			buffer.append(buff, (size_t)ret);

			if ((size_t)ret < sizeof(buff))
				break;
		}

		std::cout << buffer << std::endl;



		//std::cout << "\x1b[32mwriting to client\x1b[0m" << std::endl;


		std::string path;

		if (buffer.find("GET / ") != std::string::npos) {
			path = "site/index.html";
			serve_file(path, "text/html");
		}

		else if (buffer.find("GET /style.css ") != std::string::npos) {
			path = "site/style.css";
			serve_file(path, "text/css");
		}

		else if (buffer.find("GET /issues2.json ") != std::string::npos) {
			path = "site/issues2.json";
			serve_file(path, "application/json");
		}

		//else if (buffer.find("GET /favicon.png ") != std::string::npos) {
		//	std::string fav = generate_favicon_png();
		//	::write(_socket, fav.c_str(), fav.size());
		//}
	}

	else {
		std::cout << "\x1b[31munknown event\x1b[0m" << std::endl;
	}

	//if (count == 2) {
	//	cs::shutdown(_socket, SHUT_RDWR);
	//}
}

/* descriptor */
auto cs::client::descriptor(void) const noexcept -> int {
	return _socket;
}
