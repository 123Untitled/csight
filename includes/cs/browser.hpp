#ifndef ___CS_BROWSER_HPP___
#define ___CS_BROWSER_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

// ::in_port_t
#include <netinet/in.h>


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- B R O W S E R --------------------------------------------------------

	auto open_browser(const ::in_port_t ___port) -> void;


} // namespace cs

#endif // ___cs_requirements

#endif // ___CS_BROWSER_HPP___
