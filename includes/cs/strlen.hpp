#ifndef ___CS_STRLEN_HPP___
#define ___CS_STRLEN_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/types.hpp"


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- S T R L E N -----------------------------------------------------------

	/* strlen */
	constexpr auto strlen(const char* str) noexcept -> cs::size_t {

		#if __has_builtin(__builtin_strlen)
			return __builtin_strlen(str);
		#else
			const char* ptr = str;
			while (*ptr)
				++ptr;
			return static_cast<cs::size_t>(ptr - str);
		#endif
	}

} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_STRLEN_HPP___
