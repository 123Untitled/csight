#include "cs/config.hpp"
#include "cs/core/parser.hpp"
#include "cs/network/socket.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include "cs/html.hpp"
#include "cs/network/dispatch.hpp"
#include "cs/network/server.hpp"

std::string readFile(const std::string& path) {
	std::ifstream file(path);
	if (file) {
		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();
		return content;
	}
	return "";
}

void serveFile(int client_socket, const std::string& path, const std::string& content_type) {
	std::string content = readFile(path);
	if (content.empty()) {
		content = "HTTP/1.1 404 Not Found\n\n404 Not Found";
	} else {
		std::stringstream response;
		response << "HTTP/1.1 200 OK\n";
		response << "Content-Type: " << content_type << "\n";
		response << "Connection: close\n\n";
		response << content;
		content = response.str();
	}
	write(client_socket, content.c_str(), content.size());
}

void openBrowser(const std::string& url) {
#ifdef __APPLE__
	std::string command = "open " + url;  // Commande pour macOS
#elif __linux__
	std::string command = "xdg-open " + url;  // Commande pour Linux
#else
#error "Plateforme non supportée"
#endif

	// Lancer la commande
	if (system(command.c_str())) {
		std::cerr << "Erreur lors de l'ouverture du navigateur" << std::endl;
	}
}

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

bool running = true;

struct observer {
	auto dispatch(int) -> void {
		std::cout << "dispatch" << std::endl;
		char buffer[128];
		auto ret = ::read(0, buffer, 128);
		if (ret < 0)
			throw cs::runtime_error{"failed to read from descriptor"};
		if (ret)
			::write(1, buffer, (unsigned)ret);
		running = false;
	}
};

int main(int ac, char** av) {
#if ___cs_requirements

	using ___dispatch = cs::dispatch<observer>;

	try {

		cs::server server;

		observer obs;


		___dispatch dispatcher;

		cs::descriptor fd{STDIN_FILENO};

		___dispatch::add<EVFILT_READ>(dispatcher, obs, fd);


		while (running)
			dispatcher.wait();

	___dispatch::remove(dispatcher, fd);

	} catch (const cs::exception& e) {
		std::cerr << e.what() << std::endl;
		perror("error");
	}


	return 0;

	//auto ___lst = cs::parser::parse(STDIN_FILENO);
	//
	//for (auto& ___iss : ___lst)
	//	___iss.print();


	//cs::generate_html(___lst);
	//cs::generate_js(___lst);


	auto sock = cs::make_socket<AF_INET>();

	int opt = 1;
	// Forcefully attaching socket to the port 8080
	if (setsockopt(sock, SOL_SOCKET,
				SO_REUSEADDR /*| SO_REUSEPORT*/, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	// create address
	struct sockaddr_in ___addr;

	// set address
	___addr.sin_family = AF_INET;
	___addr.sin_port = htons(8080);
	___addr.sin_addr.s_addr = INADDR_ANY;


	// bind
	if (bind(sock, (struct sockaddr*)&___addr, sizeof(___addr)) == -1) {
		perror("bind");
		return EXIT_FAILURE;
	}

	// listen
	if (listen(sock, 5) == -1) {
		perror("listen");
		return EXIT_FAILURE;
	}

	int new_socket;
	openBrowser("http://localhost:8080");

	while ((new_socket = accept(sock, nullptr, nullptr))) {
		char buffer[1024] = {0};
		read(new_socket, buffer, 1023);
		std::string request(buffer);
		std::cout << request << std::endl;
		std::string path = "index.html";  // Default path

		if (request.find("GET / ") != std::string::npos) {
			path = "index.html";
			serveFile(new_socket, path, "text/html");
		}
		else if (request.find("GET /style.css ") != std::string::npos) {
			path = "style.css";
			serveFile(new_socket, path, "text/css");
		}
		else if (request.find("GET /favicon.png ") != std::string::npos) {
			std::string fav = generate_favicon_png();
			write(new_socket, fav.c_str(), fav.size());
		}
		close(new_socket);
	}




#endif // ___cs_requirements
	return 0;
}


	// OLD IMPLEMENTATION
	// no sync with stdio buffer
	//std::ios::sync_with_stdio(false);
	//// set full buffered stdout
	//std::setvbuf(stdout, nullptr, _IOFBF, BUFSIZ);
	//std::vector<std::string> files;
	//if (first_filter(files) != true) {
	//	std::cout << "ERROR: first filter failed" << std::endl;
	//	return EXIT_FAILURE; }
	//second_pass(files);
