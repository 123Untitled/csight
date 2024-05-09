#ifndef ___cs_containers_pair_hpp___
#define ___cs_containers_pair_hpp___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/type_traits/move.hpp"
#include "cs/type_traits/remove_const.hpp"
#include <type_traits>


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- P A I R -------------------------------------------------------------

	template <typename ___t1, typename ___t2>
	class pair {


		public:

			// -- public types ------------------------------------------------

			/* first type */
			using first_type = ___t1;

			/* second type */
			using second_type = ___t2;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = cs::pair<___t1, ___t2>;


			// -- private members ---------------------------------------------

			/* first */
			first_type _first;

			/* second */
			second_type _second;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			constexpr pair(void)
				noexcept(std::is_nothrow_default_constructible<first_type>::value
					  && std::is_nothrow_default_constructible<second_type>::value) = default;

			/* copy constructor */
			constexpr pair(const ___self&)
				noexcept(std::is_nothrow_copy_constructible<first_type>::value
					  && std::is_nothrow_copy_constructible<second_type>::value) = default;


			/* move constructor */
			constexpr pair(___self&&)
				noexcept(std::is_nothrow_move_constructible<first_type>::value
					  && std::is_nothrow_move_constructible<second_type>::value) = default;


			/* members copy constructor */
			constexpr pair(const first_type& ___f, const second_type& ___s)
				noexcept(std::is_nothrow_copy_constructible<first_type>::value
					  && std::is_nothrow_copy_constructible<second_type>::value)
			: _first{___f}, _second{___s} {
			}

			/* members move constructor */
			constexpr pair(cs::remove_const<first_type>&& ___f, cs::remove_const<second_type>&& ___s)
				noexcept(std::is_nothrow_move_constructible<first_type>::value
					  && std::is_nothrow_move_constructible<second_type>::value)
			: _first{cs::move(___f)}, _second{cs::move(___s)} {
			}

			/* members copy / move constructor */
			constexpr pair(const first_type& ___f, cs::remove_const<second_type>&& ___s)
				noexcept(std::is_nothrow_copy_constructible<first_type>::value
					  && std::is_nothrow_move_constructible<second_type>::value)
			: _first{___f}, _second{cs::move(___s)} {
			}

			/* members move / copy constructor */
			constexpr pair(cs::remove_const<first_type>&& ___f, const second_type& ___s)
				noexcept(std::is_nothrow_move_constructible<first_type>::value
					  && std::is_nothrow_copy_constructible<second_type>::value)
			: _first{cs::move(___f)}, _second{___s} {
			}


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const ___self&)
				noexcept(std::is_nothrow_copy_assignable<first_type>::value
					  && std::is_nothrow_copy_assignable<second_type>::value) -> ___self& = default;

			/* move assignment operator */
			auto operator=(___self&&)
				noexcept(std::is_nothrow_move_assignable<first_type>::value
					  && std::is_nothrow_move_assignable<second_type>::value) -> ___self& = default;


			// -- public comparison operators ---------------------------------

			/* equal operator */
			constexpr auto operator==(const ___self& ___ot) const noexcept -> bool {
				return _first == ___ot._first && _second == ___ot._second;
			}

			/* not equal operator */
			constexpr auto operator!=(const ___self& ___ot) const noexcept -> bool {
				return _first != ___ot._first || _second != ___ot._second;
			}

			/* less than operator */
			constexpr auto operator< (const ___self&) const noexcept -> bool = delete;

			/* less than or equal operator */
			constexpr auto operator<=(const ___self&) const noexcept -> bool = delete;

			/* greater than operator */
			constexpr auto operator> (const ___self&) const noexcept -> bool = delete;

			/* greater than or equal operator */
			constexpr auto operator>=(const ___self&) const noexcept -> bool = delete;




		// -- friends ---------------------------------------------------------

		/* get first (const reference) */
		template <typename ___t1_, typename ___t2_>
		friend constexpr auto get(const cs::pair<___t1_, ___t2_>&) noexcept -> const ___t1_&;

		/* get second (const reference) */
		template <typename ___t1_, typename ___t2_>
		friend constexpr auto get(const cs::pair<___t2_, ___t1_>&) noexcept -> const ___t1_&;

		/* get first (reference) */
		template <typename ___t1_, typename ___t2_>
		friend constexpr auto get(cs::pair<___t1_, ___t2_>&) noexcept -> ___t1_&;

		/* get second (reference) */
		template <typename ___t1_, typename ___t2_>
		friend constexpr auto get(cs::pair<___t2_, ___t1_>&) noexcept -> ___t1_&;

		/* get first (move reference) */
		template <typename ___t1_, typename ___t2_>
		friend constexpr auto get(cs::pair<___t1_, ___t2_>&&) noexcept -> ___t1_&&;

		/* get second (move reference) */
		template <typename ___t1_, typename ___t2_>
		friend constexpr auto get(cs::pair<___t2_, ___t1_>&&) noexcept -> ___t1_&&;

		/* get first (const move reference) */
		template <typename ___t1_, typename ___t2_>
		friend constexpr auto get(const cs::pair<___t1_, ___t2_>&&) noexcept -> const ___t1_&&;

		/* get second (const move reference) */
		template <typename ___t1_, typename ___t2_>
		friend constexpr auto get(const cs::pair<___t2_, ___t1_>&&) noexcept -> const ___t1_&&;

	}; // class pair


	// -- non-member functions ------------------------------------------------

	/* get first (const reference) */
	template <typename ___t1, typename ___t2>
	constexpr auto get(const cs::pair<___t1, ___t2>& ___p) noexcept -> const ___t1& {
		return ___p._first;
	}

	/* get second (const reference) */
	template <typename ___t1, typename ___t2>
	constexpr auto get(const cs::pair<___t2, ___t1>& ___p) noexcept -> const ___t1& {
		return ___p._second;
	}

	/* get first (reference) */
	template <typename ___t1, typename ___t2>
	constexpr auto get(cs::pair<___t1, ___t2>& ___p) noexcept -> ___t1& {
		return ___p._first;
	}

	/* get second (reference) */
	template <typename ___t1, typename ___t2>
	constexpr auto get(cs::pair<___t2, ___t1>& ___p) noexcept -> ___t1& {
		return ___p._second;
	}

	/* get first (move reference) */
	template <typename ___t1, typename ___t2>
	constexpr auto get(cs::pair<___t1, ___t2>&& ___p) noexcept -> ___t1&& {
		return cs::move(___p._first);
	}

	/* get second (move reference) */
	template <typename ___t1, typename ___t2>
	constexpr auto get(cs::pair<___t2, ___t1>&& ___p) noexcept -> ___t1&& {
		return cs::move(___p._second);
	}

	/* get first (const move reference) */
	template <typename ___t1, typename ___t2>
	constexpr auto get(const cs::pair<___t1, ___t2>&& ___p) noexcept -> const ___t1&& {
		return cs::move(___p._first);
	}

	/* get second (const move reference) */
	template <typename ___t1, typename ___t2>
	constexpr auto get(const cs::pair<___t2, ___t1>&& ___p) noexcept -> const ___t1&& {
		return cs::move(___p._second);
	}


} // namespace cs

#endif // ___cs_requirements
#endif // ___cs_containers_pair_hpp___
