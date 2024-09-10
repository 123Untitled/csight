#include "cs/network/dispatch.hpp"
#include "cs/diagnostics/exception.hpp"
#include "cs/type_traits/move.hpp"
#include "cs/hint.hpp"
#include <unistd.h>

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

/* disable */
auto cs::dispatch::disable(cs::io_event& ___obs) -> void {
	___self::_shared()._disable(___obs);
}

/* run */
auto cs::dispatch::run(void) -> void {
	___self::_shared()._run();
}

/* stop */
auto cs::dispatch::stop(void) -> void {
	___self::_shared()._running = false;
}



// -- private lifecycle -------------------------------------------------------

/* default constructor */
cs::dispatch::dispatch(void)
#if defined(___cs_os_macos)
: _handle{::kqueue()},
#elif defined(___cs_os_linux)
: _handle{::epoll_create1(0)},
#endif
  _events{}, _map{}, _running{true} {

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


// -- non-member functions ------------------------------------------------

static auto ev_read(const cs::ev_flag ___ev) noexcept -> bool {

	#if defined(___cs_os_macos)
	return ___ev & EVFILT_READ ? true : false;
	#elif defined(___cs_os_linux)
	return ___ev & EPOLLIN ? true : false;
	#endif
}

static auto ev_write(const cs::ev_flag ___ev) noexcept -> bool {

	#if defined(___cs_os_macos)
	return ___ev & EVFILT_WRITE ? true : false;
	#elif defined(___cs_os_linux)
	return ___ev & EPOLLOUT ? true : false;
	#endif
}

// -- private methods ---------------------------------------------------------


/* poll */
auto cs::dispatch::_poll(void) -> void {

	// wait for events
	const auto nev = _wait();

	if (nev == 0)
		return;

	::write(STDOUT_FILENO, "\n", 1U);

	// process events
	for (size_type i = 0U; i < nev; ++i) {

		// get event flag
		const cs::ev_flag ev = _event(_events[i]);
		// get observer
		cs::io_event& ___io = _data(_events[i]);

		// check event
		if (ev_read(ev))
			___io.read();

		else if (ev_write(ev))
			___io.send();

		else
			throw cs::runtime_error{"unknown event"};
	}

	// resize events
	if (nev == _events.size())
		_events.resize(_events.size() * 2U);
}

/* wait */
auto cs::dispatch::_wait(void) -> size_type {

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

	return static_cast<size_type>(ret);
}

/* add */
auto cs::dispatch::_add(cs::unique_ptr<cs::io_event>&& ___io) -> void {

	// check state
	if (not _running)
		return;

	auto it = _map.emplace(___io.get(), cs::move(___io));

	cs::io_event* user_data = it.first->second.get();
	const int descriptor = user_data->descriptor();


	// -- macos ---------------------------------------------------------------

	#if defined(___cs_os_macos)

	std::cout << "adding descriptor to dispatch: " << descriptor << std::endl;

	// create event (enable EOF notification)
	___event event {
		.ident  = static_cast<uintptr_t>(descriptor),
		.filter = EVFILT_READ,
		.flags  = EV_ADD | EV_ENABLE | EV_EOF,
		.fflags = 0U,
		.data   = 0,
		.udata  = user_data
	};

	// EV_CLEAR info: make

	// add event
	if (::kevent(_handle, &event, 1, nullptr, 0, nullptr) != 0)
		throw cs::runtime_error{"failed to add descriptor to kqueue"};


	// -- linux ---------------------------------------------------------------

	#elif defined(___cs_os_linux)

	// create event
	struct epoll_event event {
		.events = EPOLLIN, // default for all io objects
		.data = {
			.ptr = user_data }
	};

	// add event
	if (::epoll_ctl(_handle, EPOLL_CTL_ADD,
					descriptor,
					&event) == -1)
		throw cs::runtime_error{"failed to add descriptor to epoll"};

	#endif

	std::cout << "dispatch descriptor [" << it.first->second->descriptor() << "]\n";
}

/* mod */
auto cs::dispatch::_mod(const cs::ev_flag ___ev, cs::io_event& ___io) -> void {


	// -- macos ---------------------------------------------------------------

	#if defined(___cs_os_macos)

	// create event
	struct kevent event {
		.ident  = static_cast<uintptr_t>(___io.descriptor()),
		.filter = ___ev,
		.flags  = EV_ENABLE,
		.fflags = 0U,
		.data   = 0,
		.udata  = &___io
	};

	// modify event
	if (::kevent(_handle, &event, 1, nullptr, 0, nullptr) == -1)
		throw cs::runtime_error{"failed to modify descriptor in kqueue"};


	// -- linux ---------------------------------------------------------------

	#elif defined(___cs_os_linux)

	// create event
	struct epoll_event event {
		.events = ___ev,
		.data   = {
			.ptr = &___io }
	};

	// modify event
	if (::epoll_ctl(_handle, EPOLL_CTL_MOD, ___io.descriptor(), &event) == -1)
		throw cs::runtime_error{"failed to modify descriptor in epoll"};

	#endif

}


/* remove */
auto cs::dispatch::_remove(cs::io_event& ___io) -> void {

	// search for io event
	auto it = _map.find(&___io);

	if (it == _map.end())
		throw cs::runtime_error{"io event not found in dispatch map, cannot remove it!"};

	std::cout << "removing descriptor from dispatch: " << ___io.descriptor() << std::endl;


	// -- macos ---------------------------------------------------------------

	#if defined(___cs_os_macos)

	// create event
	struct kevent event {
		.ident  = static_cast<uintptr_t>(___io.descriptor()),
		.filter = EVFILT_READ,
		.flags  = EV_DELETE,
		.fflags = 0U,
		.data   = 0,
		.udata  = nullptr
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
		.data   = {
			.ptr = nullptr }
	};

	// remove event
	if (::epoll_ctl(_handle, EPOLL_CTL_DEL, ___io.descriptor(), &event) == -1)
		throw cs::runtime_error{"failed to remove descriptor from epoll"};

	#endif


	// -- common --------------------------------------------------------------

	// erase from map
	_map.erase(it);

}

/* disable */
auto cs::dispatch::_disable(cs::io_event& ___io) -> void {

	// search for io event
	auto it = _map.find(&___io);

	if (it == _map.end())
		throw cs::runtime_error{"io event not found in dispatch map, cannot disable it!"};

	std::cout << "disabling descriptor from dispatch: " << ___io.descriptor() << std::endl;


	// -- macos ---------------------------------------------------------------

	#if defined(___cs_os_macos)

	// create event
	struct kevent event {
		.ident  = static_cast<uintptr_t>(___io.descriptor()),
		.filter = EVFILT_READ,
		.flags  = EV_DISABLE,
		.fflags = 0U,
		.data   = 0,
		.udata  = nullptr
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
		.data   = {
			.ptr = nullptr }
	};

	// remove event
	if (::epoll_ctl(_handle, EPOLL_CTL_MOD, ___io.descriptor(), &event) == -1)
		throw cs::runtime_error{"failed to remove descriptor from epoll"};

	#endif

}

/* run */
auto cs::dispatch::_run(void) -> void {

	static size_type i = 0U;

	// wait
	while (_running) {

		cs::dispatch::_poll();

		if (++i == 8U) {
			::write(STDOUT_FILENO, "\r\x1b[2K", 5U);
			i = 0U;
		}

		::write(STDOUT_FILENO, ".", 1);
	}

}


