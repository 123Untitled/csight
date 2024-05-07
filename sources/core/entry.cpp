#include "cs/core/entry.hpp"


// -- private lifecycle -------------------------------------------------------

/* members constructor */
cs::core::entry::entry(cs::string&& file,
					   cs::core::issue&& issue) noexcept
: _file{cs::move(file)}, _issues{} {
	_issues.push(cs::move(issue));
}


// -- public accessors --------------------------------------------------------

/* const file */
auto cs::core::entry::file(void) const noexcept -> const cs::string& {
	return _file;
}

/* const issues */
auto cs::core::entry::issues(void) const noexcept -> const cs::core::issues& {
	return _issues;
}
