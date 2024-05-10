#include "cs/config.hpp"

#include "cs/core/parser.hpp"
#include "cs/html.hpp"
#include "cs/network/server.hpp"
#include "cs/diagnostics/exception.hpp"
#include "cs/network/dispatch.hpp"
#include "cs/browser.hpp"
#include "cs/system/input.hpp"

#include "cs/memory/unique_ptr.hpp"
#include "cs/hint.hpp"
#include "cs/map.hpp"
#include "cs/strlen.hpp"
#include "cs/string_view.hpp"

//#include "cs/http/parser.hpp"

//#include "cs/containers/array.hpp"

#include "cs/containers/pair.hpp"

#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <poll.h>

#include "cs/string.hpp"
#include "cs/system/open.hpp"
#include "cs/system/access.hpp"

#include "cs/system/cwd.hpp"
#include "cs/utility/hash.hpp"
#include <signal.h>
#include "cs/containers/hashmap.hpp"

// signal handler
void signal_handler(int sig) {
	cs::dispatch::stop();
	std::cout << "\x1b[1;31m\n\nSIGINT received\n\n\x1b[0m" << std::endl;
}

#include <string>
#include <chrono>
#include <unordered_map>

#if ___cs_requirements
int main(int ac, char** av) {

	using cmap = cs::hashmap<std::string, std::string>;
	using smap = std::unordered_map<std::string, std::string>;



	int iterations = 100000;

	// get time
	auto start = std::chrono::high_resolution_clock::now();
	{
		cmap cm;

		for (int i = 0; i < iterations; ++i) {
			cm["key" + std::to_string(i)] = "value" + std::to_string(i);
		}
	}
	auto end = std::chrono::high_resolution_clock::now();

	auto duration1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

	std::cout << "cmap: " << duration1.count() << std::endl;

	// get time
	start = std::chrono::high_resolution_clock::now();
	{
		smap sm;

		std::cout << "capacity: " << sm.bucket_count() << std::endl;

		for (int i = 0; i < iterations; ++i) {
			sm["key" + std::to_string(i)] = "value" + std::to_string(i);
		}
		end = std::chrono::high_resolution_clock::now();
	}

	auto duration2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

	std::cout << "smap: " << duration2.count() << std::endl;









	return 0;


	// signal handler
	if (signal(SIGINT, signal_handler) == SIG_ERR) {
		perror("signal");
		return 1;
	}
	if (signal(SIGTERM, signal_handler) == SIG_ERR) {
		perror("signal");
		return 1;
	}
	if (signal(SIGQUIT, signal_handler) == SIG_ERR) {
		perror("signal");
		return 1;
	}


	try {

		if (::isatty(STDIN_FILENO)) {
			cs::hint::info("no data to parse");
			return 1;
		}

		//cs::reader<int> rd;

		cs::core::map m;
		cs::descriptor dsc{STDIN_FILENO};



		cs::reader<4096U> rd;


		cs::parser prs;

		do {
			rd.read(dsc);
			prs.parse(rd, m);

		} while (not rd.eof());

		if (m.empty()) {
			cs::hint::info("no issues found");
			return 0;
		}


		m.for_each([&](const cs::core::entry& e) {

			std::cout << "\x1b[1;31m------------------- [\x1b[0m"
					  << e.file()
					  << "\x1b[1;31m] -------------------\x1b[0m\n\n";

			e.issues().for_each([](const cs::core::issue& i) {
				i.print();
				std::cout << std::endl;
			});
			std::cout << std::endl;
		});

		//return 0;

		//m.debug();

		cs::generate_json(m);

		//cs::generate_html(___lst);

		// add server and stdin
		cs::dispatch::add(
				cs::make_unique<cs::server>()
				//cs::make_unique<cs::input>()
		);

		cs::open_browser(8080);

		cs::dispatch::run();

		return 0;


	}

	catch (const cs::exception& e) {
		std::cerr << e.what() << std::endl;
		perror("error");
	}

	catch (...) {
		std::cerr << "unknown error" << std::endl;
		perror("error");
	}

	return 0;
}

#endif // ___cs_requirements
