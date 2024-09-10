#ifndef ___CS_MEMORY_UNIQUE_PTR_HPP___
#define ___CS_MEMORY_UNIQUE_PTR_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/memory/malloc.hpp"
#include "cs/diagnostics/exception.hpp"
#include "cs/type_traits/forward.hpp"
#include "cs/type_traits/is_base_of.hpp"


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- U N I Q U E  P T R --------------------------------------------------

	/* unique pointer */
	template <typename ___type>
	class unique_ptr final {


		// -- friends ---------------------------------------------------------

		/* unique pointer as friend */
		template <typename>
		friend class unique_ptr;

		/* make unique as friend */
		template <typename ___tp, typename... ___params>
		friend auto make_unique(___params&&...) -> cs::unique_ptr<___tp>;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = cs::unique_ptr<___type>;


		public:

			// -- public types ------------------------------------------------

			/* value type */
			using value_type = ___type;

			/* pointer type */
			using pointer = value_type*;

			/* const pointer type */
			using const_pointer = const value_type*;

			/* reference type */
			using reference = value_type&;

			/* const reference type */
			using const_reference = const value_type&;


		private:

			// -- private members ---------------------------------------------

			/* data */
			pointer _data;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			unique_ptr(void) noexcept
			: _data{nullptr} {
			}

			/* nullptr constructor */
			unique_ptr(decltype(nullptr)) noexcept
			: _data{nullptr} {
			}

			/* deleted copy constructor */
			unique_ptr(const ___self&) = delete;

			/* move constructor */
			unique_ptr(___self&& ___ot) noexcept
			: _data{___ot._data} {
				___ot._data = nullptr;
			}

			/* move constructor (derived) */
			template <typename ___tp>
			unique_ptr(cs::unique_ptr<___tp>&& ___ot) noexcept
			: _data{___ot._data} {
				___ot._data = nullptr;

				// check for derived type
				static_assert(cs::is_base_of<___type, ___tp>,
						"unique_ptr: invalid pointer type");
			}

			/* destructor */
			~unique_ptr(void) noexcept {
				_free();
			}


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const ___self&) -> ___self& = delete;

			/* move assignment operator */
			auto operator=(___self&& ___ot) noexcept -> ___self& {

				if (this == &___ot)
					return *this;

				_free();

				_data = ___ot._data;
				___ot._data = nullptr;

				return *this;
			}

			/* move assignment operator (derived) */
			template <typename ___tp>
			auto operator=(cs::unique_ptr<___tp>&& ___ot) noexcept -> ___self& {

				if (this == &___ot)
					return *this;

				_free();

				_data = ___ot._data;
				___ot._data = nullptr;

				// check for derived type
				static_assert(cs::is_base_of<___type, ___tp>,
						"unique_ptr: invalid pointer type");

				return *this;
			}


			// -- public accessors --------------------------------------------

			/* dereference */
			auto operator*(void) noexcept -> reference {
				return *_data;
			}

			/* const dereference */
			auto operator*(void) const noexcept -> const_reference {
				return *_data;
			}

			/* arrow */
			auto operator->(void) noexcept -> pointer {
				return _data;
			}

			/* const arrow */
			auto operator->(void) const noexcept -> const_pointer {
				return _data;
			}

			/* get */
			auto get(void) noexcept -> pointer {
				return _data;
			}

			/* const get */
			auto get(void) const noexcept -> const_pointer {
				return _data;
			}


			// -- public conversion operators ---------------------------------

			/* bool conversion operator */
			explicit operator bool(void) const noexcept {
				return _data != nullptr;
			}


			// -- public comparison operators ---------------------------------

			/* nullptr equality operator */
			auto operator==(decltype(nullptr)) const noexcept -> bool {
				return _data == nullptr;
			}

			/* nullptr inequality operator */
			auto operator!=(decltype(nullptr)) const noexcept -> bool {
				return _data != nullptr;
			}

			/* equality operator */
			template <typename ___tp>
			auto operator==(const cs::unique_ptr<___tp>& ___ot) const noexcept -> bool {
				return static_cast<void*>(_data) == static_cast<void*>(___ot._data);
			}

			/* inequality operator */
			template <typename ___tp>
			auto operator!=(const cs::unique_ptr<___tp>& ___ot) const noexcept -> bool {
				return static_cast<void*>(_data) != static_cast<void*>(___ot._data);
			}

			/* less than operator */
			template <typename ___tp>
			auto operator<(const cs::unique_ptr<___tp>& ___ot) const noexcept -> bool {
				return static_cast<void*>(_data) < static_cast<void*>(___ot._data);
			}

			/* greater than operator */
			template <typename ___tp>
			auto operator>(const cs::unique_ptr<___tp>& ___ot) const noexcept -> bool {
				return static_cast<void*>(_data) > static_cast<void*>(___ot._data);
			}

			/* less than or equal operator */
			template <typename ___tp>
			auto operator<=(const cs::unique_ptr<___tp>& ___ot) const noexcept -> bool {
				return static_cast<void*>(_data) <= static_cast<void*>(___ot._data);
			}

			/* greater than or equal operator */
			template <typename ___tp>
			auto operator>=(const cs::unique_ptr<___tp>& ___ot) const noexcept -> bool {
				return static_cast<void*>(_data) >= static_cast<void*>(___ot._data);
			}


		private:

			// -- private lifecycle -------------------------------------------

			/* pointer constructor */
			template <typename ___tp>
			unique_ptr(___tp* ___ptr) noexcept
			: _data{___ptr} {
			}


			// -- private methods ---------------------------------------------

			/* free */
			auto _free(void) noexcept -> void {

				// check allocated memory
				if (_data == nullptr)
					return;

				// destruct object
				_data->~___type();

				// free memory
				cs::free(_data);
			}



	}; // class unique_ptr


	// -- non-member functions ------------------------------------------------

	/* make unique */
	template <typename ___type, typename... ___params>
	auto make_unique(___params&&... ___args) -> cs::unique_ptr<___type> {

		// allocate memory
		auto* ___ptr = cs::malloc<___type>();

		// check for allocation failure
		if (___ptr == nullptr)
			throw cs::runtime_error{"cs::make_unique: failed to allocate memory"};

		// construct object
		new (static_cast<void*>(___ptr)) ___type{cs::forward<___params>(___args)...};

		// return unique pointer
		return cs::unique_ptr<___type>{___ptr};
	}

} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_MEMORY_UNIQUE_PTR_HPP___
