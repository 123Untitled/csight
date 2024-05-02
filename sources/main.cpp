#include "cs/config.hpp"

#include "cs/core/parser.hpp"
#include "cs/html.hpp"
#include "cs/network/server.hpp"



#include "cs/containers/array.hpp"


#if ___cs_requirements
int main(int ac, char** av) {

	try {

		constexpr cs::array<int, 10> arr{2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

		for (cs::size_t i = 0U; i < arr.size(); ++i)
			std::cout << arr._data[i] << std::endl;

		return 0;

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
