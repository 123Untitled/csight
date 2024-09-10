#ifndef ___CS_STRING_HPP___
#define ___CS_STRING_HPP___


#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/types.hpp"
#include "cs/memory/memcpy.hpp"
#include "cs/string_view.hpp"

#include <iostream>


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- S T R I N G ---------------------------------------------------------

	class string final {


		public:

			// -- public types ------------------------------------------------

			/* value type */
			using value_type      = char;

			/* reference type */
			using reference       = value_type&;

			/* const reference type */
			using const_reference = const value_type&;

			/* pointer type */
			using pointer         = value_type*;

			/* const pointer type */
			using const_pointer   = const value_type*;

			/* size type */
			using size_type       = cs::size_t;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = cs::string;


			// -- private members ---------------------------------------------

			/* data */
			pointer _data;

			/* size */
			size_type _size;

			/* capacity */
			size_type _capacity;


		public:


			auto print(void) const -> void {
				std::cout << "    data: [\x1b[32m" << _data << "\x1b[0m]" << std::endl;
				std::cout << "    size: " << _size << std::endl;
				std::cout << "capacity: " << _capacity << std::endl;
			}


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			string(void) noexcept;

			/* variadic constructor */
			template <typename... ___params>
			explicit string(const ___params&... ___args) {

				// compute required size
				_capacity = 0U + (___self::_size_multi(___args) + ...);

				// reserve capacity
				_data = ___self::_allocate(_capacity);

				// offset
				size_type ___of = 0U;

				// append and update size
				_size = 0U + (___self::_append_multi(___of, ___args) + ...);

				// null terminate
				___self::_null_terminate();
			}

			/* copy constructor */
			string(const ___self& other) noexcept
			: _data{_empty}, _size{0U}, _capacity{0U} {

				// check other size
				if (other._size == 0U)
					return;

				// allocate memory
				_data = _allocate(other._size);

				// copy data
				cs::memcpy(_data, other._data, other._size);

				// update size
				_size = other._size;

				// null terminate
				_null_terminate();

				// update capacity
				_capacity = _size;
			}

			/* move constructor */
			string(___self&&) noexcept;

			/* destructor */
			~string(void) noexcept;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const ___self&) -> ___self& = delete;

			/* move assignment operator */
			auto operator=(___self&&) noexcept -> ___self&;


			// -- public accessors --------------------------------------------

			/* data */
			auto data(void) noexcept -> pointer;

			/* const data */
			auto data(void) const noexcept -> const_pointer;

			/* size */
			auto size(void) const noexcept -> size_type;

			/* capacity */
			auto capacity(void) const noexcept -> size_type;

			/* empty */
			auto empty(void) const noexcept -> bool;

			/* front */
			auto front(void) noexcept -> reference;

			/* front */
			auto front(void) const noexcept -> const_reference;

			/* back */
			auto back(void) noexcept -> reference;

			/* back */
			auto back(void) const noexcept -> const_reference;


			// -- public element access ---------------------------------------

			/* operator [] */
			auto operator[](const size_type) noexcept -> reference;

			/* operator [] */
			auto operator[](const size_type) const noexcept -> const_reference;


			// -- public modifiers --------------------------------------------

			/* reserve */
			auto reserve(const size_type) -> void;

			/* resize */
			auto resize(const size_type) -> void;

			/* clear */
			auto clear(void) noexcept -> void;

			/* push back */
			auto push_back(const value_type) -> void;

			/* pop back */
			auto pop_back(const size_type = 1U) noexcept -> void;

			/* append multi */
			template <typename... ___params>
			auto append(const ___params&... ___args) -> ___self& {

				// check if any arguments
				if constexpr (sizeof...(___args) == 0U)
					return *this;

				// compute required size
				const auto ___req = (___self::_size_multi(___args) + ...);

				// reserve capacity
				if (___req > ___self::_available_capacity())
					___self::_reserve(
							___self::_try_expand(_size + ___req)
					);

				// offset
				size_type ___of = _size;

				// append and update size
				_size += (___self::_append_multi(___of, ___args) + ...);

				// null terminate
				___self::_null_terminate();

				return *this;
			}

			/* insert multi */
			/*
			template <typename... ___params>
			auto insert(const size_type ___pos, const ___params&... ___args) -> ___self& {

				// check if any arguments
				if constexpr (sizeof...(___args) == 0U)
					return *this;

				// check position
				if (___pos > _size)
					return *this;

				// compute required size
				const auto ___req = (___self::_size_multi(___args) + ...);

				// reserve capacity
				if (___req > ___self::_available_capacity())
					___self::_reserve(___self::_try_expand(_size + ___req));

				// shift data
				msh::memmove(_data + ___pos + ___req,
							 _data + ___pos, _size - ___pos);

				// offset
				size_type ___of = ___pos;

				// insert and update size
				_size += (___self::_append_multi(___of, ___args) + ...);

				// null terminate
				_null_terminate();

				return *this;
			}
			*/

			/* erase */
			auto erase(const size_type) noexcept -> void;



			// -- public comparison operators ---------------------------------

			/* equal */
			auto operator==(const ___self&) const noexcept -> bool;

			/* not equal */
			auto operator!=(const ___self&) const noexcept -> bool;

			/* less than */
			auto operator<(const ___self&) const noexcept -> bool;

			/* greater than */
			auto operator>(const ___self&) const noexcept -> bool;

			/* less than or equal to */
			auto operator<=(const ___self&) const noexcept -> bool;

			/* greater than or equal to */
			auto operator>=(const ___self&) const noexcept -> bool;


		private:

			// -- private static members --------------------------------------

			/* empty string */
			static value_type _empty[1];


			// -- private static methods --------------------------------------

			/* allocate */
			static auto _allocate(const size_type) -> pointer;


			// -- private methods ---------------------------------------------

			/* init */
			auto _init(void) noexcept -> void;

			/* copy members */
			auto _copy_members(___self&) noexcept -> void;

			/* reallocate */
			auto _reallocate(const size_type) -> pointer;

			/* null terminate */
			auto _null_terminate(void) noexcept -> void;

			/* null terminate */
			auto _null_terminate(const size_type) noexcept -> void;

			/* available capacity */
			auto _available_capacity(void) const noexcept -> size_type;

			/* expand */
			auto _expand(void) const noexcept -> size_type;

			/* try expand */
			auto _try_expand(const size_type) const noexcept -> size_type;

			/* deallocate */
			auto _deallocate(void) noexcept -> void;

			/* reserve */
			auto _reserve(const size_type) -> void;


			// -- private comparison methods ----------------------------------

			/* compare */
			auto _compare(const ___self&) const noexcept -> int;


			// -- append multi methods ----------------------------------------

			/* append multi (character) */
			auto _append_multi(size_type&, const value_type) -> size_type;

			/* append multi (string) */
			auto _append_multi(size_type&, const ___self&) -> size_type;

			/* append multi (string view) */
			auto _append_multi(size_type&, const cs::string_view&) -> size_type;

			/* append multi (array) */
			template <size_type ___sz>
			auto _append_multi(size_type& ___of, const value_type (&___ar)[___sz]) -> size_type {

				// copy array
				cs::memcpy(_data + ___of, ___ar, ___sz - 1U);
				// update offset
				___of += ___sz - 1U;

				return ___sz - 1U;
			}

			/* append multi (integer) */
			template <typename ___type> requires (std::is_integral_v<___type>
											   && not std::is_same_v<char, ___type>
											   && std::is_unsigned_v<___type>)
			auto _append_multi(size_type& ___of, const ___type& ___val) -> size_type {

				// copy integer
				___type ___num = ___val;

				// get offset position
				const size_type ___st = ___of;

				// loop over all digits
				do {
					_data[___of] = static_cast<value_type>((___num % 10U) + '0');
					___num /= 10U;
					++___of;
				} while (___num != 0U);


				// reverse all integral digits
				for (size_type start = ___st,
								 end = ___of - 1U; start < end; ++start, --end) {

					// swap characters
					const char tmp = _data[start];
					_data[start]   = _data[end];
					_data[end]     = tmp;
				}

				// return number of characters
				return ___of - ___st;
			}




			/* size multi (character) */
			static auto _size_multi(const value_type) noexcept -> size_type;

			/* size multi (string) */
			static auto _size_multi(const ___self&) noexcept -> size_type;

			/* size multi (string view) */
			static auto _size_multi(const cs::string_view&) noexcept -> size_type;

			/* size multi (array) */
			template <size_type ___sz>
			static auto _size_multi(const value_type (&)[___sz]) noexcept -> size_type {

				// check array size
				static_assert(___sz > 0U, "array must have at least one element");
				// assume array is null terminated
				return ___sz - 1U;
			}

			/* size multi (integer) */
			template <typename ___type> requires (std::is_integral_v<___type>
											&& not std::is_same_v<char, ___type>)
			static auto _size_multi(const ___type&) noexcept -> size_type {

				// return max number of digits
				return std::numeric_limits<___type>::digits10;
			}

	}; // class string


	// -- non-member functions ------------------------------------------------

	/* is ctrl */
	auto is_ctrl(const string::value_type) noexcept -> bool;

} // namespace cs


// -- non-member functions ----------------------------------------------------

/* operator << */
auto operator<<(std::ostream&, const cs::string&) -> std::ostream&;

#endif // ___cs_requirements
#endif // ___CS_STRING_HPP___
