#ifndef ___CS_NETWORK_POLL_HPP___
#define ___CS_NETWORK_POLL_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/containers/vector.hpp"
#include "cs/io_event.hpp"

#include <sys/poll.h>

#include <unistd.h>

// -- C S  N A M E S P A C E --------------------------------------------------

namespace test {


	/* event flag type */
	using ev_flag = short int;

	/* poll event */
	enum poll_ev : cs::ev_flag {

		// explicit events
		EV_READ          = POLLIN,  // there is data to read
		EV_PRIORITY_READ = POLLPRI, // there is urgent data to read
		EV_WRITE         = POLLOUT, // writing now will not block

		// implicit events (always polled for, see 'revents')
		EV_ERROR         = POLLERR, // error condition
		EV_HANGUP        = POLLHUP, // connection closed
		EV_INVALID       = POLLNVAL // invalid polling request

	}; // enum dispatch_event


	// -- P O L L -------------------------------------------------------------

	class poll {

		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = test::poll;

			/* poll struct */
			using ___pollfd = struct ::pollfd;

			/* size type */
			using ___size = typename cs::vector<___pollfd>::size_type;


			/*

			 * struct pollfd

			 * int       fd;        // file descriptor to poll
			 * short int events;    // types of events poller cares about
			 * short int revents;   // types of events that actually occurred

			 */

			// -- private members ---------------------------------------------

			/* poll fds */
			cs::vector<___pollfd> _fds;


			// -- private constants -------------------------------------------

			/* timeout (ms) */
			enum : int {
				POLL_TIMEOUT = 200
			};


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			poll(void) noexcept = default;

			/* copy constructor */
			poll(const ___self &) = default;

			/* move constructor */
			poll(___self&&) noexcept = default;

			/* destructor */
			~poll(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const ___self &) -> ___self& = default;

			/* move assignment operator */
			auto operator=(___self&&) noexcept -> ___self& = default;


			// -- public modifiers --------------------------------------------

			/* add */
			template <cs::ev_flag ___ev>
			auto add(cs::io_event&& ___vl) -> void {

				// emplace poll fd
				_fds.emplace_back(___pollfd{___vl.descriptor(), ___ev, 0});
			}

			/* wait */
			auto wait(void) -> void {

				// wait events
				const int ret = ::poll(_fds.data(), _fds.size(), POLL_TIMEOUT);

				// check for error
				if (ret < 0)
					throw cs::runtime_error{"poll failed"};

				// timeout
				if (ret == 0) {
					::write(STDOUT_FILENO, ".", 1);
					return;
				}

				for (___size i = 0U; i < _fds.size(); ++i) {

					// get poll fd
					const auto& pfd = _fds[i];

					// check for events
					if (pfd.revents & POLLIN) {
						::write(STDOUT_FILENO, "r", 1); }

					if (pfd.revents & POLLOUT) {
						::write(STDOUT_FILENO, "w", 1); }

					if (pfd.revents & POLLERR) {
						::write(STDOUT_FILENO, "e", 1); }

					if (pfd.revents & POLLHUP) {
						::write(STDOUT_FILENO, "h", 1); }

					if (pfd.revents & POLLNVAL) {
						::write(STDOUT_FILENO, "n", 1); }

					// clear revents
					_fds[i].revents = 0;
				}

			}

		// On success, poll() returns a nonnegative value which is the number of elements
		// in the pollfds whose revents fields have been set to a nonzero value
		// (indicating an event or an error).
		// A return value of zero indicates that the system call timed out
		// before any file descriptors became read.


		private:

			// -- private modifiers -------------------------------------------

			/* remove */
			auto _remove(const int ___fd) -> void {

				// loop over fds
				for (___size i = 0U; i < _fds.size(); ++i) {

					if (_fds[i].fd == ___fd) {
						// erase
						break; }
				}
			}


		private:


	}; // class poll

} // namespace cs

#endif // ___cs_requirements

#endif // ___CS_NETWORK_POLL_HPP___
