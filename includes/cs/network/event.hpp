#ifndef ___CS_NETWORK_EVENT_HPP___
#define ___CS_NETWORK_EVENT_HPP___


#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/os.hpp"

#if defined(___cs_os_macos)
#	include <sys/event.h>
#	include <sys/time.h>
#elif defined(___cs_os_linux)
#	include <sys/epoll.h>
#endif


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- macos ---------------------------------------------------------------

	#if defined(___cs_os_macos)

	/* event flag type */
	using ev_flag = ::int16_t; // kqueue event filter

	/* dispatch event */
	enum dispatch_ev : cs::ev_flag {
		EV_READ  = EVFILT_READ,
		EV_WRITE = EVFILT_WRITE,
	};


	// -- linux ---------------------------------------------------------------

	#elif defined(___cs_os_linux)

	/* event flag type */
	using ev_flag = ::uint32_t; // epoll event mask

	/* dispatch event */
	enum dispatch_ev : cs::ev_flag {
		EV_READ  = EPOLLIN,
		EV_WRITE = EPOLLOUT,
	};
	#endif

} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_NETWORK_EVENT_HPP___
