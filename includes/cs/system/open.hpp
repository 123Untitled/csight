#ifndef ___CS_SYSTEM_OPEN_HPP___
#define ___CS_SYSTEM_OPEN_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include <fcntl.h>
#include "cs/system/descriptor.hpp"
#include "cs/diagnostics/exception.hpp"
#include "cs/string.hpp"


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- O P E N -------------------------------------------------------------

	/* open */
	template <typename... ___params>
	auto open(const char* ___path, const ___params&... ___args) -> cs::descriptor {

		// open descriptor
		auto ___desc = ::open(___path, ___args...);
		if (___desc == cs::descriptor::INVALID_DESCRIPTOR)
			throw cs::runtime_error("failed to open descriptor");

		// return descriptor
		return cs::descriptor{___desc};
	}

	/* open */
	template <typename... ___params>
	auto open(const cs::string& ___path, const ___params&... ___args) -> cs::descriptor {
		return cs::open(___path.data(), ___args...);
	}

} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_SYSTEM_OPEN_HPP___
