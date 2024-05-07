#ifndef ___CS_CONTAINERS_HASHMAP_HPP___
#define ___CS_CONTAINERS_HASHMAP_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/types.hpp"
#include "cs/type_traits/move.hpp"


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- H A S H M A P -------------------------------------------------------

	template <typename ___key, typename ___value>
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
			struct node final {


				// -- members -------------------------------------------------

				/* hash */
				const size_type _hash;

				/* key */
				key_type _key;

				/* value */
				mapped_type _value;


				// -- lifecycle -----------------------------------------------

				/* default constructor */
				node(void) noexcept /* noexcept default constructible */ = default;

				/* members constructor */
				node(const size_type hash, const key_type& key, const mapped_type& value)
				: _hash{hash}, _key{key}, _value{value} {
				}

				/* members move constructor */
				node(const size_type hash, key_type&& key, mapped_type&& value)
				: _hash{hash}, _key{cs::move(key)}, _value{cs::move(value)} {
				}

				/* deleted copy constructor */
				node(const node&) = delete;

				/* deleted move constructor */
				node(node&&) = delete;


				// -- assignment operators ------------------------------------

				/* deleted copy assignment operator */
				auto operator=(const node&) -> node& = delete;

				/* deleted move assignment operator */
				auto operator=(node&&) -> node& = delete;

			}; // struct node


			// -- private types -----------------------------------------------

			/* pointer type */
			using pointer = node*;


			// -- private data ------------------------------------------------

			/* data */
			pointer* _data;

			/* size */
			size_type _size;

			/* capacity */
			size_type _capacity;


	}; // class hashmap

} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_CONTAINERS_HASHMAP_HPP___
