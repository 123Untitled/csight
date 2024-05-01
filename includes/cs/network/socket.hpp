#ifndef CS_SOCKET_HEADER
#define CS_SOCKET_HEADER


#include "cs/config.hpp"

#if ___cs_requirements

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>



// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- S O C K E T ---------------------------------------------------------

	class socket final {


		// -- friends ---------------------------------------------------------

		/* socket as friend */
		template <int, int, int>
		friend auto make_socket(void) -> cs::socket;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = cs::socket;


			// -- private constants -------------------------------------------

			/* invalid socket */
			enum : int {
				INVALID_SOCKET = -1
			};


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			socket(void) noexcept
			: _socket{INVALID_SOCKET} {
			}

			/* deleted copy constructor */
			socket(const ___self&) = delete;

			/* move constructor */
			socket(___self&& ___ot) noexcept
			: _socket{___ot._socket} {
				___ot._socket = INVALID_SOCKET;
			}

			/* destructor */
			~socket(void) noexcept {
				_close();
			}


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const ___self&) -> ___self& = delete;

			/* move assignment operator */
			auto operator=(___self&& ___ot) noexcept -> ___self& {

				if (this == &___ot)
					return *this;

				_close();

				_socket = ___ot._socket;
				___ot._socket = INVALID_SOCKET;

				return *this;
			}


			// -- public conversion operators ---------------------------------

			/* int conversion operator */
			operator int(void) const noexcept {
				return _socket;
			}


		private:

			// -- private lifecycle -------------------------------------------

			/* descriptor constructor */
			explicit socket(const int ___socket) noexcept
			: _socket{___socket} {
			}


			// -- private methods ---------------------------------------------

			/* close */
			auto _close(void) noexcept -> void {
				if (_socket == INVALID_SOCKET)
					return;
				::close(_socket);
			}

			// -- private members ---------------------------------------------

			/* socket */
			int _socket;

	}; // class socket


	// -- non-member functions ------------------------------------------------

	/* socket */
	template <int ___domain, int ___type = SOCK_STREAM, int ___protocol = 0>
	auto make_socket(void) -> cs::socket {
		const int ___sock = ::socket(___domain, ___type, ___protocol);
		if (___sock == cs::socket::INVALID_SOCKET)
			throw;
		return cs::socket{___sock};
	}


} // namespace cs

#endif // ___cs_requirements

#endif // CS_SOCKET_HEADER
