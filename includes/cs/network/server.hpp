#ifndef ___CS_NETWORK_SERVER_HPP___
#define ___CS_NETWORK_SERVER_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/io_event.hpp"
#include "cs/network/client.hpp"
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

			/* running */
			bool _running;

			/* port */
			::in_port_t _port;

			/* socket */
			cs::socket _socket;

			/* client */
			cs::client _client;


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
			~server(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const ___self&) -> ___self& = delete;

			/* move assignment operator */
			auto operator=(___self&&) noexcept -> ___self& = default;


			// -- public overriden methods ------------------------------------

			/* dispatch */
			auto dispatch(const cs::ev_flag) -> void override;

			/* descriptor */
			auto descriptor(void) const noexcept -> int override;


			// -- public methods ----------------------------------------------

			/* run */
			auto run(void) -> void;

			/* stop */
			auto stop(void) -> void;

	}; // class server

} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_NETWORK_SERVER_HPP___
