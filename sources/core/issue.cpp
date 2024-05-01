#include "cs/core/issue.hpp"

#if ___cs_requirements


// -- public lifecycle --------------------------------------------------------

/* default constructor */
cs::issue::issue(void) noexcept
: _path{}, _line{0U}, _column{0U}, _message{}, _code{} {
}


// -- public modifiers --------------------------------------------------------

/* clear */
auto cs::issue::clear(void) noexcept -> void {
	_path.clear();
	_line = 0U;
	_column = 0U;
	_message.clear();
	_code.clear();
}


/* path */
auto cs::issue::path(cs::vector<char>&& ___path) noexcept -> void {
	_path = static_cast<cs::vector<char>&&>(___path);
}

/* line */
auto cs::issue::line(const size_type ___line) noexcept -> void {
	_line = ___line;
}

/* column */
auto cs::issue::column(const size_type ___column) noexcept -> void {
	_column = ___column;
}

/* message */
auto cs::issue::message(cs::vector<char>&& ___message) noexcept -> void {
	_message = static_cast<cs::vector<char>&&>(___message);
}

/* code */
auto cs::issue::code(cs::vector<char>&& ___code) noexcept -> void {
	_code = static_cast<cs::vector<char>&&>(___code);
}


// -- public accessors --------------------------------------------------------

/* path */
auto cs::issue::path(void) noexcept -> cs::vector<char>& {
	return _path;
}

/* const path */
auto cs::issue::path(void) const noexcept -> const cs::vector<char>& {
	return _path;
}

/* line */
auto cs::issue::line(void) noexcept -> size_type& {
	return _line;
}

/* const line */
auto cs::issue::line(void) const noexcept -> const size_type& {
	return _line;
}

/* column */
auto cs::issue::column(void) noexcept -> size_type& {
	return _column;
}

/* const column */
auto cs::issue::column(void) const noexcept -> const size_type& {
	return _column;
}

/* message */
auto cs::issue::message(void) noexcept -> cs::vector<char>& {
	return _message;
}

/* const message */
auto cs::issue::message(void) const noexcept -> const cs::vector<char>& {
	return _message;
}

/* code */
auto cs::issue::code(void) noexcept -> cs::vector<char>& {
	return _code;
}

/* const code */
auto cs::issue::code(void) const noexcept -> const cs::vector<char>& {
	return _code;
}


#include <iostream>
#include <unistd.h>

/* print */
auto cs::issue::print(void) const noexcept -> void {
	::write(1, "\x1b[32mPATH\x1b[0m ", 14);
	::write(1, _path.data(), _path.size());
	::write(1, "\n", 1);

	::write(1, "\x1b[32mLINE\x1b[0m ", 14);
	::write(1, std::to_string(_line).data(), std::to_string(_line).size());
	::write(1, "\n", 1);

	::write(1, "\x1b[32mCOLUMN\x1b[0m ", 16);
	::write(1, std::to_string(_column).data(), std::to_string(_column).size());
	::write(1, "\n", 1);

	::write(1, "\x1b[32mMESSAGE\x1b[0m ", 17);
	::write(1, _message.data(), _message.size());
	::write(1, "\n\n", 2);

}


#endif // ___cs_requirements
