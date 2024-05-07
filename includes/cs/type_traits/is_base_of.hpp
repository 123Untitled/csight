#ifndef ___CS_TYPE_TRAITS_IS_BASE_OF_HPP___
#define ___CS_TYPE_TRAITS_IS_BASE_OF_HPP___


#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/type_traits/integral_constant.hpp"


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- I S  B A S E  O F ---------------------------------------------------

	namespace ___impl {


		/* test conversion */
		template <typename ___base>
		auto ___test_conversion(const volatile ___base*) -> cs::true_type;

		/* test conversion */
		template <typename>
		auto ___test_conversion(const volatile void*) -> cs::false_type;


		/* test is base of */
		template <typename ___base, typename ___derived>
		auto ___test_is_base_of(int) -> decltype(___test_conversion<___base>(static_cast<___derived*>(nullptr)));

		/* test is base of */
		template<typename, typename>
		auto ___test_is_base_of(...) -> cs::true_type;

	} // namespace ___impl


	template <typename ___base, typename ___derived>
	concept is_base_of = cs::bool_constant<
	/*
			std::is_class<Base>::value &&
			std::is_class<Derived>::value &&
			*/
			decltype(cs::___impl::___test_is_base_of<___base, ___derived>(0))::value
			>::value;


} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_TYPE_TRAITS_IS_BASE_OF_HPP___
