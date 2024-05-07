#ifndef ___CS_HTTP_HEADER_HPP___
#define ___CS_HTTP_HEADER_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/containers/vector.hpp"


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- H T T P  N A M E S P A C E ------------------------------------------

	namespace http {


		// -- H E A D E R -------------------------------------------------------

		class header final {


			private:

				// -- private types -------------------------------------------

				/* self type */
				using ___self = cs::http::header;


			public:


				// -- public lifecycle ----------------------------------------

				/* default constructor */
				header(void) noexcept = default;

				/* copy constructor */
				header(const ___self&) = default;

				/* move constructor */
				header(___self&&) noexcept = default;

				/* destructor */
				~header(void) noexcept = default;


				// -- public assignment operators -----------------------------

				/* copy assignment operator */
				auto operator=(const ___self&) -> ___self& = default;

				/* move assignment operator */
				auto operator=(___self&&) noexcept -> ___self& = default;


				// -- public accessors ----------------------------------------

				/* name */
				auto name(void) noexcept -> cs::vector<char>&;

				/* const name */
				auto name(void) const noexcept -> const cs::vector<char>&;

				/* field */
				auto field(void) noexcept -> cs::vector<char>&;

				/* const field */
				auto field(void) const noexcept -> const cs::vector<char>&;


			private:

				// -- private members -----------------------------------------

				/* name */
				cs::vector<char> _name;

				/* field */
				cs::vector<char> _field;

			// interresting headers for the server

			// headers
			//enum class ___header : cs::size_t {
			//	CACHE_CONTROL,
			//	CLEAR_SITE_DATA,
			//	CONNECTION,
			//	KEEP_ALIVE,
			//	ACCEPT,
			//	ACCEPT_ENCODING,
			//	ACCEPT_LANGUAGE,
			//	CONTENT_LENGTH,
			//	CONTENT_TYPE,
			//	CONTENT_ENCODING,
			//	CONTENT_LANGUAGE,
			//	REFRESH,
			//	HOST,
			//	USER_AGENT,
			//	ALLOW,
			//	SERVER,
			//};



		}; // class header

	} // namespace http

} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_HTTP_HEADER_HPP___
