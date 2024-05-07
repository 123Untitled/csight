#ifndef ___CS_MAP_HPP___
#define ___CS_MAP_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/core/entry.hpp"

#define CS_CORE_MAP_DEBUG


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- C O R E -------------------------------------------------------------

	namespace core {


		// -- M A P -----------------------------------------------------------

		class map final {


			public:


				// -- private types -------------------------------------------

				/* self type */
				using ___self = cs::core::map;


			public:

				// -- public types --------------------------------------------

				/* value type */
				using value_type = cs::core::entry;

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


			private:

				// -- private structs -----------------------------------------

				/* node */
				class ___node final {


					private:

						// -- private members ---------------------------------

						/* hash */
						const size_type _hash;

						/* entry */
						value_type _entry;


					public:

						// -- public lifecycle --------------------------------

						/* members constructor */
						___node(const size_type hash, cs::string&& file, cs::core::issue&& issue)
						: _hash{hash}, _entry{cs::move(file), cs::move(issue)} {
						}

						/* non-copyable */
						___cs_not_copyable(___node);

						/* non-moveable */
						___cs_not_moveable(___node);

						/* destructor */
						~___node(void) noexcept = default;


						// -- public accessors --------------------------------

						/* hash */
						auto hash(void) const noexcept -> size_type {
							return _hash;
						}

						/* entry */
						auto entry(void) noexcept -> value_type& {
							return _entry;
						}

						/* const entry */
						auto entry(void) const noexcept -> const value_type& {
							return _entry;
						}

				}; // struct node


				// -- private members -----------------------------------------

				/* data */
				___node** _data;

				/* size */
				size_type _size;

				/* capacity */
				size_type _capacity;

				#ifdef CS_CORE_MAP_DEBUG
				/* nprobe */
				size_type _nprobe;

				/* nresize */
				size_type _nresize;
				#endif


				// -- private constants ---------------------------------------

				/* default size */
				enum : size_type {
					DEFAULT_SIZE = 127U
				};

				/* load factor */
				static constexpr float _load_factor = 0.75f;


			public:

				// -- public lifecycle ----------------------------------------

				/* default constructor */
				map(void);

				/* deleted copy constructor */
				map(const ___self&) = delete;

				/* move constructor */
				map(___self&& ___ot) noexcept;

				/* destructor */
				~map(void) noexcept;


				// -- public assignment operators -----------------------------

				/* deleted copy assignment operator */
				auto operator=(const ___self&) -> ___self& = delete;

				/* move assignment operator */
				auto operator=(___self&& ___ot) noexcept -> ___self&;


				// -- public methods ------------------------------------------

				/* for each */
				template <typename ___fn, typename... ___params>
				auto for_each(___fn&& ___fu, ___params&&... ___args)

					// check noexcept function
					noexcept(noexcept(___fu(
									cs::declval<reference>(),
									cs::declval<___params>()...))) -> void {

					// loop over data
					for (size_type i = 0U; i < _capacity; ++i) {
						// call if not null
						if (_data[i] != nullptr)
							___fu((_data[i])->entry(),
									cs::forward<___params>(___args)...); }
				}

				/* const for each */
				template <typename ___fn, typename... ___params>
				auto for_each(___fn&& ___fu, ___params&&... ___args) const

					// check noexcept function
					noexcept(noexcept(___fu(
									cs::declval<const_reference>(),
									cs::declval<___params>()...))) -> void {

					// loop over data
					for (size_type i = 0U; i < _capacity; ++i) {
						// call if not null
						if (_data[i] != nullptr)
							___fu((_data[i])->entry(),
									cs::forward<___params>(___args)...); }
				}


				// -- public accessors ----------------------------------------

				/* size */
				auto size(void) const noexcept -> size_type;

				/* capacity */
				auto capacity(void) const noexcept -> size_type;

				/* empty */
				auto empty(void) const noexcept -> bool;


				// -- public static accessors ---------------------------------

				/* load factor */
				static auto load_factor(void) noexcept -> float;

				/* default size */
				static auto default_size(void) noexcept -> size_type;



				// -- public modifiers ----------------------------------------

				/* insert */
				auto insert(cs::string&&, cs::core::issue&&) -> void;

				/* find */
				auto find(const cs::string&) const -> const_pointer;

				/* shrink to perfect */
				auto shrink_to_perfect(void) -> void;


				#ifdef CS_CORE_MAP_DEBUG
				/* debug */
				auto debug(void) noexcept -> void {

					std::cout << " initial size: " << DEFAULT_SIZE << std::endl;
					std::cout << "  load factor: " << _load_factor << std::endl;
					std::cout << "\n     capacity: " << _capacity << std::endl;
					std::cout << "         size: " << _size << std::endl;
					std::cout << "\n       nprobe: " << _nprobe << std::endl;
					std::cout << "      nresize: " << _nresize << std::endl;
					std::cout << "useless jumps: " << _capacity - _size << std::endl;
				}
				#endif


			private:


				// -- private methods -----------------------------------------

				/* init */
				auto _init(void) noexcept -> void;

				/* copy members */
				auto _copy_members(const ___self&) noexcept -> void;

				/* clear */
				auto _clear(void) noexcept -> void;

				/* resize */
				auto _resize(void) -> void;


				// -- private accessors -------------------------------------------

				/* compute load factor */
				auto _compute_load_factor(void) const noexcept -> float;

				/* need resize */
				auto _need_resize(void) const noexcept -> bool;


				// -- private static methods ----------------------------------

				/* hash */
				static auto _hash(const cs::string&) noexcept -> size_type;

		}; // class map

	} // namespace core

} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_MAP_HPP___
