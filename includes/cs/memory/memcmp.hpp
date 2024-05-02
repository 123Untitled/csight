#ifndef ___CS_MEMCMP_HPP___
#define ___CS_MEMCMP_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/types.hpp"


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- M E M C M P -----------------------------------------------------------

	/* memcmp */
	template <typename ___type>
	auto memcmp(const ___type& ___lhs, const ___type& ___rhs) noexcept -> int {

		auto* lhs = reinterpret_cast<const unsigned char*>(&___lhs);
		auto* rhs = reinterpret_cast<const unsigned char*>(&___rhs);

		for (cs::size_t i = 0U; i < sizeof(___type); ++i) {

			if (lhs[i] == rhs[i])
				continue;

			return lhs[i] < rhs[i] ? -1 : 1;
		}
		return 0;
	}


} // namespace cs

#endif // ___cs_requirements

#endif // ___CS_MEMCMP_HPP___
