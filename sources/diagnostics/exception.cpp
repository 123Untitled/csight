#include "cs/diagnostics/exception.hpp"


// -- public lifecycle --------------------------------------------------------

/* message constructor */
cs::runtime_error::runtime_error(const char* ___msg) noexcept
: _message{___msg} {
}


// -- public interface --------------------------------------------------------

/* what */
auto cs::runtime_error::what(void) const noexcept -> const char* {
	return _message;
}
