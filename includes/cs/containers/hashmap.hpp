#ifndef ___CS_CONTAINERS_HASHMAP_HPP___
#define ___CS_CONTAINERS_HASHMAP_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/types.hpp"
#include "cs/memory/malloc.hpp"
#include "cs/type_traits/move.hpp"
#include "cs/type_traits/forward.hpp"
#include "cs/type_traits/declval.hpp"
#include "cs/diagnostics/exception_guard.hpp"
#include "cs/memory/lifecycle.hpp"
#include "cs/string.hpp"
#include "cs/containers/pair.hpp"


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
	inline auto hash_(const std::string& ___vl) noexcept -> cs::size_t {
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

			/* value type */
			using value_type = cs::pair<const ___key, ___value>;

			/* size type */
			using size_type = cs::size_t;


		private:

			// -- private types -----------------------------------------------

			/* wrapper */
			struct wrapper final {

				// -- members -------------------------------------------------

				/* hash */
				const size_type _hash;

				/* value */
				value_type _pair;


				// -- lifecycle -----------------------------------------------

				/* emplace constructor */
				template <typename... ___params>
				wrapper(size_type ___hs, ___params&&... ___args)
				: _hash{___hs},
				  _pair{cs::forward<___params>(___args)..., mapped_type{}} {
				}

				/* non-copyable */
				___cs_not_copyable(wrapper);

				/* non-moveable */
				___cs_not_moveable(wrapper);

				/* destructor */
				~wrapper(void) noexcept = default;

			}; // struct wrapper



			// -- private data ------------------------------------------------

			/* data */
			wrapper** _data;

			/* size */
			size_type _size;

			/* capacity */
			size_type _capacity;


			// -- private constants -------------------------------------------

			/* default size */
			enum : size_type {
				DEFAULT_SIZE = 1U
			};

			/* load factor */
			static constexpr float _load_factor = 0.75f;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			hashmap(void)
			: _data{cs::calloc<wrapper*>(DEFAULT_SIZE)},
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
			//template <typename... ___params>
			//auto insert(key_type&& ___ky, ___params&&... ___args) -> void {
			auto operator[](key_type&& ___ky) -> mapped_type& {

				// check if size is full
				if (_need_resize() == true)
					_resize();

				// hash key
				const auto hash = cs::hash_(___ky);
				auto index = hash % _capacity;

				// find empty slot
				for (size_type q = 1U; _data[index] != nullptr; ++q) {

					//std::cout << "collision: " << index << std::endl;

					// check hash and file exists
					if (_data[index]->_hash == hash
					 && cs::get<const key_type>(_data[index]->_pair) == ___ky)
						return cs::get<mapped_type>(_data[index]->_pair);

					// quadratic probing
					index = (index + (q * q)) % _capacity;
				}

				// allocate new node
				_data[index] = cs::malloc<wrapper>();

				if constexpr (std::is_nothrow_constructible<wrapper, size_type, key_type>::value) {

					// construct node
					cs::lifecycle<wrapper>::construct(_data[index],
							hash,
							cs::move(___ky)
					);
				}
				else {
					cs::exception_guard guard{[&] {
						cs::free(_data[index]);
						_data[index] = nullptr;
					}};

					// construct node
					cs::lifecycle<wrapper>::construct(_data[index],
							hash,
							cs::move(___ky)
					);

					// release guard
					guard.complete();
				}

				// increment size
				++_size;

				// return reference
				return cs::get<mapped_type>(_data[index]->_pair);
			}

			/* for each */
			template <typename ___fn, typename... ___params>
			auto for_each(___fn&& ___fu, ___params&&... ___args)

				// check noexcept function
				noexcept(noexcept(___fu(cs::declval<value_type&>(),
										cs::declval<___params>()...))) -> void {

				// loop over data
				for (size_type i = 0U; i < _capacity; ++i) {
					// call if not null
					if (_data[i] != nullptr)
						___fu(_data[i]->_pair,
							   cs::forward<___params>(___args)...);
				}
			}

			/* const for each */
			template <typename ___fn, typename... ___params>
			auto for_each(___fn&& ___fu, ___params&&... ___args) const

				// check noexcept function
				noexcept(noexcept(___fu(
								cs::declval<const value_type&>(),
								cs::declval<___params>()...))) -> void {

					// loop over data
					for (size_type i = 0U; i < _capacity; ++i) {
						// call if not null
						if (_data[i] != nullptr)
							___fu(_data[i]->_pair,
								  cs::forward<___params>(___args)...);
					}

			}

			/* contains */
			auto contains(const key_type& ___ky) const noexcept -> bool {

				// hash key
				const auto hash = cs::hash_(___ky);
				auto index = hash % _capacity;

				// find empty slot
				for (size_type q = 1U; _data[index] != nullptr; ++q) {

					// check hash and file exists
					if (_data[index]->_hash == hash
					 && cs::get<const key_type>(_data[index]->_pair) == ___ky)
						return true;

					// quadratic probing
					index = (index + (q * q)) % _capacity;
				}

				return false;
			}


		private:

			// -- private methods ---------------------------------------------

			/* resize */
			auto _resize(void) {

				// compute new capacity
				const auto ___cp = (_capacity << 1U);// + 1U; // odd

				// allocate zeroed memory
				auto ___new = cs::calloc<wrapper*>(___cp);

				// re-hash map
				for (size_type i = 0U; i < _capacity; ++i) {

					if (_data[i] == nullptr)
						continue;

					// hash key
					auto idx = _data[i]->_hash % ___cp;

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

				wrapper**  it = _data;
				wrapper** end = it + _capacity;

				// free data
				for (;it != end; ++it) {

					// continue if null
					if (*it == nullptr)
						continue;

					// destruct node
					cs::lifecycle<wrapper>::destroy(*it);
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
