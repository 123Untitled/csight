#include "cs/browser.hpp"
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "cs/diagnostics/exception.hpp"
#include <cstdlib>
#include <iostream>

// get environ
extern char** environ;


auto cs::open_browser(const ::in_port_t ___port) -> void {


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

