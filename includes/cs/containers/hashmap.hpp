#ifndef ___CS_CONTAINERS_HASHMAP_HPP___
#define ___CS_CONTAINERS_HASHMAP_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/types.hpp"
#include "cs/memory/malloc.hpp"
#include "cs/type_traits/move.hpp"
#include "cs/type_traits/forward.hpp"
#include "cs/memory/lifecycle.hpp"


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {

	/* hash */
	template <typename ___type>
	auto hash(const ___type& ___vl) noexcept -> cs::size_t {
		// jenkins one at a time hash
		cs::size_t hash = 0U;
		for (const auto& it : ___vl) {
			hash += it;
			hash += hash << 10;
			hash ^= hash >> 6;
		}
		hash += hash << 3;
		hash ^= hash >> 11;
		hash += hash << 15;
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
			: _data{cs::calloc<pointer>(DEFAULT_SIZE)},
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
				const auto hash = cs::hash(___ky);
				auto index = hash % _capacity;
//
//	// find empty slot
//	for (size_type q = 1U; _data[index] != nullptr; ++q) {
//
//		// check hash and file exists
//		if (_data[index]->hash() == hash
//		 && _data[index]->entry().file() == file) {
//			_data[index]->entry()._issues.push(cs::move(___iss));
//			// clear file (move semantic responsibility)
//			file.clear();
//			return; }
//
//		// quadratic probing, todo: maybe check infinite loop
//		index = (index + (q * q)) % _capacity;
//
//#ifdef CS_CORE_MAP_DEBUG
//		++_nprobe;
//#endif
//	}
//
//	// allocate new node
//	_data[index] = cs::malloc<___node>();
//
//	// construct node
//	cs::lifecycle<___node>::construct(_data[index],
//			hash,
//			cs::move(file),
//			cs::move(___iss));
//
//	// increment size
//	++_size;
//}
//
//				return true;
			}


		private:

			// -- private methods ---------------------------------------------

			/* resize */
			auto _resize(void) {
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
