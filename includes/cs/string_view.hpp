#ifndef ___CS_STRING_VIEW_HPP___
#define ___CS_STRING_VIEW_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/types.hpp"
#include "cs/strlen.hpp"

//#include <iosfwd>
#include <iostream>


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- S T R I N G  V I E W --------------------------------------------------

	class string_view final {


		private:

			// -- private types ------------------------------------------------

			/* self type */
			using ___self = cs::string_view;


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

			// -- private members ----------------------------------------------

			/* data */
			const_pointer _data;

			/* size */
			size_type _size;


			// -- private static members --------------------------------------

			/* empty */
			static constexpr const_pointer _empty = "";


		public:

			// -- public lifecycle ---------------------------------------------

			/* default constructor */
			constexpr string_view(void) noexcept
			: _data{_empty}, _size{0U} {
			}

			/* buffer constructor */
			explicit constexpr string_view(const_pointer data) noexcept
			: _data{data}, _size{cs::strlen(data)} {
			}

			/* members constructor */
			constexpr string_view(const_pointer data, size_type size) noexcept
			: _data{data}, _size{size} {
			}

			/* copy constructor */
			constexpr string_view(const ___self&) noexcept = default;

			/* move constructor */
			constexpr string_view(___self&&) noexcept = default;

			/* destructor */
			constexpr ~string_view(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			constexpr auto operator=(const ___self&) noexcept -> ___self& = default;

			/* move assignment operator */
			constexpr auto operator=(___self&&) noexcept -> ___self& = default;

			/* buffer assignment operator */
			constexpr auto operator=(const_pointer data) noexcept -> ___self& {
				_data = data;
				_size = cs::strlen(data);
				return *this;
			}


			// -- public accessors --------------------------------------------

			/* data */
			constexpr auto data(void) const noexcept -> const_pointer {
				return _data;
			}

			/* size */
			constexpr auto size(void) const noexcept -> size_type {
				return _size;
			}

			/* empty */
			constexpr auto empty(void) const noexcept -> bool {
				return _size == 0U;
			}


			// -- public element access ---------------------------------------

			/* operator[] */
			constexpr auto operator[](size_type pos) const noexcept -> const_reference {
				return _data[pos];
			}


			// -- public modifiers --------------------------------------------

			/* clear */
			constexpr auto clear(void) noexcept -> void {
				_data = _empty;
				_size = 0U;
			}

	}; // class string_view

} // namespace cs


// -- non-member functions ----------------------------------------------------

/* operator << */
inline auto operator<<(std::ostream& os, const cs::string_view& view) -> std::ostream& {
	return os.write(view.data(), static_cast<std::streamsize>(view.size()));
}

#endif // ___cs_requirements
#endif // ___CS_STRING_VIEW_HPP___
