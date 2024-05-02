#ifndef ___CS_CONTAINERS_ARRAY_HPP___
#define ___CS_CONTAINERS_ARRAY_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/types.hpp"


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- A R R A Y -----------------------------------------------------------

	template <typename ___type, cs::size_t ___size>
	class array final {

		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = cs::array<___type, ___size>;

		public:

			// -- public types ------------------------------------------------

			/* value type */
			using value_type = ___type;

			/* reference type */
			using reference = value_type&;

			/* const reference type */
			using const_reference = const value_type&;

			/* pointer type */
			using pointer = value_type*;

			/* const pointer type */
			using const_pointer = const value_type*;

			/* size type */
			using size_type = cs::size_t;



			// -- public members ----------------------------------------------

			/* data */
			value_type _data[___size];


			// -- public accessors --------------------------------------------

			/* size */
			static consteval auto size(void) noexcept -> size_type {
				return ___size;
			}

	}; // class array

} // namespace cs

#endif // ___cs_requirements

#endif // ___CS_CONTAINERS_ARRAY_HPP___
