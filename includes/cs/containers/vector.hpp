#ifndef CS_VECTOR_HEADER
#define CS_VECTOR_HEADER

#include "cs/config.hpp"
#include "cs/memory/malloc.hpp"
#include "cs/memory/memcpy.hpp"
#include "cs/type_traits/is_trivially_copyable.hpp"
#include "cs/type_traits/is_trivially_destructible.hpp"
#include "cs/type_traits/forward.hpp"

// bad alloc
#include <new>



#if ___cs_requirements


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- A L L O C A T I O N -------------------------------------------------

	template <typename ___type>
	class vector final {


		// -- assertions ------------------------------------------------------

		/* trivially copyable assertion */
		static_assert(cs::is_trivially_copyable<___type>,
				"allocation: type must be trivially copyable");

		/* trivially destructible assertion */
		static_assert(cs::is_trivially_destructible<___type>,
				"allocation: type must be trivially destructible");


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = cs::vector<___type>;


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
			using size_type = decltype(sizeof(0));


		private:

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
			vector(void) noexcept
			: _data{nullptr}, _size{0U}, _capacity{0U} {
			}

			/* capacity constructor */
			vector(const size_type ___sz)
			: _data{cs::malloc<value_type>(___sz)}, _size{0U}, _capacity{___sz} {

				if (_data == nullptr)
					throw std::bad_alloc{};
			}

			/* copy constructor */
			vector(const ___self& ___ot)
			: _data{nullptr}, _size{___ot._size}, _capacity{___ot._size} {

				if (_size == 0U)
					return;

				_data = cs::malloc<value_type>(_size);

				if (_data == nullptr)
					throw std::bad_alloc{};

				cs::memcpy(_data, ___ot._data, _size);
			}

			/* move constructor */
			vector(___self&& ___ot) noexcept
			: _data{___ot._data}, _size{___ot._size}, _capacity{___ot._capacity} {
				___ot._init();
			}

			/* destructor */
			~vector(void) noexcept {
				_deallocate();
			}


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const ___self& ___ot) -> ___self& {

				if (this == &___ot)
					return *this;

				if (_capacity < ___ot._size)
					_reserve(___ot._size);

				_size     = ___ot._size;
				_capacity = ___ot._size;

				cs::memcpy(_data, ___ot._data, _size);

				return *this;
			}

			/* move assignment operator */
			auto operator=(___self&& ___ot) noexcept -> ___self& {

				if (this == &___ot)
					return *this;

				_deallocate();

				_copy_members(___ot);

				___ot._init();

				return *this;
			}


			// -- public accessors --------------------------------------------

			/* data */
			auto data(void) noexcept -> pointer {
				return _data;
			}

			/* const data */
			auto data(void) const noexcept -> const_pointer {
				return _data;
			}

			/* size */
			auto size(void) const noexcept -> size_type {
				return _size;
			}

			/* capacity */
			auto capacity(void) const noexcept -> size_type {
				return _capacity;
			}

			/* empty */
			auto empty(void) const noexcept -> bool {
				return _size == 0U;
			}


			// -- public element access ---------------------------------------

			/* operator [] */
			auto operator[](const size_type ___i) noexcept -> reference {
				return _data[___i];
			}

			/* operator [] */
			auto operator[](const size_type ___i) const noexcept -> const_reference {
				return _data[___i];
			}



			// -- public modifiers --------------------------------------------

			/* reserve */
			auto reserve(const size_type ___cp) -> void {

				if (___cp <= _capacity)
					return;

				_reserve(___cp);
			}

			/* resize */
			auto resize(const size_type ___sz) -> void {

				if (___sz > _capacity)
					_reserve(___sz);

				_size = ___sz;
			}

			/* clear */
			auto clear(void) noexcept -> void {
				_size = 0U;
			}

			/* push back */
			auto push_back(const value_type& ___val) -> void {

				if (_size == _capacity)
					_reserve(_expand());

				new (_data + _size) value_type{___val};

				++_size;
			}

			/* push back */
			auto push_back(value_type&& ___val) -> void {

				if (_size == _capacity)
					_reserve(_expand());

				new (_data + _size) value_type{static_cast<value_type&&>(___val)};

				++_size;
			}

			/* pop back */
			auto pop_back(void) noexcept -> void {
				--_size;
			}

			/* emplace back */
			template <typename... ___params>
			auto emplace_back(___params&&... ___args) -> void {

				if (_size == _capacity)
					_reserve(_expand());

				new (_data + _size) value_type{cs::forward<___params>(___args)...};

				++_size;
			}




		private:

			// -- private methods ---------------------------------------------

			/* init */
			auto _init(void) noexcept -> void {
				_data     = nullptr;
				_size     = 0U;
				_capacity = 0U;
			}

			/* copy members */
			auto _copy_members(___self& ___ot) noexcept -> void {
				_data     = ___ot._data;
				_size     = ___ot._size;
				_capacity = ___ot._capacity;
			}

			/* available capacity */
			auto _available_capacity(void) const noexcept -> size_type {
				return _capacity - _size;
			}

			/* expand */
			auto _expand(void) const noexcept -> size_type {
				// check overflow
				return _capacity ? _capacity << 1U : 1U;
			}

			/* try expand */
			auto _try_expand(const size_type ___cp) const noexcept -> size_type {
				// check overflow
				const auto ___ncp = _capacity << 1U;
				return ___ncp < ___cp ? ___cp : ___ncp;
			}

			/* deallocate */
			auto _deallocate(void) noexcept -> void {
				if (_data == nullptr)
					return;
				cs::free(_data);
			}

			/* reserve */
			auto _reserve(const size_type ___cp) -> void {

				auto ___ptr = cs::realloc<value_type>(_data, ___cp);

				if (___ptr == nullptr)
					throw std::bad_alloc{};

				_data     = ___ptr;
				_capacity = ___cp;
			}


	}; // class allocation

} // namespace cs

#endif // ___cs_requirements

#endif // CS_VECTOR_HEADER
