#include "cs/http/request.hpp"


// -- public modifiers --------------------------------------------------------

/* clear */
auto cs::http::request::clear(void) noexcept -> void {
	_method.clear();
	_headers.clear();
}


// -- public accessors --------------------------------------------------------

/* method */
auto cs::http::request::method(void) noexcept -> cs::http::method& {
	return _method;
}

/* const method */
auto cs::http::request::method(void) const noexcept -> const cs::http::method& {
	return _method;
}

/* headers */
auto cs::http::request::headers(void) noexcept -> cs::list<cs::http::header>& {
	return _headers;
}

/* const headers */
auto cs::http::request::headers(void) const noexcept -> const cs::list<cs::http::header>& {
	return _headers;
}
