#include "cs/config.hpp"

#include "cs/core/parser.hpp"
#include "cs/html.hpp"
#include "cs/network/server.hpp"



#include "cs/containers/array.hpp"

#define new_type(type1, type2) type1##type2


#if ___cs_requirements
int main(int ac, char** av) {

	typedef new_type(int32, _t) my_int;

	try {


		//auto ___lst = cs::parser::parse(STDIN_FILENO);

		//for (auto& ___iss : ___lst)
		//	___iss.print();


		//cs::generate_html(___lst);
		//cs::generate_js(___lst);

		//return 0;

		cs::server server{8080};
		server.run();


	} catch (const cs::exception& e) {
		std::cerr << e.what() << std::endl;
		perror("error");
	}





	return 0;
}

#endif // ___cs_requirements
