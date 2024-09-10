#ifndef CS_MEMCPY_HEADER
#define CS_MEMCPY_HEADER

#include "cs/config.hpp"

#if ___cs_requirements

#include "cs/types.hpp"

#if !__has_builtin(__builtin_memcpy)
#define CS_USE_SYSTEM_MEMCPY
#include <string.h>
#endif


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- M E M C P Y -----------------------------------------------------------

	/* memcpy */
	template <typename ___type>
	auto memcpy(___type* ___dst, const ___type* ___src, const cs::size_t ___sz) noexcept -> void {

		#if defined(CS_USE_SYSTEM_MEMCPY)
			static_cast<void>(::memcpy(___dst, ___src, ___sz * sizeof(___type)));
		#else
			static_cast<void>(__builtin_memcpy(___dst, ___src, ___sz * sizeof(___type)));
		#endif
	}

} // namespace cs

#endif // ___cs_requirements

#endif // CS_MEMCPY_HEADER
