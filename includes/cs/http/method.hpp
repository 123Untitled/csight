#ifndef ___CS_HTTP_METHOD_HPP___
#define ___CS_HTTP_METHOD_HPP___


#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/types.hpp"
#include "cs/containers/vector.hpp"


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- H T T P  N A M E S P A C E ------------------------------------------

	namespace http {


		// -- M E T H O D -----------------------------------------------------

		class method final {


			private:

				// -- private types -------------------------------------------

				/* self type */
				using ___self = cs::http::method;


				// -- private members -----------------------------------------

				/* name */
				cs::vector<char> _name;

				/* uri */
				cs::vector<char> _uri;

				/* version */
				cs::vector<char> _version;


			public:

				// -- public constants --------------------------------

				/* method type */
				enum : unsigned {

					GET,
					HEAD,
					TRACE,
					DELETE,
					OPTIONS,
					PUT,
					POST,
					PATCH,
					CONNECT,

					MAX_METHOD,
					UNDEF
				};


				// -- public lifecycle ----------------------------------------

				/* default constructor */
				method(void) noexcept = default;

				/* copy constructor */
				method(const ___self&) noexcept = default;

				/* move constructor */
				method(___self&&) noexcept = default;

				/* destructor */
				~method(void) noexcept = default;


				// -- public assignment operators -----------------------------

				/* copy assignment operator */
				auto operator=(const ___self&) noexcept -> ___self& = default;

				/* move assignment operator */
				auto operator=(___self&&) noexcept -> ___self& = default;


				// -- public modifiers ----------------------------------------

				/* clear */
				auto clear(void) noexcept -> void;


				// -- public accessors ----------------------------------------

				/* name */
				auto name(void) noexcept -> cs::vector<char>&;

				/* const name */
				auto name(void) const noexcept -> const cs::vector<char>&;

				/* uri */
				auto uri(void) noexcept -> cs::vector<char>&;

				/* const uri */
				auto uri(void) const noexcept -> const cs::vector<char>&;

				/* version */
				auto version(void) noexcept -> cs::vector<char>&;

				/* const version */
				auto version(void) const noexcept -> const cs::vector<char>&;

				/* type */
				auto type(void) const noexcept -> unsigned;

		}; // class method

	} // namespace http

} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_HTTP_METHOD_HPP___
