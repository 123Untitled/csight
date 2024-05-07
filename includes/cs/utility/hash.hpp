#ifndef ___cs_utiliy_hash_hpp___
#define ___cs_utiliy_hash_hpp___


#include "cs/config.hpp"
#if ___cs_requirements

#include <stdint.h>



// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// stateful hash class (create a hash gradually)

	template <unsigned>
	class hash;


	template <>
	class hash<32U> final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = cs::hash<32U>;


		public:

			// -- public types ------------------------------------------------

			/* hash type */
			using hash_type = uint32_t;


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			constexpr hash(void) noexcept
			: _hash{0U} {
			}

			/* copy constructor */
			constexpr hash(const ___self&) noexcept = default;

			/* move constructor */
			constexpr hash(___self&&) noexcept = default;

			/* destructor */
			~hash(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			constexpr auto operator=(const ___self&) noexcept -> ___self& = default;

			/* move assignment operator */
			constexpr auto operator=(___self&&) noexcept -> ___self& = default;


			// -- public modifiers --------------------------------------------

			/* update */
			constexpr auto update(const char ___ch) noexcept -> void {
				// jenkins one at a time hash
				_hash += static_cast<unsigned char>(___ch);
				_hash += _hash << 10U;
				_hash ^= _hash >> 6U;
			}

			/* finalize */
			constexpr auto finalize(void) noexcept -> void {
				// jenkins one at a time hash
				_hash += _hash << 3U;
				_hash ^= _hash >> 11U;
				_hash += _hash << 15U;
			}

			/* reset */
			constexpr auto reset(void) noexcept -> void {
				_hash = 0U;
			}


			// -- public accessors --------------------------------------------

			/* value */
			constexpr auto value(void) const noexcept -> hash_type {
				return _hash;
			}


			// -- public conversion operators ---------------------------------

			/* hash type conversion operator */
			constexpr operator hash_type(void) const noexcept {
				return _hash;
			}


		private:

			// -- private members ---------------------------------------------

			/* hash */
			hash_type _hash;

	}; // class hash




} // namespace cs

#endif // ___cs_requirements
#endif // ___cs_utiliy_hash_hpp___










