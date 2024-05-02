#ifndef ___CS_ADDRESS_HPP___
#define ___CS_ADDRESS_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/memory/memcmp.hpp"

#include <netinet/in.h>


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- A D D R E S S -------------------------------------------------------

	class address final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = cs::address;


			// -- private members ---------------------------------------------

			/* address */
			::sockaddr_storage _addr;

			/* size */
			::socklen_t        _size;


		public:


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			constexpr address(void) noexcept
			: _addr{}, _size{sizeof(::sockaddr_storage)} {
			}

			/* ipv4 constructor */
			constexpr address(const ::in_addr& ip, const ::in_port_t port) noexcept
			: _addr{}, _size{sizeof(::sockaddr_in)} {

				// interpret as ipv4
				auto& addr = as_ipv4();

				// set fields
				addr.sin_family      = AF_INET;
				addr.sin_port        = htons(port);
				addr.sin_addr.s_addr = ip.s_addr;
			}

			/* ipv6 constructor */
			constexpr address(const ::in6_addr& ip, const ::in_port_t port) noexcept
			: _addr{}, _size{sizeof(::sockaddr_in6)} {

				// interpret as ipv6
				auto& addr = as_ipv6();

				// set fields
				addr.sin6_family = AF_INET6;
				addr.sin6_port   = htons(port);
				addr.sin6_addr   = ip;
			}

			/* copy constructor */
			constexpr address(const ___self&) noexcept = default;

			/* move constructor */
			constexpr address(___self&&) noexcept = default;

			/* destructor */
			~address(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			constexpr auto operator=(const ___self&) noexcept -> ___self& = default;

			/* move assignment operator */
			constexpr auto operator=(___self&&) noexcept -> ___self& = default;


			// -- public accessors --------------------------------------------

			/* size */
			constexpr auto size(void) noexcept -> ::socklen_t& {
				return _size;
			}

			/* const size */
			constexpr auto size(void) const noexcept -> const ::socklen_t& {
				return _size;
			}


			/* is ipv4 */
			constexpr auto is_ipv4(void) const noexcept -> bool {
				return _addr.ss_family == AF_INET;
			}

			/* is ipv6 */
			constexpr auto is_ipv6(void) const noexcept -> bool {
				return _addr.ss_family == AF_INET6;
			}


			/* as ipv4 */
			constexpr auto as_ipv4(void) noexcept -> ::sockaddr_in& {
				return *static_cast<::sockaddr_in*>(static_cast<void*>(&_addr));
			}

			/* const as ipv4 */
			constexpr auto as_ipv4(void) const noexcept -> const ::sockaddr_in& {
				return *static_cast<const ::sockaddr_in*>(static_cast<const void*>(&_addr));
			}


			/* as ipv6 */
			constexpr auto as_ipv6(void) noexcept -> ::sockaddr_in6& {
				return *static_cast<::sockaddr_in6*>(static_cast<void*>(&_addr));
			}

			/* const as ipv6 */
			constexpr auto as_ipv6(void) const noexcept -> const ::sockaddr_in6& {
				return *static_cast<const ::sockaddr_in6*>(static_cast<const void*>(&_addr));
			}


			/* as sockaddr */
			constexpr auto as_sockaddr(void) noexcept -> ::sockaddr& {
				return *static_cast<::sockaddr*>(static_cast<void*>(&_addr));
			}

			/* const as sockaddr */
			constexpr auto as_sockaddr(void) const noexcept -> const ::sockaddr& {
				return *static_cast<const ::sockaddr*>(static_cast<const void*>(&_addr));
			}

			/* is localhost */
			constexpr auto is_localhost(void) const noexcept -> bool {

				// check for ipv4
				if (_addr.ss_family == AF_INET)
					return as_ipv4().sin_addr.s_addr == htonl(INADDR_LOOPBACK);

				// check for ipv6
				else if (_addr.ss_family == AF_INET6)
					return cs::memcmp(&as_ipv6().sin6_addr, &in6addr_loopback) == 0;

				return false;
			}


			// -- public modifiers --------------------------------------------

			/* clear */
			 constexpr auto clear(void) noexcept -> void {
				_addr = {};
				_size = sizeof(::sockaddr_storage);
			}


	}; // class address

} // namespace cs

#endif // ___cs_requirements

#endif // ___CS_ADDRESS_HPP___


/*
	 auto presentation_to_network(const std::string_view& ip, ::in_addr& addr) noexcept -> bool {
		return ::inet_pton(AF_INET, ip.data(), &addr) == 1;
	}

	 auto presentation_to_network(const std::string_view& ip, ::in6_addr& addr) noexcept -> bool {
		return ::inet_pton(AF_INET6, ip.data(), &addr) == 1;
	}
	*/
