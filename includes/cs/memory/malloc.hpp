#ifndef ___CS_MEMORY_MALLOC_HPP___
#define ___CS_MEMORY_MALLOC_HPP___

#include "cs/config.hpp"

#if ___cs_requirements

#if    !__has_builtin(__builtin_malloc)  \
	|| !__has_builtin(__builtin_calloc)  \
	|| !__has_builtin(__builtin_realloc) \
	|| !__has_builtin(__builtin_free)
#	define CS_USE_SYSTEM_MALLOC
#	include <cstdlib>
#endif

#include "cs/diagnostics/exception.hpp"
#include "cs/types.hpp"


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- M A L L O C ---------------------------------------------------------

	/* malloc */
	template <typename ___type>
	auto malloc(const cs::size_t ___sz = 1U) -> ___type* {

		#if defined(CS_USE_SYSTEM_MALLOC)
			void* ptr = ::malloc(___sz * sizeof(___type));
		#else
			void* ptr = __builtin_malloc(___sz * sizeof(___type));
		#endif

		if (ptr == nullptr)
			throw cs::runtime_error{"malloc failed"};

		return static_cast<___type*>(ptr);
	}

	/* calloc */
	template <typename ___type>
	auto calloc(const cs::size_t ___sz) -> ___type* {

		#if defined(CS_USE_SYSTEM_MALLOC)
			void* ptr = ::calloc(___sz, sizeof(___type));
		#else
			void* ptr = __builtin_calloc(___sz, sizeof(___type));
		#endif

		if (ptr == nullptr)
			throw cs::runtime_error{"calloc failed"};

		return static_cast<___type*>(ptr);
	}

	/* realloc */
	template <typename ___type>
	auto realloc(___type* ___ptr, const cs::size_t ___sz) -> ___type* {

		#if defined(CS_USE_SYSTEM_MALLOC)
			void* ptr = ::realloc(___ptr, ___sz * sizeof(___type)));
		#else
			void* ptr = __builtin_realloc(___ptr, ___sz * sizeof(___type));
		#endif

		if (ptr == nullptr)
			throw cs::runtime_error{"realloc failed"};

		return static_cast<___type*>(ptr);
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
