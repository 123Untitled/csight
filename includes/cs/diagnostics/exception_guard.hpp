#ifndef ___CS_EXCEPTION_GUARD_HPP___
#define ___CS_EXCEPTION_GUARD_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/type_traits/move.hpp"


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- E X C E P T I O N  G U A R D ----------------------------------------

	template <typename ___rollback>
	class exception_guard final {


		private:

			// -- private types -------------------------------------------

			/* self type */
			using ___self = cs::exception_guard<___rollback>;


		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			exception_guard(void) = delete;

			/* explicit constructor */
			constexpr explicit exception_guard(___rollback ___rb)
			: _rollback{cs::move(___rb)}, _completed{false} {
			}

			/* deleted copy constructor */
			exception_guard(const ___self&) = delete;

			/* move constructor */
			constexpr exception_guard(___self&& ___ot) noexcept /* nothrow move */
			: _rollback{cs::move(___ot._rollback)}, _completed{___ot._completed} {
				___ot._completed = true;
			}

			/* destructor */
			~exception_guard(void) {
				if (_completed == true)
					return;
				_rollback();
			}


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const ___self&) -> ___self& = delete;

			/* deleted move assignment operator */
			auto operator=(___self&&) -> ___self& = delete;


			// -- public modifiers --------------------------------------------

			/* complete */
			constexpr auto complete(void) noexcept -> void {
				_completed = true;
			}


		private:

			// -- private members ---------------------------------------------

			/* rollback */
			___rollback _rollback;

			/* completed */
			bool _completed;

	}; // class exception_guard

}; // namespace cs

#endif // ___cs_requirements
#endif // ___CS_EXCEPTION_GUARD_HPP___
