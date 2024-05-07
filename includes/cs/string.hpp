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


	// -- S T R I N G -----------------------------------------------------------

	class string final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = cs::string;

		public:


			auto print(void) const -> void {

				std::cout << "\x1b[32mdata: [\x1b[0m" << _data << "\x1b[32m]\x1b[0m" << std::endl;
				std::cout << "size: " << _size << std::endl;
				std::cout << "capacity: " << _capacity << std::endl;

			}


			// -- public types ------------------------------------------------

			/* value type */
			using value_type = char;

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


			// -- private members ---------------------------------------------

			/* data */
			pointer _data;

			/* size */
			size_type _size;

			/* capacity */
			size_type _capacity;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			string(void) noexcept;

			/* array constructor */
			template <size_type ___sz>
			explicit string(const value_type (&___ar)[___sz]) noexcept
			: _data{_empty}, _size{___sz - 1U}, _capacity{_size} {

				// check array size
				static_assert(___sz > 0U,
					"array must have at least one element");

				// allocate memory
				_data = _allocate(_size);

				// copy array
				cs::memcpy(_data, ___ar, _size);

				// null terminate
				_null_terminate();
			}

			/* deleted copy constructor */
			string(const ___self&) = delete;

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
			auto append(const ___params&... ___args) -> void {

				// check if any arguments
				if constexpr (sizeof...(___args) == 0U)
					return;

				// compute size
				const auto ___sz = _size + (_size_multi(___args) + ...);

				// check available capacity
				if (___sz > _available_capacity())
					_reserve(_try_expand(___sz));

				// offset
				size_type ___of = _size;

				// append
				(_append_multi(___of, ___args), ...);

				// update size
				_size = ___sz;

				// null terminate
				_null_terminate();
			}


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
			auto _append_multi(size_type&, const value_type) -> void;

			/* append multi (string) */
			auto _append_multi(size_type&, const ___self&) -> void;

			/* append multi (string view) */
			auto _append_multi(size_type&, const cs::string_view&) -> void;

			/* append multi (array) */
			template <size_type ___sz>
			auto _append_multi(size_type& ___of, const value_type (&___ar)[___sz]) -> void {

				// copy array
				cs::memcpy(_data + ___of, ___ar, ___sz - 1U);
				// update offset
				___of += ___sz - 1U;
			}

			/* size multi (character) */
			static auto _size_multi(const value_type) noexcept -> size_type;

			/* size multi (string) */
			static auto _size_multi(const ___self&) noexcept -> size_type;

			/* size multi (string view) */
			static auto _size_multi(const cs::string_view&) noexcept -> size_type;

			/* size multi */
			template <size_type ___sz>
			static auto _size_multi(const value_type (&)[___sz]) noexcept -> size_type {
				// check array size
				static_assert(___sz > 0U, "array must have at least one element");
				// assume array is null terminated
				return ___sz - 1U;
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
