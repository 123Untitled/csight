#include "cs/config.hpp"

//#include "cs/core/parser.hpp"
#include "cs/html.hpp"
#include "cs/network/server.hpp"
#include "cs/diagnostics/exception.hpp"
#include "cs/network/dispatch.hpp"
#include "cs/browser.hpp"
#include "cs/system/input.hpp"

#include "cs/memory/unique_ptr.hpp"


//#include "cs/containers/array.hpp"

#if ___cs_requirements
int main(int ac, char** av) {

	try {

		// add server and stdin
		cs::dispatch::add(cs::make_unique<cs::server>(),
						  cs::make_unique<cs::input>());

		cs::open_browser(8080);

		cs::dispatch::run();

		//auto ___lst = cs::parser::parse(STDIN_FILENO);

		//for (auto& ___iss : ___lst)
		//	___iss.print();

		//cs::generate_html(___lst);
		//cs::generate_js(___lst);


	}
	catch (const cs::exception& e) {
		std::cerr << e.what() << std::endl;
		perror("error");
	}





	return 0;
}

#endif // ___cs_requirements
