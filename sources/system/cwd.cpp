#include "cs/system/cwd.hpp"
#include "cs/diagnostics/exception.hpp"
#include <unistd.h>


// -- private lifecycle -------------------------------------------------------

/* default constructor */
cs::___impl::___cwd::___cwd(void)
/* : _buffer{}, _view{} uninitialized */ {

	// get current working directory
	if (::getcwd(_buffer, PATH_MAX) == nullptr) {
		// see pathconf
		//long int size = ::pathconf(".", _PC_PATH_MAX);
		throw cs::runtime_error("unable to get current working directory");
	}


	_view = _buffer;
}


// -- private static methods --------------------------------------------------

/* shared */
auto cs::___impl::___cwd::_shared(void) -> const ___self& {
	static cs::___impl::___cwd _instance;
	return _instance;
}


// -- public static accessors -------------------------------------------------

/* view */
auto cs::___impl::___cwd::view(void) -> const cs::string_view& {
	return _shared()._view;
}


// -- non-member functions ----------------------------------------------------

/* cwd */
auto cs::cwd(void) -> const cs::string_view& {
	return cs::___impl::___cwd::view();
}
