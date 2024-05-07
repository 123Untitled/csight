#ifndef ___CS_SYSTEM_WRITE_HPP___
#define ___CS_SYSTEM_WRITE_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/system/descriptor.hpp"
#include "cs/string.hpp"


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- W R I T E -----------------------------------------------------------

	/* write */
	auto write(const cs::descriptor&, const cs::string&) -> cs::size_t;

} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_SYSTEM_WRITE_HPP___
