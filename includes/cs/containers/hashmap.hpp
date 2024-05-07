#ifndef ___CS_CONTAINERS_HASHMAP_HPP___
#define ___CS_CONTAINERS_HASHMAP_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/types.hpp"
#include "cs/memory/malloc.hpp"
#include "cs/type_traits/move.hpp"
#include "cs/type_traits/forward.hpp"
#include "cs/type_traits/declval.hpp"
#include "cs/memory/lifecycle.hpp"
#include "cs/string.hpp"


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {

	/* hash */
	inline auto hash_(const cs::string& ___vl) noexcept -> cs::size_t {
		// jenkins one at a time hash
		cs::size_t hash = 0U;

		auto it = ___vl.data();
		auto end = it + ___vl.size();


		for (; it != end; ++it) {
			hash += static_cast<unsigned char>(*it);
			hash += hash << 10U;
			hash ^= hash >> 6U;
		}
		hash += hash << 3U;
		hash ^= hash >> 11U;
		hash += hash << 15U;
		return hash;
	}

	/* hash */
	inline auto hash_(const cs::string_view& ___vl) noexcept -> cs::size_t {
		// jenkins one at a time hash
		cs::size_t hash = 0U;

		auto it = ___vl.data();
		auto end = it + ___vl.size();


		for (; it != end; ++it) {
			hash += static_cast<unsigned char>(*it);
			hash += hash << 10U;
			hash ^= hash >> 6U;
		}
		hash += hash << 3U;
		hash ^= hash >> 11U;
		hash += hash << 15U;
		return hash;
	}

	// -- H A S H M A P -------------------------------------------------------

	template <typename ___key,
			  typename ___value>
	class hashmap final {

		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = cs::hashmap<___key, ___value>;


		public:

			// -- public types ------------------------------------------------

			/* key type */
			using key_type = ___key;

			/* mapped type */
			using mapped_type = ___value;

			/* size type */
			using size_type = cs::size_t;


		private:

			// -- private structs ---------------------------------------------

			/* node */
			class ___node final {


				private:

					// -- private members -------------------------------------

					/* hash */
					const size_type _hash;

					/* key */
					key_type _key;

					/* value */
					mapped_type _value;


				public:

					// -- public lifecycle ------------------------------------

					/* default constructor */
					___node(void) noexcept /* noexcept default constructible */ = default;

					/* members constructor */
					template <typename... ___params>
					___node(const size_type ___hs, const key_type& ___ke, ___params&&... ___args)
					/* noexcept key move construct, value construct */
					: _hash{___hs}, _key{___ke}, _value{cs::forward<___params>(___args)...} {
					}

					/* members move constructor */
					template <typename... ___params>
					___node(const size_type ___hs, key_type&& ___ke, ___params&&... ___args)
					/* noexcept key move construct, value construct */
					: _hash{___hs}, _key{cs::move(___ke)}, _value{cs::forward<___params>(___args)...} {
					}

					/* deleted copy constructor */
					___node(const ___node&) = delete;

					/* deleted move constructor */
					___node(___node&&) = delete;

					/* destructor */
					~___node(void) noexcept = default;


					// -- public assignment operators -------------------------

					/* deleted copy assignment operator */
					auto operator=(const ___node&) -> ___node& = delete;

					/* deleted move assignment operator */
					auto operator=(___node&&) -> ___node& = delete;


					// -- public accessors ------------------------------------

					/* hash */
					auto hash(void) const noexcept -> size_type {
						return _hash;
					}

					/* key */
					auto key(void) const noexcept -> const key_type& {
						return _key;
					}

					/* value */
					auto value(void) noexcept -> mapped_type& {
						return _value;
					}

					/* const value */
					auto value(void) const noexcept -> const mapped_type& {
						return _value;
					}

			}; // class ___node


			// -- private types -----------------------------------------------

			/* pointer type */
			using pointer = ___node*;


			// -- private data ------------------------------------------------

			/* data */
			pointer* _data;

			/* size */
			size_type _size;

			/* capacity */
			size_type _capacity;


			// -- private constants -------------------------------------------

			/* default size */
			enum : size_type {
				DEFAULT_SIZE = 127U
			};

			/* load factor */
			static constexpr float _load_factor = 0.75f;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			hashmap(void)
			: _data{cs::calloc<___node*>(DEFAULT_SIZE)},
			  _size{0U},
			  _capacity{DEFAULT_SIZE} {
			}

			/* non-copyable */
			___cs_not_copyable(hashmap);

			/* non-moveable */
			___cs_not_moveable(hashmap);

			/* destructor */
			~hashmap(void) noexcept {
				_clear();
			}


			// -- public modifiers --------------------------------------------

			/* insert */
			template <typename... ___params>
			auto insert(const key_type& ___ky, ___params&&... ___args) -> void {

				// check if size is full
				if (_need_resize() == true)
					_resize();

				// hash key
				const auto hash = cs::hash_(___ky);
				auto index = hash % _capacity;

				// find empty slot
				for (size_type q = 1U; _data[index] != nullptr; ++q) {

					// check hash and file exists
					if (_data[index]->hash() == hash
					 && _data[index]->key() == ___ky) {
						// update value
						//_data[index]->value() = mapped_type{cs::forward<___params>(___args)...};
						return; }

					// quadratic probing
					index = (index + (q * q)) % _capacity;
				}

				// allocate new node
				_data[index] = cs::malloc<___node>();

				// construct node
				cs::lifecycle<___node>::construct(_data[index],
						hash, cs::forward<key_type>(___ky),
							  cs::forward<___params>(___args)...);

				// increment size
				++_size;
			}

			/* for each */
			template <typename ___fn, typename... ___params>
			auto for_each(___fn&& ___fu, ___params&&... ___args)

				// check noexcept function
				noexcept(noexcept(___fu(cs::declval<key_type&>(),
										cs::declval<mapped_type&>(),
										cs::declval<___params>()...))) -> void {

				// loop over data
				for (size_type i = 0U; i < _capacity; ++i) {
					// call if not null
					if (_data[i] != nullptr)
						___fu((_data[i])->key(),
							  (_data[i])->value(),
							   cs::forward<___params>(___args)...); }
			}

			/* const for each */
			//template <typename ___fn, typename... ___params>
			//auto for_each(___fn&& ___fu, ___params&&... ___args) const
			//
			//	// check noexcept function
			//	noexcept(noexcept(___fu(
			//					cs::declval<mapped_type>(),
			//					cs::declval<___params>()...))) -> void {
			//
			//		// loop over data
			//		for (size_type i = 0U; i < _capacity; ++i) {
			//			// call if not null
			//			if (_data[i] != nullptr)
			//				___fu((_data[i])->entry(),
			//						cs::forward<___params>(___args)...); }
			//	}


		private:

			// -- private methods ---------------------------------------------

			/* resize */
			auto _resize(void) {

				// compute new capacity
				const auto ___cp = (_capacity << 1U) + 1U; // odd

				// allocate zeroed memory
				auto ___new = cs::calloc<___node*>(___cp);

				// re-hash map
				for (size_type i = 0U; i < _capacity; ++i) {

					if (_data[i] == nullptr)
						continue;

					// hash key
					auto idx = _data[i]->hash() % ___cp;

					// find empty slot
					for (size_type q = 1U; ___new[idx] != nullptr; ++q) {
						// quadratic probing
						idx = (idx + (q * q)) % ___cp;
					}

					___new[idx] = _data[i];
				}

				// free old data
				cs::free(_data);

				// set new data
				_data = ___new;

				// set new capacity
				_capacity = ___cp;
			}

			/* compute load factor */
			auto _compute_load_factor(void) const noexcept -> float {
				return static_cast<float>(_size) / static_cast<float>(_capacity);
			}

			/* need resize */
			auto _need_resize(void) const noexcept -> bool {
				return _compute_load_factor() >= _load_factor;
			}

			
			auto _clear(void) noexcept {

				___node**  it = _data;
				___node** end = it + _capacity;

				// free data
				for (;it != end; ++it) {

					// continue if null
					if (*it == nullptr)
						continue;

					// destruct node
					cs::lifecycle<___node>::destroy(*it);
					// free node
					cs::free(*it);
				}

				// free data
				cs::free(_data);

			}




	}; // class hashmap

} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_CONTAINERS_HASHMAP_HPP___
