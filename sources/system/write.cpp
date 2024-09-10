#include "cs/system/write.hpp"

/* write */
auto cs::write(const cs::descriptor& ___desc, const cs::string& ___data) -> cs::size_t {

	// write data to descriptor
	const auto ___ret = ::write(___desc.handle(), ___data.data(), ___data.size());
	if (___ret == -1)
		throw cs::runtime_error("failed to write data to descriptor");

	return static_cast<cs::size_t>(___ret);
}

/* write */
auto cs::write(const cs::socket& ___desc, const cs::string& ___data) -> cs::size_t {

	// write data to descriptor
	const auto ___ret = ::write(___desc, ___data.data(), ___data.size());
	if (___ret == -1)
		throw cs::runtime_error("failed to write data to descriptor");

	return static_cast<cs::size_t>(___ret);
}
