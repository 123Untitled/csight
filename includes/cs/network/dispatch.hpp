#ifndef ___CS_NETWORK_DISPATCH_HPP___
#define ___CS_NETWORK_DISPATCH_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/os.hpp"
#include "cs/system/descriptor.hpp"
#include "cs/containers/vector.hpp"
#include "cs/io_event.hpp"
#include "cs/network/event.hpp"
#include "cs/memory/unique_ptr.hpp"

#include <unordered_map>


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- D I S P A T C H -----------------------------------------------------

	class dispatch {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = cs::dispatch;

			/* event type */
			#if defined(___cs_os_macos)
			using ___event = struct ::kevent;

			/* event type */
			#elif defined(___cs_os_linux)
			using ___event = struct ::epoll_event;
			#endif


			// -- private constants -------------------------------------------

			/* timeout (ms) */
			enum : long {
				DISPATCH_TIMEOUT = 128U
			};


		private:

			// -- private members ---------------------------------------------

			/* handle */
			cs::descriptor _handle;

			/* events */
			cs::vector<___event> _events;

			/* map */
			std::unordered_map<const cs::io_event*,
				cs::unique_ptr<cs::io_event>> _map;

			/* running */
			bool _running;



		public:

			// -- public types ------------------------------------------------

			/* size type */
			using size_type = cs::vector<___event>::size_type;


			// -- public methods ----------------------------------------------

			/* poll */
			static auto poll(void) -> void;

			/* add */
			template <typename... ___ios>
			static auto add(___ios&&... ___args) -> void {

				auto& ___sh = ___self::_shared();
				(___sh._add(cs::forward<___ios>(___args)), ...);
			}

			/* mod */
			template <cs::ev_flag ___ev, typename... ___ios>
			static auto mod(___ios&... ___args) -> void {

				auto& ___sh = ___self::_shared();
				(___sh._mod(___ev, ___args), ...);
			}

			/* remove */
			static auto remove(cs::io_event&) -> void;

			/* run */
			static auto run(void) -> void;

			/* stop */
			static auto stop(void) -> void;


		private:

			// -- private lifecycle -------------------------------------------

			/* default constructor */
			dispatch(void);

			/* deleted copy constructor */
			dispatch(const ___self&) = delete;

			/* deleted move constructor */
			dispatch(___self&&) = delete;

			/* destructor */
			~dispatch(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const ___self&) -> ___self& = delete;

			/* deleted move assignment operator */
			auto operator=(___self&&) -> ___self& = delete;


			// -- private static methods --------------------------------------

			/* shared */
			static auto _shared(void) -> ___self&;

			/* data */
			static auto _data(___event&) noexcept -> cs::io_event&;

			/* event */
			static auto _event(___event&) noexcept -> cs::ev_flag;


			// -- private methods ---------------------------------------------

			/* poll */
			auto _poll(void) -> void;

			/* wait */
			auto _wait(void) -> size_type;

			/* add */
			auto _add(cs::unique_ptr<cs::io_event>&&) -> void;

			/* mod */
			auto _mod(const cs::ev_flag, cs::io_event&) -> void;

			/* remove */
			auto _remove(cs::io_event&) -> void;

			/* run */
			auto _run(void) -> void;

	}; // class dispatch




} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_NETWORK_DISPATCH_HPP___





			///* is event */
			//#if defined(___cs_os_macos)
			//
			//enum : cs::ev_flag {
			//	EV_MASK = ~EVFILT_READ
			//			| ~EVFILT_WRITE
			//			| ~EVFILT_AIO
			//			| ~EVFILT_VNODE
			//			| ~EVFILT_PROC
			//			| ~EVFILT_SIGNAL
			//			| ~EVFILT_TIMER
			//			| ~EVFILT_MACHPORT
			//			| ~EVFILT_FS
			//			| ~EVFILT_USER
			//			| ~EVFILT_VM
			//			| ~EVFILT_EXCEPT
			//			| ~EVFILT_SYSCOUNT
			//};
			//
			//
			//template <cs::ev_flag ___ev>
			//static constexpr bool is_event = (~___ev & EV_MASK) == 0;
			//
			//#elif defined(___cs_os_linux)
			//
			//
			///* is epoll event */
			//template <cs::ev_flag ___ev>
			//static constexpr bool is_event = (___ev & ~(EPOLLIN
			//										  | EPOLLOUT
			//										  | EPOLLRDHUP
			//										  | EPOLLPRI
			//										  | EPOLLERR
			//										  | EPOLLHUP
			//										  | EPOLLET
			//										  | EPOLLONESHOT)) == 0;
			//
			//#endif

