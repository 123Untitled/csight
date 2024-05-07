#ifndef ___CS_SYSTEM_CWD_HPP___
#define ___CS_SYSTEM_CWD_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include <sys/param.h>
#include "cs/string_view.hpp"


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- C W D ---------------------------------------------------------------

	namespace ___impl {


		/* cwd */
		class ___cwd final {


			public:

				// -- public static accessors ---------------------------------

				/* view */
				static auto view(void) -> const cs::string_view&;


			private:

				// -- private types -------------------------------------------

				/* self type */
				using ___self = cs::___impl::___cwd;


				// -- private lifecycle ---------------------------------------

				/* default constructor */
				___cwd(void);

				/* non-copyable class */
				___cs_not_copyable(___cwd);

				/* non-movable class */
				___cs_not_moveable(___cwd);

				/* destructor */
				~___cwd(void) noexcept = default;


				// -- private static methods ----------------------------------

				/* shared */
				static auto _shared(void) -> const ___self&;


				// -- private members -----------------------------------------

				/* buffer */
				char _buffer[PATH_MAX];

				/* view */
				cs::string_view _view;

		}; // class ___cwd

	} // namespace ___impl


	// -- non-member functions ------------------------------------------------

	/* cwd */
	auto cwd(void) -> const cs::string_view&;

} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_SYSTEM_CWD_HPP___
