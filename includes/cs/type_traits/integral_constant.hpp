#ifndef ___CS_TYPE_TRAITS_INTEGRAL_CONSTANT_HPP___
#define ___CS_TYPE_TRAITS_INTEGRAL_CONSTANT_HPP___


#include "cs/config.hpp"
#if ___cs_requirements


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- I N T E G R A L  C O N S T A N T ------------------------------------

	template <typename ___type, ___type ___vl>
	struct integral_constant {


		// -- public types ----------------------------------------------------

		/* value type */
		using value_type = ___type;

		/* injected-class-name */
		using type = cs::integral_constant<___type, ___vl>;


		// -- public constants ------------------------------------------------

		/* value */
		static constexpr value_type value = ___vl;


		// -- public conversion operators -------------------------------------

		/* value_type conversion operator */
		constexpr operator value_type(void) const noexcept {
			return value;
		}


		// -- public operators ------------------------------------------------

		/* operator() */
		constexpr auto operator()(void) const noexcept -> value_type {
			return value;
		}

	}; // struct integral_constant


	/* bool constant */
	template <bool ___vl>
	using bool_constant = cs::integral_constant<bool, ___vl>;

	/* true type */
	using true_type = cs::bool_constant<true>;

	/* false type */
	using false_type = cs::bool_constant<false>;

} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_TYPE_TRAITS_INTEGRAL_CONSTANT_HPP___
