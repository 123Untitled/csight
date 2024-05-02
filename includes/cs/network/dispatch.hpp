#ifndef ___CS_DISPATCH_HPP___
#define ___CS_DISPATCH_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/os.hpp"
#include "cs/diagnostics/exception.hpp"
#include "cs/system/descriptor.hpp"
#include "cs/containers/vector.hpp"

#if defined(___cs_os_macos)
#	include <sys/event.h>
#	include <sys/time.h>
#elif defined(___cs_os_linux)
#	include <sys/epoll.h>
#endif

#include <iostream>


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- non-member constants ------------------------------------------------

	#if defined(___cs_os_macos)
	/* dispatch event */
	enum dispatch_event : int {
		EV_READ  = EVFILT_READ,
		EV_WRITE = EVFILT_WRITE,
		EV_ERROR = EVFILT_ERROR
	};

	#elif defined(___cs_os_linux)
	/* dispatch event */
	enum dispatch_event : ::uint32_t {
		EV_READ  = EPOLLIN,
		EV_WRITE = EPOLLOUT,
		EV_ERROR = EPOLLERR
	};
	#endif


	// -- D I S P A T C H -----------------------------------------------------

	template <typename ___type>
	class dispatch {

		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = cs::dispatch<___type>;

			/* event type */
			#if defined(___cs_os_macos)
			using ___event = struct ::kevent;

			/* event flags */
			using ___ev_flag = int;


			/* event type */
			#elif defined(___cs_os_linux)
			using ___event = struct ::epoll_event;

			/* event flags */
			using ___ev_flag = ::uint32_t;

			#endif

			/* size type */
			using ___size = cs::vector<___event>::size_type;


			// -- private constants -------------------------------------------

			/* timeout (ms) */
			enum : long {
				DISPATCH_TIMEOUT = 200U
			};


			/* is event */
			#if defined(___cs_os_macos)

			enum : int {
				EV_MASK = ~EVFILT_READ
						| ~EVFILT_WRITE
						| ~EVFILT_AIO
						| ~EVFILT_VNODE
						| ~EVFILT_PROC
						| ~EVFILT_SIGNAL
						| ~EVFILT_TIMER
						| ~EVFILT_MACHPORT
						| ~EVFILT_FS
						| ~EVFILT_USER
						| ~EVFILT_VM
						| ~EVFILT_EXCEPT
						| ~EVFILT_SYSCOUNT
			};

			// -1  1111111111111111
			// -2  1111111111111110
			// -3  1111111111111101
			// -4  1111111111111100
			// -5  1111111111111011
			// -6  1111111111111010
			// -7  1111111111111001
			// -8  1111111111111000
			// -9  1111111111110111
			// -10 1111111111110110
			// -12 1111111111110100
			// -15 1111111111110001
			// -18 1111111111101110

			// inverted
			// 0   0000000000000000
			// 1   0000000000000001
			// 2   0000000000000010
			// 3   0000000000000011
			// 4   0000000000000100
			// 5   0000000000000101
			// 6   0000000000000110
			// 7   0000000000000111
			// 8   0000000000001000
			// 9   0000000000001001
			// 10  0000000000001010
			// 12  0000000000001100
			// 15  0000000000001111
			// 18  0000000000010010


			template <___ev_flag ___ev>
			static constexpr bool is_event = (~___ev & EV_MASK) == 0;

			#elif defined(___cs_os_linux)


			/* is epoll event */
			template <___ev_flag ___ev>
			static constexpr bool is_event = (___ev & ~(EPOLLIN
													  | EPOLLOUT
													  | EPOLLRDHUP
													  | EPOLLPRI
													  | EPOLLERR
													  | EPOLLHUP
													  | EPOLLET
													  | EPOLLONESHOT)) == 0;

			#endif


		private:

			// -- public types ------------------------------------------------

			/* observer type */
			using observer = ___type;


			// -- private members ---------------------------------------------

			/* handle */
			cs::descriptor _handle;

			/* events */
			cs::vector<___event> _events;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			dispatch(void)
			#if defined(___cs_os_macos)
			: _handle{::kqueue()},
			#elif defined(___cs_os_linux)
			: _handle{::epoll_create1(0)},
			#endif
			_events{}
				{

				// check for error
				if (not _handle)
					throw cs::runtime_error{"failed to create kqueue"};

				// allocate events
				_events.resize(64);
			}


			/* deleted copy constructor */
			dispatch(const ___self&) = delete;

			/* move constructor */
			dispatch(___self&&) noexcept = default;

			/* destructor */
			~dispatch(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const ___self&) -> ___self& = delete;

			/* move assignment operator */
			auto operator=(___self&&) noexcept -> ___self& = default;


			// -- public methods ----------------------------------------------

			/* wait */
			#if defined(___cs_os_macos)
			auto wait(void) -> void {

				// compute timeout
				constexpr struct ::timespec timeout {
					.tv_sec  =  DISPATCH_TIMEOUT / 1'000U,
					.tv_nsec = (DISPATCH_TIMEOUT % 1'000U) * 1'000'000U
				};

				// wait for events
				const auto ret = ::kevent(_handle, nullptr, 0, _events.data(), static_cast<int>(_events.size()), &timeout);

				// check for error
				if (ret == -1)
					throw cs::runtime_error{"failed to wait for events"};

				// process events
				for (___size i = 0U; i < static_cast<___size>(ret); ++i) {

					// get event
					const auto& ___ev = _events[i];

					// send event type
					reinterpret_cast<observer*>(___ev.udata)->dispatch(___ev.filter);
				}

				// resize events
				if (static_cast<___size>(ret) == _events.size())
					_events.resize(_events.size() * 2U);

			}

			#elif defined(___cs_os_linux)
			auto wait(void) -> void {

				std::cout << "linux dispatch is not implemented" << std::endl;
			}
			#endif


			// -- public modifiers --------------------------------------------

			/* add */
			template <___ev_flag ___evs>
			static auto add(const ___self& ___disp, observer& ___obs) -> void {

				// check for valid event
				static_assert(___self::is_event<___evs>, "dispatch: event not supported");

				#if defined(___cs_os_macos)

				// create event
				struct kevent event {
					.ident  = static_cast<uintptr_t>(___obs.descriptor()),
					.filter = ___evs,
					.flags  = EV_ADD,
					.fflags = 0U,
					.data   = 0,
					.udata  = &___obs
				};

				// EV_CLEAR info: make

				// add event
				if (::kevent(___disp._handle, &event, 1, nullptr, 0, nullptr) != 0)
					throw cs::runtime_error{"failed to add descriptor to kqueue"};

				#elif defined(___cs_os_linux)

				// create event
				struct epoll_event event {
					.events = ___evs,
					.data   = {reinterpret_cast<void*>(___obs.descriptor())}
				};

				// add event
				if (::epoll_ctl(___disp._handle, EPOLL_CTL_ADD, ___obs.descriptor(), &event) == -1)
					throw cs::runtime_error{"failed to add descriptor to epoll"};

				#endif
			}


			/* remove */
			static auto remove(const ___self& ___disp, observer& ___obs) -> void {

				#if defined(___cs_os_macos)

				// create event
				struct kevent event {
					.ident  = static_cast<uintptr_t>(___obs.descriptor()),
					.filter = EVFILT_READ,
					.flags  = EV_DELETE,
					.fflags = 0U,
					.data   = 0,
					.udata  = &___obs
				};

				std::cout << "removing descriptor from kqueue: " << ___obs.descriptor() << std::endl;

				// remove event
				if (::kevent(___disp._handle, &event, 1, nullptr, 0, nullptr) == -1) {
					perror("kevent");
					throw cs::runtime_error{"failed to remove descriptor from kqueue"};
				}

				#elif defined(___cs_os_linux)

				// create event
				struct epoll_event event {
					.events = 0,
					.data   = {reinterpret_cast<void*>(___obs.descriptor())}
				};

				// remove event
				if (::epoll_ctl(___disp._handle, EPOLL_CTL_DEL, ___obs.descriptor(), &event) == -1)
					throw cs::runtime_error{"failed to remove descriptor from epoll"};

				#endif
			}

	}; // class dispatch



	// -- non-member functions ------------------------------------------------

	inline auto is_read_event(const int ___evnts) noexcept -> bool {

		#if defined(___cs_os_macos)
		return ___evnts & EVFILT_READ ? true : false;
		#elif defined(___cs_os_linux)
		return true;
		//return ___evnts & EPOLLIN ? true : false;
		#endif
	}

	inline auto is_write_event(const int ___evnts) noexcept -> bool {

		#if defined(___cs_os_macos)
		return ___evnts & EVFILT_WRITE ? true : false;
		#elif defined(___cs_os_linux)
		return true;
		//return ___evnts & EPOLLOUT ? true : false;
		#endif
	}

} // namespace cs

#endif // ___cs_requirements

#endif // ___CS_DISPATCH_HPP___
