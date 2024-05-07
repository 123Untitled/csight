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

#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <poll.h>

#include "cs/string.hpp"
#include "cs/system/open.hpp"
#include "cs/system/access.hpp"

#include "cs/system/cwd.hpp"
#include "cs/utility/hash.hpp"

#if ___cs_requirements
int main(int ac, char** av) {


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
		<< e.file() << "\x1b[1;31m] -------------------\x1b[0m\n\n";

			e.issues().for_each([](const cs::core::issue& i) {
				i.print();
				std::cout << std::endl;
			});
			std::cout << std::endl;
		});

		return 0;

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
