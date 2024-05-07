#ifndef ___CS_MEMORY_LIFECYCLE_HPP___
#define ___CS_MEMORY_LIFECYCLE_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/type_traits/move.hpp"
#include "cs/type_traits/forward.hpp"
#include "cs/type_traits/is_trivially_destructible.hpp"


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- L I F E C Y C L E ---------------------------------------------------

	template <typename ___type>
	class lifecycle final {


		public:

			// -- public types ------------------------------------------------

			/* self type */
			using self       = cs::lifecycle<___type>;

			/* value type */
			using value_type = ___type;

			/* pointer type */
			using pointer    = value_type*;
			//using pointer    = xns::remove_const<value_type>*;


			// -- public lifecycle --------------------------------------------

			/* non-instantiable class */
			___cs_not_instantiable(lifecycle);


			// -- public static methods ---------------------------------------

			/* default construct */
			static constexpr auto construct(pointer ___p)
				/* noexcept(xns::is_nothrow_default_constructible<value_type>) */ -> void {
				//static_assert(xns::is_default_constructible<value_type>,
				//		"lifecycle, type is not default constructible");
				::new((void*)___p) value_type{};
			}

			/* copy construct */
			static constexpr auto construct(pointer ___p, const value_type& ___value)
				/* noexcept(xns::is_nothrow_copy_constructible<value_type>) */ -> void {
				//static_assert(xns::is_copy_constructible<value_type>,
				//		"lifecycle, type is not copy constructible");
				::new((void*)___p) value_type{___value};
			}

			/* move construct */
			static constexpr auto construct(pointer ___p, value_type&& ___value)
				/* noexcept(xns::is_nothrow_move_constructible<value_type>) */ -> void {
				//static_assert(xns::is_move_constructible<value_type>,
				//		"lifecycle, type is not move constructible");
				::new((void*)___p) value_type{cs::move(___value)};
			}

			/* forward construct */
			template <typename... ___params> /* requires (sizeof...(___params) > 1
												   || (sizeof...(___params) == 1
												   && xns::are_not_same<value_type, xns::remove_cvref<___params>...>)) */
			static constexpr auto construct(pointer ___p, ___params&&... ___args)
				/* noexcept(xns::is_nothrow_constructible<value_type, ___params...>) */ -> void {
				//static_assert(xns::is_constructible<value_type, ___params...>,
				//		"lifecycle, type is not constructible with given arguments");
				// use () to construct to let implicit conversion work
				::new((void*)___p) value_type{cs::forward<___params>(___args)...};
			}

			/* destroy */
			static constexpr void destroy(pointer ___p) noexcept {

				// check if type is trivially destructible
				if constexpr (cs::is_trivially_destructible<value_type>)
					return;
				else {

					// check if type throws on destruction
					//static_assert(xns::is_nothrow_destructible<value_type>,
					//	"lifecycle, requires type to be nothrow destructible");

					// check type is destructible
					//static_assert(xns::is_destructible<value_type>,
					//		"lifecycle, type is not destructible");

					___p->~value_type();
				}
			}

	}; // class lifecycle

} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_MEMORY_LIFECYCLE_HPP___
