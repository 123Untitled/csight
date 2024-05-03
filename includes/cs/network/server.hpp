#ifndef ___CS_NETWORK_SERVER_HPP___
#define ___CS_NETWORK_SERVER_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/io_event.hpp"
#include "cs/network/socket.hpp"


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- S E R V E R ---------------------------------------------------------

	class server final : public cs::io_event {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = cs::server;


			// -- private members ---------------------------------------------

			/* socket */
			cs::socket _socket;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			server(void);

			/* port constructor */
			server(const ::in_port_t);

			/* deleted copy constructor */
			server(const ___self&) = delete;

			/* move constructor */
			server(___self&&) noexcept = default;

			/* destructor */
			virtual ~server(void) noexcept = default;


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

	}; // class server

} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_NETWORK_SERVER_HPP___
