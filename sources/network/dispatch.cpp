#include "cs/network/dispatch.hpp"
#include "cs/diagnostics/exception.hpp"

#include <iostream>


// -- public methods ----------------------------------------------------------

/* poll */
auto cs::dispatch::poll(void) -> void {
	___self::_shared()._poll();
}

/* remove */
auto cs::dispatch::remove(cs::io_event& ___obs) -> void {
	___self::_shared()._remove(___obs);
}



// -- private lifecycle -------------------------------------------------------

/* default constructor */
cs::dispatch::dispatch(void)
#if defined(___cs_os_macos)
: _handle{::kqueue()},
#elif defined(___cs_os_linux)
: _handle{::epoll_create1(0)},
#endif
  _events{} {

	// check for error
	if (not _handle)
		throw cs::runtime_error{"failed to create kqueue"};

	// allocate events
	_events.resize(64);
}


// -- private static methods --------------------------------------------------

/* shared */
auto cs::dispatch::_shared(void) -> ___self& {
	static ___self ___sh;
	return ___sh;
}

/* data */
auto cs::dispatch::_data(___event& ___ev) noexcept -> cs::io_event& {

	#if defined(___cs_os_macos)
	return *reinterpret_cast<cs::io_event*>(___ev.udata);

	#elif defined(___cs_os_linux)
	return *reinterpret_cast<cs::io_event*>(___ev.data.ptr);

	#endif
}

/* event */
auto cs::dispatch::_event(___event& ___ev) noexcept -> cs::ev_flag {

	#if defined(___cs_os_macos)
	return ___ev.filter;

	#elif defined(___cs_os_linux)
	return ___ev.events;

	#endif
}


// -- private methods ---------------------------------------------------------


/* poll */
auto cs::dispatch::_poll(void) -> void {

	// wait for events
	const auto nev = _wait();

	// process events
	for (___size i = 0U; i < nev; ++i) {
		// get event flag
		const cs::ev_flag ev = _event(_events[i]);
		// get observer
		cs::io_event& ob = _data(_events[i]);
		// dispatch event
		ob.dispatch(ev);
	}

	// resize events
	if (nev == _events.size())
		_events.resize(_events.size() * 2U);
}

/* wait */
auto cs::dispatch::_wait(void) -> ___size {

	// -- macos ---------------------------------------------------------------

	#if defined(___cs_os_macos)
	// compute timeout
	constexpr struct ::timespec timeout {
		.tv_sec  =  DISPATCH_TIMEOUT / 1'000U,
		.tv_nsec = (DISPATCH_TIMEOUT % 1'000U) * 1'000'000U
	};
	// wait for events
	const auto ret = ::kevent(_handle, nullptr, 0, _events.data(),
													static_cast<int>(_events.size()),
													&timeout);


	// -- linux ---------------------------------------------------------------

	#elif defined(___cs_os_linux)
	// wait for events
	const auto ret = ::epoll_wait(_handle, _events.data(),
											static_cast<int>(_events.size()),
											DISPATCH_TIMEOUT);
	// see epoll_pwait
	#endif


	// -- common --------------------------------------------------------------

	// check for error
	if (ret < 0)
		throw cs::runtime_error{"failed to wait for events"};

	return static_cast<___size>(ret);
}

/* add */
auto cs::dispatch::_add(const cs::ev_flag ___evs, cs::io_event& ___obs) -> void {

	// -- macos ---------------------------------------------------------------

	#if defined(___cs_os_macos)

	// create event
	___event event {
		.ident  = static_cast<uintptr_t>(___obs.descriptor()),
		.filter = ___evs,
		.flags  = EV_ADD,
		.fflags = 0U,
		.data   = 0,
		.udata  = &___obs
	};

	// EV_CLEAR info: make

	// add event
	if (::kevent(_handle, &event, 1, nullptr, 0, nullptr) != 0)
		throw cs::runtime_error{"failed to add descriptor to kqueue"};


	// -- linux ---------------------------------------------------------------

	#elif defined(___cs_os_linux)

	// create event
	struct epoll_event event {
		.events = ___evs,
		.data = {
			.ptr = &___obs }
	};

	// add event
	if (::epoll_ctl(_handle, EPOLL_CTL_ADD, ___obs.descriptor(), &event) == -1)
		throw cs::runtime_error{"failed to add descriptor to epoll"};

	#endif


}

/* remove */
auto cs::dispatch::_remove(cs::io_event& ___obs) -> void {

	std::cout << "removing descriptor from dispatch: " << ___obs.descriptor() << std::endl;

	// -- macos ---------------------------------------------------------------

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


	// remove event
	if (::kevent(_handle, &event, 1, nullptr, 0, nullptr) == -1) {
		perror("kevent");
		throw cs::runtime_error{"failed to remove descriptor from kqueue"};
	}


	// -- linux ---------------------------------------------------------------

	#elif defined(___cs_os_linux)

	// create event
	struct epoll_event event {
		.events = 0,
		.data   = {reinterpret_cast<void*>(___obs.descriptor())}
	};

	// remove event
	if (::epoll_ctl(._handle, EPOLL_CTL_DEL, ___obs.descriptor(), &event) == -1)
		throw cs::runtime_error{"failed to remove descriptor from epoll"};

	#endif
}
