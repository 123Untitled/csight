#ifndef ___CS_SYSTEM_ACCESS_HPP___
#define ___CS_SYSTEM_ACCESS_HPP___


#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/string.hpp"
#include <unistd.h>


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- A C C E S S ------------------------------------------------------------

	/* access */
	template <int ___mode>
	auto access(const cs::string& ___path) noexcept -> bool {
		// check for invalid mode
		static_assert((___mode & ~(F_OK | R_OK | W_OK | X_OK)) == 0, "invalid mode");
		// call access
		return ::access(___path.data(), ___mode) == 0;
	}

} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_SYSTEM_ACCESS_HPP___
