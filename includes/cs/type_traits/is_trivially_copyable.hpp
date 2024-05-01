#ifndef CS_IS_TRIVIALLY_COPYABLE_HEADER
#define CS_IS_TRIVIALLY_COPYABLE_HEADER

#include "cs/config.hpp"

#if ___cs_requirements

// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- I S  T R I V I A L L Y  C O P Y A B L E -----------------------------

	/* is_trivially_copyable */
	template <typename ___type>
	concept is_trivially_copyable = __is_trivially_copyable(___type);

} // namespace cs

#endif // ___cs_requirements

#endif // CS_IS_TRIVIALLY_COPYABLE_HEADER
