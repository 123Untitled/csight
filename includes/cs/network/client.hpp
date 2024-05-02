#ifndef ___CS_CLIENT_HPP___
#define ___CS_CLIENT_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/network/socket.hpp"
#include "cs/io_event.hpp"


#include <string>

// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- C L I E N T ---------------------------------------------------------

	class client final : public cs::io_event {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = cs::client;


			// -- private members ---------------------------------------------

			/* socket */
			cs::socket _socket;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			client(void) noexcept = default;

			/* socket constructor */
			client(cs::socket&&) noexcept;

			/* deleted copy constructor */
			client(const ___self&) = delete;

			/* move constructor */
			client(___self&&) noexcept = default;

			/* destructor */
			~client(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const ___self&) -> ___self& = delete;

			/* move assignment operator */
			auto operator=(___self&&) noexcept -> ___self& = default;


			// -- public modifiers --------------------------------------------

			/* socket */
			auto socket(cs::socket&&) noexcept -> void;


			// -- public overriden methods ------------------------------------

			/* dispatch */
			auto dispatch(const int) -> void override;

			/* descriptor */
			auto descriptor(void) const noexcept -> int override;



		private:

			// -- private methods ---------------------------------------------

			/* serve file */
			auto serve_file(const std::string&,
					        const std::string&) -> void;

			auto serve_index(void) -> void;

	}; // class client

} // namespace cs

#endif // ___cs_requirements

#endif // ___CS_CLIENT_HPP___
