#ifndef ___CS_TYPE_TRAITS_DECLVAL_HPP___
#define ___CS_TYPE_TRAITS_DECLVAL_HPP___

#include "cs/config.hpp"
#if ___cs_requirements


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- D E C L V A L -------------------------------------------------------

	namespace ___impl {

		template <typename ___type>
		auto declval(int) noexcept -> ___type&&;

		template <typename ___type>
		auto declval(long) noexcept -> ___type;
	}


	/* declval */
	template <typename ___type>
	auto declval(void) noexcept -> decltype(___impl::declval<___type>(0));

} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_TYPE_TRAITS_DECLVAL_HPP___
