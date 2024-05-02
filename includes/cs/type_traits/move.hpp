#ifndef ___CS_MOVE_HPP___
#define ___CS_MOVE_HPP___


#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/type_traits/remove_reference.hpp"


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- M O V E ---------------------------------------------------------------

	/* move */
	template <typename ___type>
	constexpr auto move(___type&& ___vl) noexcept -> cs::remove_reference<___type>&& {
		return static_cast<cs::remove_reference<___type>&&>(___vl);
	}

} // namespace cs

#endif // ___cs_requirements

#endif // ___CS_MOVE_HPP___
