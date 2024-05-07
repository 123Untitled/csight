#ifndef CS_LIST_HEADER
#define CS_LIST_HEADER

#include "cs/config.hpp"
#include "cs/memory/malloc.hpp"
#include "cs/type_traits/forward.hpp"
#include "cs/type_traits/conditional.hpp"
#include "cs/diagnostics/exception_guard.hpp"
#include "cs/memory/lifecycle.hpp"

#include <iostream>


#if ___cs_requirements


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- L I S T -------------------------------------------------------------

	template <typename ___type>
	class list final {

		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = list<___type>;


			// -- forward declarations ----------------------------------------

			/* iterator */
			template <bool>
			class ___iterator;

			/* node type */
			struct ___node;

			/* node pointer type */
			using ___node_ptr = ___node*;


			/* node type */
			struct ___node final {


				// -- members -------------------------------------------------

				/* next */
				___node_ptr _next;

				/* data */
				___type     _data;


				// -- lifecycle -----------------------------------------------

				/* default constructor */
				___node(void) noexcept
				: _next{nullptr}, _data{} {
				}

				/* parameter constructor */
				template <typename... ___params>
				___node(___params&&... ___args) noexcept(false /* need to check noexcept */)
				: _next{nullptr}, _data{cs::forward<___params>(___args)...} {
				}

				/* deleted copy constructor */
				___node(const ___node&) = delete;

				/* deleted move constructor */
				___node(___node&&) = delete;

				/* destructor */
				~___node(void) noexcept = default;


				// -- assignment operators ------------------------------------

				/* deleted copy assignment operator */
				auto operator=(const ___node&) noexcept -> ___node& = delete;

				/* deleted move assignment operator */
				auto operator=(___node&&) noexcept -> ___node& = delete;

			}; // struct ___node


			/* lifecycle type */
			using ___lifecycle = cs::lifecycle<___node>;


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

			/* iterator type */
			using iterator = ___iterator<false>;

			/* const iterator type */
			using const_iterator = ___iterator<true>;



		private:

			// -- private members ---------------------------------------------

			/* head node */
			___node_ptr _head;

			/* tail node */
			___node** _tail;

			/* size */
			size_type _size;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			list(void) noexcept
			: _head{nullptr}, _tail{&_head}, _size{0U} {
			}

			/* copy constructor */
			list(const ___self& ___ot)
			: _head{nullptr}, _tail{&_head}, _size{0U} {

				auto ___tmp = ___ot._head;

				while (___tmp != nullptr) {
					push_back(___tmp->_data);
					___tmp = ___tmp->_next;
				}
			}

			/* move constructor */
			list(___self&& ___ot) noexcept
			: _head{___ot._head}, _tail{___ot._tail}, _size{___ot._size} {
				___ot._init();
			}

			/* destructor */
			~list(void) noexcept {
				_clear();
			}


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const ___self& ___ot) -> ___self& {

				if (this == &___ot)
					return *this;

				_clear();
				_init();

				auto ___tmp = ___ot._head;

				while (___tmp != nullptr) {
					push_back(___tmp->_data);
					___tmp = ___tmp->_next;
				}

				return *this;
			}

			/* move assignment operator */
			auto operator=(___self&& ___ot) noexcept -> ___self& {

				if (this == &___ot)
					return *this;

				_clear();
				_copy_members(___ot);
				___ot._init();

				return *this;
			}


			// -- public iterators --------------------------------------------

			/* begin */
			auto begin(void) noexcept -> iterator {
				return iterator{_head};
			}

			/* const begin */
			auto begin(void) const noexcept -> const_iterator {
				return const_iterator{_head};
			}

			/* cbegin */
			auto cbegin(void) const noexcept -> const_iterator {
				return const_iterator{_head};
			}

			/* end */
			auto end(void) noexcept -> iterator {
				return iterator{nullptr};
			}

			/* const end */
			auto end(void) const noexcept -> const_iterator {
				return const_iterator{nullptr};
			}

			/* cend */
			auto cend(void) const noexcept -> const_iterator {
				return const_iterator{nullptr};
			}


			// -- public accessors --------------------------------------------

			/* size */
			auto size(void) const noexcept -> size_type {
				return _size;
			}

			/* empty */
			auto empty(void) const noexcept -> bool {
				return _size == 0U;
			}

			/* front */
			auto front(void) noexcept -> reference {
				return _head->_data;
			}

			/* const front */
			auto front(void) const noexcept -> const_reference {
				return _head->_data;
			}

			/* back */
			auto back(void) noexcept -> reference {
				return reinterpret_cast<___node_ptr>(_tail)->_data;
			}

			/* const back */
			auto back(void) const noexcept -> const_reference {
				return reinterpret_cast<___node_ptr>(_tail)->_data;
			}


			// -- public modifiers --------------------------------------------

			/* clear */
			auto clear(void) noexcept -> void {
				_clear();
				_init();
			}

			/* push back */
			auto push_back(const_reference ___vl) noexcept -> void {

				// allocate memory for new node
				auto ___tmp = cs::malloc<___node>();

				// guard memory allocation
				cs::exception_guard ___guard{[&___tmp] {
					cs::free(___tmp);
				}};

				// construct new node
				___lifecycle::construct(___tmp, ___vl);

				// complete memory allocation
				___guard.complete();

				// link new node
				*_tail = ___tmp;
				 _tail = &(*_tail)->_next;

				 // increment size
				++_size;
			}

			/* push back */
			auto push_back(value_type&& ___vl) noexcept -> void {

				// allocate memory for new node
				auto ___tmp = cs::malloc<___node>();

				// guard memory allocation
				cs::exception_guard ___guard{[&___tmp] {
					cs::free(___tmp);
				}};

				// construct new node
				___lifecycle::construct(___tmp, cs::move(___vl));

				// complete memory allocation
				___guard.complete();

				// link new node
				*_tail = ___tmp;
				 _tail = &(*_tail)->_next;

				 // increment size
				++_size;
			}

			/* emplace back */
			template <typename... ___params>
			auto emplace_back(___params&&... ___args) noexcept -> void {

				// allocate memory for new node
				auto ___tmp = cs::malloc<___node>();

				// guard memory allocation
				cs::exception_guard ___guard{[&___tmp] {
					cs::free(___tmp);
				}};

				// construct new node
				___lifecycle::construct(___tmp, cs::forward<___params>(___args)...);

				// complete memory allocation
				___guard.complete();

				// link new node
				*_tail = ___tmp;
				 _tail = &(*_tail)->_next;

				 // increment size
				++_size;
			}


			/* print */
			auto print(void) const noexcept -> void {
				auto ___tmp = _head;

				while (___tmp != nullptr) {
					std::cout << ___tmp->_data << " ";
					___tmp = ___tmp->_next;
				}

				std::cout << std::endl;
			}

		private:

			// -- private methods ---------------------------------------------

			/* init */
			auto _init(void) noexcept -> void {
				_head = nullptr;
				_tail = &_head;
				_size = 0U;
			}

			/* copy members */
			auto _copy_members(const ___self& ___ot) noexcept -> void {
				_head = ___ot._head;
				_tail = ___ot._tail;
				_size = ___ot._size;
			}

			/* clear */
			auto _clear(void) noexcept -> void {

				// loop over nodes
				while (_head != nullptr) {

					// save next node
					auto ___nx = _head->_next;

					// destroy current node
					___lifecycle::destroy(_head);

					// free current node
					cs::free(_head);

					// move to next node
					_head = ___nx;
				}
			}


	}; // class list


	// -- I T E R A T O R -----------------------------------------------------

	template <typename ___type>
	template <bool ___const>
	class list<___type>::___iterator final {


		// -- friends ---------------------------------------------------------

		/* list as friend */
		friend class list<___type>;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = ___iterator<___const>;

			/* reference type */
			using ___reference = cs::conditional<___const, const_reference, reference>;

			/* pointer type */
			using ___pointer = cs::conditional<___const, const_pointer, pointer>;


			// -- private members ---------------------------------------------

			/* node pointer */
			___node_ptr _node;


			// -- private lifecycle -------------------------------------------

			/* node constructor */
			___iterator(___node_ptr ___node) noexcept
			: _node{___node} {
			}


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			___iterator(void) noexcept
			: _node{nullptr} {
			}

			/* copy constructor */
			___iterator(const ___self&) noexcept = default;

			/* copy constructor */
			___iterator(const ___iterator<!___const>& ___ot) noexcept
			: _node{___ot._node} {
				static_assert(___const, "iterator: cannot convert from const_iterator to iterator.");
			}

			/* move constructor */
			___iterator(___self&&) noexcept = default;

			/* move constructor */
			___iterator(___iterator<!___const>&& ___ot) noexcept
			: _node{___ot._node} {
				static_assert(___const, "iterator: cannot convert from const_iterator to iterator.");
			}

			/* destructor */
			~___iterator(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const ___self&) noexcept -> ___self& = default;

			/* copy assignment operator */
			auto operator=(const ___iterator<!___const>& ___ot) noexcept -> ___self& {
				static_assert(___const, "iterator: cannot convert from const_iterator to iterator.");
				_node = ___ot._node;
				return *this;
			}

			/* move assignment operator */
			auto operator=(___self&&) noexcept -> ___self& = default;

			/* move assignment operator */
			auto operator=(___iterator<!___const>&& ___ot) noexcept -> ___self& {
				static_assert(___const, "iterator: cannot convert from const_iterator to iterator.");
				_node = ___ot._node;
				return *this;
			}


			// -- public accessors --------------------------------------------

			/* dereference */
			auto operator*(void) noexcept -> ___reference {
				return _node->_data;
			}

			/* arrow */
			auto operator->(void) noexcept -> ___pointer {
				return &_node->_data;
			}


			// -- public modifiers --------------------------------------------

			/* pre-increment */
			auto operator++(void) noexcept -> ___self& {
				_node = _node->_next;
				return *this;
			}

			/* post-increment */
			auto operator++(int) noexcept -> ___self {
				auto ___tmp{_node};
				_node = _node->_next;
				return ___tmp;
			}


			// -- public comparaison operators --------------------------------

			/* equality */
			template <bool ___c>
			auto operator==(const ___iterator<___c>& ___ot) const noexcept -> bool {
				return _node == ___ot._node;
			}

			/* inequality */
			template <bool ___c>
			auto operator!=(const ___iterator<___c>& ___ot) const noexcept -> bool {
				return _node != ___ot._node;
			}

			/* less than */
			template <bool ___c>
			auto operator<(const ___iterator<___c>& ___ot) const noexcept -> bool {
				return _node < ___ot._node;
			}

			/* greater than */
			template <bool ___c>
			auto operator>(const ___iterator<___c>& ___ot) const noexcept -> bool {
				return _node > ___ot._node;
			}

			/* less than or equal */
			template <bool ___c>
			auto operator<=(const ___iterator<___c>& ___ot) const noexcept -> bool {
				return _node <= ___ot._node;
			}

			/* greater than or equal */
			template <bool ___c>
			auto operator>=(const ___iterator<___c>& ___ot) const noexcept -> bool {
				return _node >= ___ot._node;
			}


	}; // class ___iterator

} // namespace cs

#endif // ___cs_requirements

#endif // CS_LIST_HEADER
