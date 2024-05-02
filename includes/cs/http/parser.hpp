#ifndef ___CS_HTTP_PARSER_HPP___
#define ___CS_HTTP_PARSER_HPP___

#include "cs/config.hpp"
#if ___cs_requirements




// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- H T T P  N A M E S P A C E ------------------------------------------

	namespace http {


		// -- P A R S E R -----------------------------------------------------

		class parser final {

			public:

			private:

			// ascii special characters for HTTP parsing
			// 32 space
			// \r
			// \n
			// : split header key and value
			// / path separator
			// ? query separator
			// & string split
			// = assocation between key and value
			// + space in encoded url
			// % escape percent encoding in url
			// [] ipv6 address
			// . domain separator and decimal point
			// - domain separator and specific range
			// " quote for string
			// , separator for lists


			// REQUEST METHODS



		}; // class parser

	} // namespace http

} // namespace cs

#endif // ___cs_requirements

#endif // ___CS_HTTP_PARSER_HPP___
