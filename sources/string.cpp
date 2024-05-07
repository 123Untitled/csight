#include "cs/string.hpp"
#include "cs/memory/malloc.hpp"


// -- private static members --------------------------------------------------

/* empty string */
cs::string::value_type cs::string::_empty[1] = {'\0'};


// -- public lifecycle --------------------------------------------------------

/* default constructor */
cs::string::string(void) noexcept
: _data{_empty}, _size{0U}, _capacity{0U} {
}

/* move constructor */
cs::string::string(___self&& ___ot) noexcept
: _data{___ot._data}, _size{___ot._size}, _capacity{___ot._capacity} {
	___ot._init();
}

/* destructor */
cs::string::~string(void) noexcept {
	_deallocate();
}


// -- public assignment operators ---------------------------------------------

/* move assignment operator */
auto cs::string::operator=(___self&& ___ot) noexcept -> ___self& {

	if (this == &___ot)
		return *this;

	// deallocate memory
	_deallocate();

	// copy members
	_copy_members(___ot);

	// initialize other
	___ot._init();

	return *this;
}


// -- public accessors --------------------------------------------------------

/* data */
auto cs::string::data(void) noexcept -> pointer {
	return _data;
}

/* const data */
auto cs::string::data(void) const noexcept -> const_pointer {
	return _data;
}

/* size */
auto cs::string::size(void) const noexcept -> size_type {
	return _size;
}

/* capacity */
auto cs::string::capacity(void) const noexcept -> size_type {
	return _capacity;
}

/* empty */
auto cs::string::empty(void) const noexcept -> bool {
	return _size == 0U;
}


// -- public element access ---------------------------------------------------

/* operator[] */
auto cs::string::operator[](size_type ___pos) noexcept -> reference {
	return _data[___pos];
}

/* operator[] const */
auto cs::string::operator[](size_type ___pos) const noexcept -> const_reference {
	return _data[___pos];
}


// -- public modifiers --------------------------------------------------------

/* reserve */
auto cs::string::reserve(size_type ___cp) -> void {

	if (___cp <= _capacity)
		return;

	// reallocate memory
	_reserve(___cp);
}

/* resize */
auto cs::string::resize(size_type ___sz) -> void {

	if (___sz > _capacity)
		_reserve(___sz);

	_size = ___sz;

	// null terminate
	_null_terminate();
}

/* clear */
auto cs::string::clear(void) noexcept -> void {
	_size = 0U;
	_null_terminate();
}

/* push back */
auto cs::string::push_back(value_type ___ch) -> void {

	// check capacity
	if (_size == _capacity)
		_reserve(_expand());

	// copy construct
	*(_data + _size) = ___ch;

	// null terminate
	_null_terminate(++_size);
}

/* pop back */
auto cs::string::pop_back(const size_type ___cn) noexcept -> void {

	// reduce size
	_size < ___cn ? _size = 0U : _size -= ___cn;

	// null terminate
	_null_terminate(_size);
}


// -- private methods ---------------------------------------------------------

/* append multi (character) */
auto cs::string::_append_multi(size_type& ___of, const value_type ___ch) -> void {

	// copy character
	*(_data + ___of) = ___ch;
	// update offset
	++___of;
}

/* append multi (string) */
auto cs::string::_append_multi(size_type& ___of, const ___self& ___ot) -> void {

	// copy data
	cs::memcpy(_data + ___of, ___ot._data, ___ot._size);
	// update offset
	___of += ___ot._size;
}

/* append multi (string view) */
auto cs::string::_append_multi(size_type& ___of, const cs::string_view& ___ot) -> void {

	// copy data
	cs::memcpy(_data + ___of, ___ot.data(), ___ot.size());
	// update offset
	___of += ___ot.size();
}

/* size multi (character) */
auto cs::string::_size_multi(const value_type) noexcept -> size_type {
	return 1U;
}

/* size multi (string) */
auto cs::string::_size_multi(const ___self& ___ot) noexcept -> size_type {
	return ___ot._size;
}

