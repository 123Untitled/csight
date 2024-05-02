#ifndef ___CS_NETWORK_SELECT_HPP___
#define ___CS_NETWORK_SELECT_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/types.hpp"

#include <sys/select.h>


// -- C S  N A M E S P A C E --------------------------------------------------

namespace test {


	// -- S E L E C T ---------------------------------------------------------

	class select final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = test::select;

			/* fd set */
			using ___fd_set = fd_set;


			// -- private constants -------------------------------------------

			/* ms timeout */
			enum : cs::size_t {
				SELECT_TIMEOUT = 200U
			};

			/* timeout */
			constexpr static struct ::timeval _timeout {
				.tv_sec  =  SELECT_TIMEOUT / 1000,
				.tv_usec = (SELECT_TIMEOUT % 1000) * 1000
			};


			// -- private members ---------------------------------------------

			/* fd set */
			___fd_set _fds;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			select(void) noexcept {
				FD_ZERO(&_fds);
			}

			/* copy constructor */
			select(const ___self&) noexcept = default;

			/* move constructor */
			select(___self&&) noexcept = default;

			/* destructor */
			~select(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const ___self&) noexcept -> ___self& = default;

			/* move assignment operator */
			auto operator=(___self&&) noexcept -> ___self& = default;


			// -- public modifiers --------------------------------------------

			/* add */
			auto add(int fd) noexcept -> void {
				FD_SET(fd, &_fds);
			}

			/* remove */
			auto remove(int fd) noexcept -> void {
				FD_CLR(fd, &_fds);
			}

			/* clear */
			auto clear(void) noexcept -> void {
				FD_ZERO(&_fds);
			}

			// FD_ISSET(fd, &_fds);

			// -- public methods ----------------------------------------------

			/* wait */
			auto wait(void) -> void {

				struct ::timeval timeout = _timeout;

				// wait for events
				const auto nev = ::select(FD_SETSIZE, &_fds, nullptr, nullptr, &timeout);


			}

	}; // class select

} // namespace test

#endif // ___cs_requirements
#endif // ___CS_NETWORK_SELECT_HPP___
