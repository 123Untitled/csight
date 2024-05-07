#include "cs/http/header.hpp"


// -- public accessors --------------------------------------------------------

/* name */
auto cs::http::header::name(void) noexcept -> cs::vector<char>& {
	return _name;
}

/* const name */
auto cs::http::header::name(void) const noexcept -> const cs::vector<char>& {
	return _name;
}

/* field */
auto cs::http::header::field(void) noexcept -> cs::vector<char>& {
	return _field;
}

/* const field */
auto cs::http::header::field(void) const noexcept -> const cs::vector<char>& {
	return _field;
}