/* size multi (string view) */
auto cs::string::_size_multi(const cs::string_view& ___ot) noexcept -> size_type {
	return ___ot.size();
}




/* init */
auto cs::string::_init(void) noexcept -> void {
	_data     = _empty;
	_size     = 0U;
	_capacity = 0U;
}

/* copy members */
auto cs::string::_copy_members(___self& ___ot) noexcept -> void {
	_data     = ___ot._data;
	_size     = ___ot._size;
	_capacity = ___ot._capacity;
}

/* allocate */
auto cs::string::_allocate(size_type ___sz) -> pointer {
	return cs::malloc<value_type>(___sz + 1U);
}

/* reallocate */
auto cs::string::_reallocate(size_type ___sz) -> pointer {
	return cs::realloc<value_type>(_data, ___sz + 1U);
}

/* null terminate */
auto cs::string::_null_terminate(void) noexcept -> void {
	*(_data + _size) = '\0';
}

/* null terminate */
auto cs::string::_null_terminate(size_type ___sz) noexcept -> void {
	*(_data + ___sz) = '\0';
}

/* available capacity */
auto cs::string::_available_capacity(void) const noexcept -> size_type {
	return _capacity - _size;
}

/* expand */
auto cs::string::_expand(void) const noexcept -> size_type {
	// check overflow
	return _capacity ? _capacity << 1U : 1U;
}

/* try expand */
auto cs::string::_try_expand(size_type ___cp) const noexcept -> size_type {
	// check overflow
	const auto ___ncp = _capacity << 1U;
	return ___ncp < ___cp ? ___cp : ___ncp;
}

/* deallocate */
auto cs::string::_deallocate(void) noexcept -> void {
	if (_data == _empty)
		return;
	cs::free(_data);
}

/* reserve */
auto cs::string::_reserve(size_type ___cp) -> void {

	pointer ___ptr;

	// reallocate new memory
	_data != _empty
		? ___ptr = _reallocate(___cp)
		: ___ptr = _allocate(___cp);

	// update members
	_data     = ___ptr;
	_capacity = ___cp;
}


// -- private comparison methods ----------------------------------------------

/* compare */
auto cs::string::_compare(const ___self& other) const noexcept -> int {

	// compare size
	if (_size != other._size)
		return _size < other._size ? -1 : 1;

	// compare data
	for (const_pointer s1 = _data, s2 = other._data;
			*s1 != '\0'; ++s1, ++s2) {

		// check equal
		if (*s1 == *s2)
			continue;

		// return less or greater
		return *s1 < *s2 ? -1 : 1;
	}

	// return equal
	return 0;
}


// -- public comparison operators ---------------------------------------------

/* equal */
auto cs::string::operator==(const ___self& ___ot) const noexcept -> bool {
	return _compare(___ot) == 0;
}

/* not equal */
auto cs::string::operator!=(const ___self& ___ot) const noexcept -> bool {
	return _compare(___ot) != 0;
}

/* less than */
auto cs::string::operator<(const ___self& ___ot) const noexcept -> bool {
	return _compare(___ot) < 0;
}

/* greater than */
auto cs::string::operator>(const ___self& ___ot) const noexcept -> bool {
	return _compare(___ot) > 0;
}

/* less than or equal to */
auto cs::string::operator<=(const ___self& ___ot) const noexcept -> bool {
	return _compare(___ot) <= 0;
}

/* greater than or equal to */
auto cs::string::operator>=(const ___self& ___ot) const noexcept -> bool {
	return _compare(___ot) >= 0;
}



// -- non-member functions ----------------------------------------------------

/* is ctrl */
auto cs::is_ctrl(const cs::string::value_type ___ch) noexcept -> bool {
	return ___ch < 0x20 || ___ch == 0x7f;
}



// -- non-member functions ----------------------------------------------------

/* operator << */
auto operator<<(std::ostream& ___os, const cs::string& ___str) -> std::ostream& {

	if (___str.empty())
		return ___os << "'empty'";
	else
		return ___os << ___str.data();
}

