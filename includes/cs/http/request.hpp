#ifndef ___CS_HTTP_REQUEST_HPP___
#define ___CS_HTTP_REQUEST_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/http/method.hpp"
#include "cs/http/header.hpp"
#include "cs/containers/list.hpp"




// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- H T T P  N A M E S P A C E ------------------------------------------

	namespace http {


		// -- R E Q U E S T ---------------------------------------------------

		class request final {


			private:

				// -- private types -------------------------------------------

				/* self type */
				using ___self = cs::http::request;


			public:

				// -- public lifecycle ----------------------------------------

				/* default constructor */
				request(void) noexcept = default;

				/* copy constructor */
				request(const ___self&) = default;

				/* move constructor */
				request(___self&&) noexcept = default;

				/* destructor */
				~request(void) noexcept = default;


				// -- public assignment operators -----------------------------

				/* copy assignment operator */
				auto operator=(const ___self&) -> ___self& = default;

				/* move assignment operator */
				auto operator=(___self&&) noexcept -> ___self& = default;


				// -- public modifiers ----------------------------------------

				/* clear */
				auto clear(void) noexcept -> void;


				// -- public accessors ----------------------------------------

				/* method */
				auto method(void) noexcept -> cs::http::method&;

				/* const method */
				auto method(void) const noexcept -> const cs::http::method&;

				/* headers */
				auto headers(void) noexcept -> cs::list<cs::http::header>&;

				/* const headers */
				auto headers(void) const noexcept -> const cs::list<cs::http::header>&;


				// -- public methods ------------------------------------------

				/* print */
				inline auto print(void) const noexcept -> void {

					std::cout << "\x1b[32m[request]\x1b[0m" << std::endl;
					std::cout << "\x1b[33m" << _method.name() << " [\x1b[0m" << _method.uri() << "\x1b[33m] [\x1b[0m" << _method.version() << "\x1b[33m]\x1b[0m" << std::endl;

					for (const auto& header : _headers) {
						std::cout << "\x1b[35m" << header.name() << " \x1b[36m[\x1b[0m" << header.field() << "\x1b[36m]\x1b[0m" << std::endl;
					}
				}


			private:

				// -- private members -----------------------------------------

				/* method */
				cs::http::method _method;

				/* headers */
				cs::list<cs::http::header> _headers;

		}; // class request

	} // namespace http

} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_HTTP_REQUEST_HPP___
