#ifndef CS_MEMMOVE_HEADER
#define CS_MEMMOVE_HEADER

#include "cs/config.hpp"

#if ___cs_requirements

#include "cs/types.hpp"

#if !__has_builtin(__builtin_memmove)
#define CS_USE_SYSTEM_MEMMOVE
#include <string.h>
#endif

// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- M E M M O V E -------------------------------------------------------

	/* memmove */
	template <typename ___type>
	auto memmove(___type* ___dst, const ___type* ___src, const cs::size_t ___sz) noexcept -> void {

		#if defined(MSH_USE_SYSTEM_MEMMOVE)
			static_cast<void>(::memmove(___dst, ___src, ___sz * sizeof(___type)));
		#else
			static_cast<void>(__builtin_memmove(___dst, ___src, ___sz * sizeof(___type)));
		#endif
	}

} // namespace cs

#endif // ___cs_requirements

#endif // CS_MEMMOVE_HEADER
