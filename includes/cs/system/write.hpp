#ifndef ___CS_SYSTEM_WRITE_HPP___
#define ___CS_SYSTEM_WRITE_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/diagnostics/exception.hpp"
#include "cs/system/descriptor.hpp"
#include "cs/network/socket.hpp"
#include "cs/string.hpp"

#include <unistd.h>


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {

	class socket;

	// -- W R I T E -----------------------------------------------------------

	/* write */
	auto write(const cs::descriptor&, const cs::string&) -> cs::size_t;
	auto write(const cs::socket&, const cs::string&) -> cs::size_t;

	/* write */
	template <cs::size_t ___size>
	auto write(const cs::descriptor& ___desc, const char (&___data)[___size]) -> cs::size_t {
		const auto ___ret = ::write(___desc, ___data, ___size);
		if (___ret == -1)
			throw cs::runtime_error("failed to write descriptor");

		return static_cast<cs::size_t>(___ret);
	}

	/* write */
	template <cs::size_t ___size>
	auto write(const cs::socket& ___desc, const char (&___data)[___size]) -> cs::size_t {
		const auto ___ret = ::write(___desc, ___data, ___size);
		if (___ret == -1)
			throw cs::runtime_error("failed to write descriptor");

		return static_cast<cs::size_t>(___ret);
	}

} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_SYSTEM_WRITE_HPP___
