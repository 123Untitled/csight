#ifndef CS_CONDITIONAL_HEADER
#define CS_CONDITIONAL_HEADER

#include "cs/config.hpp"

#if ___cs_requirements


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- C O N D I T I O N A L ------------------------------------------------

	namespace ___impl {


		/* forward declaration */
		template <bool, typename, typename>
		struct ___conditional;

		/* true specialization */
		template <typename ___true, typename ___false>
		struct ___conditional<true, ___true, ___false> final {
			using type = ___true;
			___cs_not_instantiable(___conditional);
		};

		/* false specialization */
		template <typename ___true, typename ___false>
		struct ___conditional<false, ___true, ___false> final {
			using type = ___false;
			___cs_not_instantiable(___conditional);
		};

	} // namespace ___impl


	/* conditional */
	template <bool ___condition, typename ___true, typename ___false>
	using conditional = typename ___impl::___conditional<___condition, ___true, ___false>::type;

} // namespace cs

#endif // ___cs_requirements

#endif // CS_CONDITIONAL_HEADER
