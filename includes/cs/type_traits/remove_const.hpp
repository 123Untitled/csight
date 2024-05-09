#ifndef ___cs_type_traits_remove_const_hpp___
#define ___cs_type_traits_remove_const_hpp___

#include "cs/config.hpp"
#if ___cs_requirements


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- R E M O V E  C O N S T ----------------------------------------------

	namespace ___impl {

		/* remove const */
		template <typename ___type>
		struct ___remove_const {
			using type = ___type;
		};

		/* const specialization */
		template <typename ___type>
		struct ___remove_const<const ___type> {
			using type = ___type;
		};

	} // namespace ___impl

	/* remove const */
	template <typename ___type>
	using remove_const = typename ___impl::___remove_const<___type>::type;

} // namespace cs

#endif // ___cs_requirements
#endif // ___cs_type_traits_remove_const_hpp___
