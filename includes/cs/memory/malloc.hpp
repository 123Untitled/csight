#ifndef ___CS_MEMORY_MALLOC_HPP___
#define ___CS_MEMORY_MALLOC_HPP___

#include "cs/config.hpp"

#if ___cs_requirements

#if    !__has_builtin(__builtin_malloc)  \
	|| !__has_builtin(__builtin_realloc) \
	|| !__has_builtin(__builtin_free)
#	define CS_USE_SYSTEM_MALLOC
#	include <cstdlib>
#endif


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- M A L L O C ---------------------------------------------------------

	/* malloc */
	template <typename ___type>
	auto malloc(const decltype(sizeof(0)) ___sz = 1U) noexcept -> ___type* {

		#if defined(CS_USE_SYSTEM_MALLOC)
			return static_cast<___type*>(::malloc(___sz * sizeof(___type)));
		#else
			return static_cast<___type*>(__builtin_malloc(___sz * sizeof(___type)));
		#endif
	}

	/* realloc */
	template <typename ___type>
	auto realloc(___type* ___ptr, const decltype(sizeof(0)) ___sz) noexcept -> ___type* {

		#if defined(CS_USE_SYSTEM_MALLOC)
			return static_cast<___type*>(::realloc(___ptr, ___sz * sizeof(___type)));
		#else
			return static_cast<___type*>(__builtin_realloc(___ptr, ___sz * sizeof(___type)));
		#endif
	}

	/* free */
	template <typename ___type>
	auto free(___type* ___ptr) noexcept -> void {

		#if defined(CS_USE_SYSTEM_MALLOC)
			::free(___ptr);
		#else
			__builtin_free(___ptr);
		#endif
	}

} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_MEMORY_MALLOC_HPP___
