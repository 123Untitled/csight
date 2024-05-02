#ifndef ___IO_EVENT_HPP___
#define ___IO_EVENT_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/containers/vector.hpp"


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- I O  E V E N T ------------------------------------------------------

	class io_event {


		// -- friends ---------------------------------------------------------

		/* server as friend */
		friend class server;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = cs::io_event;


			// -- private methods ---------------------------------------------

			/* remove */
			static auto remover(void) noexcept -> cs::vector<___self*>& {
				static cs::vector<___self*> ___list{};
				return ___list;
			}


		protected:

			// -- protected methods -------------------------------------------

			/* register for removal */
			static auto register_for_removal(___self& ___obs) -> void {

				auto& ___list = ___self::remover();

				___list.push_back(&___obs);
			}

		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			io_event(void) noexcept = default;

			/* copy constructor */
			io_event(const ___self&) noexcept = default;

			/* move constructor */
			io_event(___self&&) noexcept = default;

			/* destructor */
			virtual ~io_event(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const ___self&) noexcept -> ___self& = default;

			/* move assignment operator */
			auto operator=(___self&&) noexcept -> ___self& = default;


			// -- public interface --------------------------------------------

			/* dispatch */
			virtual auto dispatch(const int) -> void = 0;

			/* descriptor */
			virtual auto descriptor(void) const noexcept -> int = 0;

	}; // class io_event

} // namespace cs

#endif // ___cs_requirements

#endif // ___IO_EVENT_HPP___
