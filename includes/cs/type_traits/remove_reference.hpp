#ifndef CS_REMOVE_REFERENCE_HEADER
#define CS_REMOVE_REFERENCE_HEADER

#include "cs/config.hpp"

#if ___cs_requirements

// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- R E M O V E  R E F E R E N C E --------------------------------------

	namespace ___impl {

		/* remove_reference */
		template <typename ___type>
		struct ___remove_reference final {
			using type = ___type;
			___cs_not_instantiable(___remove_reference);
		};

		/* remove_reference */
		template <typename ___type>
		struct ___remove_reference<___type&> final {
			using type = ___type;
			___cs_not_instantiable(___remove_reference);
		};

		/* remove_reference */
		template <typename ___type>
		struct ___remove_reference<___type&&> final {
			using type = ___type;
			___cs_not_instantiable(___remove_reference);
		};

	} // namespace ___impl


	/* remove_reference */
	template <typename ___type>
	using remove_reference = typename ___impl::___remove_reference<___type>::type;

} // namespace cs

#endif // ___cs_requirements

#endif // CS_REMOVE_REFERENCE_HEADER
