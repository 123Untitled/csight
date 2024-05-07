#include "cs/http/parser.hpp"
#include "cs/hint.hpp"


// -- public lifecycle --------------------------------------------------------

/* default constructor */
cs::http::parser::parser(void) noexcept
: _transition{&_machine[START][EOBUFF]},
  _request{nullptr}, _data{nullptr}, _end{nullptr}, _count{0U} {
}


// -- public accessors --------------------------------------------------------

/* complete */
auto cs::http::parser::complete(void) const noexcept -> bool {
	return _transition->_state == COMPLETE;
}


// -- public modifiers --------------------------------------------------------

/* clear */
auto cs::http::parser::clear(void) noexcept -> void {
	_transition = &_machine[START][EOBUFF];
	_count = 0U;
}


// -- private methods ---------------------------------------------------------

/* parse */
auto cs::http::parser::_parse(void) -> void {

	// loop over data
	for (; _data != _end; ++_data) {

		// get next transition
		_transition = &(_machine[_transition->_state])
			[_ctable[static_cast<unsigned char>(*_data)]];

		// execute action
		(this->*_actions[_transition->_action])();
	}

	// end of buffer marker
	_transition = &(_machine[_transition->_state][EOBUFF]);

	// execute action
	(this->*_actions[_transition->_action])();
}


/* increment */
auto cs::http::parser::increment(void) noexcept -> void {
	++_count;
}

/* new header */
auto cs::http::parser::new_header(void) -> void {
	_request->headers().emplace_back();
	++_count;
}

/* complete method */
auto cs::http::parser::complete_method(void) -> void {
	_request->method().name().append(_data - _count, _count);
	std::cout << "Method: " << _request->method().name() << std::endl;
	_count = 0U;
}


/* complete uri */
auto cs::http::parser::complete_uri(void) -> void {
	_request->method().uri().append(_data - _count, _count);
	_count = 0U;
}

/* complete version */
auto cs::http::parser::complete_version(void) -> void {
	_request->method().version().append(_data - _count, _count);
	_count = 0U;
}

/* complete header */
auto cs::http::parser::complete_header(void) -> void {
	_request->headers().back().name().append(_data - _count, _count);
	_count = 0U;
}

/* complete field */
auto cs::http::parser::complete_field(void) -> void {
	_request->headers().back().field().append(_data - _count, _count);
	_count = 0U;
}


/* error */
auto cs::http::parser::error(void) noexcept -> void {
	cs::hint::error("parser error");
}




const char* states[] {
	"START",
	"IN_METHOD",
	"WAIT_URI",
	"IN_URI",
	"WAIT_VERSION",
	"IN_VERSION",
	"WAIT_HEADER",
	"IN_HEADER",
	"WAIT_SEPARATOR",
	"IN_SEPARATOR",
	"WAIT_FIELD",
	"IN_FIELD",
	"WAIT_RETURN",
	"IN_RETURN",
	"WAIT_NEWLINE",
	"WAIT_BODY",
	"IN_BODY",
	"COMPLETE",
	"ERROR",
};

const char* actions[] {
	"INCREMENT",
	"NEW_HEADER",
	"COMPLETE_METHOD",
	"COMPLETE_URI",
	"COMPLETE_VERSION",
	"COMPLETE_HEADER",
	"COMPLETE_FIELD",
	"SKIP",
	"HANDLE_ERROR",
};

const char* methods[] {
	"GET",
	"HEAD",
	"TRACE",
	"DELETE",
	"OPTIONS",
	"PUT",
	"POST",
	"PATCH",
	"CONNECT",
};
