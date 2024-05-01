#ifndef CS_IS_TRIVIALLY_DESTRUCTIBLE_HEADER
#define CS_IS_TRIVIALLY_DESTRUCTIBLE_HEADER

#include "cs/config.hpp"

#if ___cs_requirements


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- I S  T R I V I A L L Y  D E S T R U C T I B L E -------------------------

	/* is_trivially_destructible */
	template <typename ___type>
	concept is_trivially_destructible = __is_trivially_destructible(___type);

} // namespace cs

#endif // ___cs_requirements

#endif // CS_IS_TRIVIALLY_DESTRUCTIBLE_HEADER
