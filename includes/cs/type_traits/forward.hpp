#ifndef CS_FORWARD_HEADER
#define CS_FORWARD_HEADER

#include "cs/config.hpp"
#include "cs/type_traits/remove_reference.hpp"

#if ___cs_requirements


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- F O R W A R D -------------------------------------------------------

	/* forward */
	template <typename ___type>
	constexpr auto forward(cs::remove_reference<___type>& ___arg) noexcept -> ___type&& {
		return static_cast<___type&&>(___arg);
	}

	/* forward */
	template <typename ___type>
	constexpr auto forward(cs::remove_reference<___type>&& ___arg) noexcept -> ___type&& {
		return static_cast<___type&&>(___arg);
	}

} // namespace cs

#endif // ___cs_requirements

#endif // CS_FORWARD_HEADER
