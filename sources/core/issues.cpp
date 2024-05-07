#include "cs/core/issues.hpp"
#include "cs/memory/lifecycle.hpp"
#include "cs/memory/malloc.hpp"


// -- public lifecycle --------------------------------------------------------

/* default constructor */
cs::core::issues::issues(void) noexcept
: _head{nullptr}, _tail{&_head} {
}

/* move constructor */
cs::core::issues::issues(___self&& ___ot) noexcept
: _head{___ot._head}, _tail{___ot._tail} {
	// zero other
	___ot._init();
}

/* destructor */
cs::core::issues::~issues(void) noexcept {
	_clear();
}


// -- public assignment operators ---------------------------------------------

/* move assignment operator */
auto cs::core::issues::operator=(___self&& ___ot) noexcept -> ___self& {

	// check self assignment
	if (this == &___ot)
		return *this;

	// clear self
	_clear();

	// move members
	_copy(___ot);

	// zero other
	___ot._init();

	// return self
	return *this;
}


// -- public accessors --------------------------------------------------------

/* size */
auto cs::core::issues::size(void) const noexcept -> size_type {
	return _size;
}

/* empty */
auto cs::core::issues::empty(void) const noexcept -> bool {
	return _size == 0U;
}


// -- public modifiers --------------------------------------------------------

/* push */
auto cs::core::issues::push(cs::core::issue&& ___iss) -> void {

	// allocate new node
	auto* ___new = cs::malloc<___node>();

	// construct node
	cs::lifecycle<___node>::construct(___new, cs::move(___iss));

	// link node
	*_tail = ___new;
	_tail = &(___new->_next);

	// increment size
	++_size;
}


// -- private methods ---------------------------------------------------------

/* init */
auto cs::core::issues::_init(void) noexcept -> void {
	_head = nullptr;
	_tail = &_head;
	_size = 0U;
}

/* copy */
auto cs::core::issues::_copy(const ___self& ___ot) noexcept -> void {
	_head = ___ot._head;
	_tail = ___ot._tail;
	_size = ___ot._size;
}

/* clear */
auto cs::core::issues::_clear(void) noexcept -> void {

	___node* it = _head;

	// loop over issues
	while (it != nullptr) {

		// save next
		auto* ___nx = it->_next;

		// destruct node
		cs::lifecycle<___node>::destroy(it);
		// free node
		cs::free(it);

		// next
		it = ___nx;
	}
}


// -- public lifecycle --------------------------------------------------------

/* members constructor */
cs::core::issues::___node::___node(cs::core::issue&& ___iss) noexcept
: _issue{cs::move(___iss)}, _next{nullptr} {
}
