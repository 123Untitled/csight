#ifndef ___CS_CLIENT_HPP___
#define ___CS_CLIENT_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/network/socket.hpp"
#include "cs/io_event.hpp"
#include "cs/http/parser.hpp"
#include "cs/http/request.hpp"
#include "cs/string.hpp"


#include <string>

// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- C L I E N T ---------------------------------------------------------

	class client final : public cs::io_event {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = cs::client;

			/* size type */
			using ___size = cs::size_t;


			// -- private constants -------------------------------------------

			/* buffer size */
			enum : ___size {
				BUFFER_SIZE = 1024
			};


			// -- private members ---------------------------------------------

			/* socket */
			cs::socket _socket;

			/* parser */
			cs::http::parser _parser;

			/* request */
			cs::http::request _request;


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
			virtual ~client(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const ___self&) -> ___self& = delete;

			/* move assignment operator */
			auto operator=(___self&&) noexcept -> ___self& = default;


			// -- public overriden methods ------------------------------------

			/* read */
			auto read(void) -> void override;

			/* send */
			auto send(void) -> void override;

			/* descriptor */
			auto descriptor(void) const noexcept -> int override;


		private:

			// -- private methods ---------------------------------------------

			/* handle request */
			auto _handle_request(void) -> void;


	}; // class client

} // namespace cs

#endif // ___cs_requirements

#endif // ___CS_CLIENT_HPP___
