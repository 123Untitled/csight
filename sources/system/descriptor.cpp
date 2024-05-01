#include "cs/system/descriptor.hpp"
#include <unistd.h>


// -- public lifecycle --------------------------------------------------------

/* default constructor */
cs::descriptor::descriptor(void) noexcept
: _desc{INVALID_DESCRIPTOR} {
}

/* descriptor constructor */
cs::descriptor::descriptor(const int ___desc) noexcept
: _desc{___desc} {
}

/* move constructor */
cs::descriptor::descriptor(___self&& ___ot) noexcept
: _desc{___ot._desc} {
	___ot._desc = INVALID_DESCRIPTOR;
}

/* destructor */
cs::descriptor::~descriptor(void) noexcept {
	_close();
}


// -- public assignment operators ---------------------------------------------

/* move assignment operator */
auto cs::descriptor::operator=(___self&& ___ot) noexcept -> ___self& {

	// check for self assignment
	if (this == &___ot)
		return *this;

	// close current descriptor
	_close();

	// get ownership
	_desc = ___ot._desc;
	___ot._desc = INVALID_DESCRIPTOR;

	// return self-reference
	return *this;
}

/* int assignment operator */
auto cs::descriptor::operator=(const int ___desc) noexcept -> ___self& {

	// close current descriptor
	_close();

	// assign new descriptor
	_desc = ___desc;

	// return self-reference
	return *this;
}


// -- public accessors --------------------------------------------------------

/* handle */
auto cs::descriptor::handle(void) const noexcept -> int {
	return _desc;
}


// -- public conversion operators ---------------------------------------------

/* int conversion operator */
cs::descriptor::operator int(void) const noexcept {
	return _desc;
}

/* bool conversion operator */
cs::descriptor::operator bool(void) const noexcept {
	return _desc != INVALID_DESCRIPTOR;
}


// -- public operators --------------------------------------------------------

/* not operator */
auto cs::descriptor::operator!(void) const noexcept -> bool {
	return _desc == INVALID_DESCRIPTOR;
}


// -- public modifiers --------------------------------------------------------

/* close */
auto cs::descriptor::close(void) noexcept -> void {
	_close();
	_desc = INVALID_DESCRIPTOR;
}


// -- private methods ---------------------------------------------------------

/* close */
auto cs::descriptor::_close(void) noexcept -> void {
	if (_desc == INVALID_DESCRIPTOR)
		return;
	::close(_desc);
}
