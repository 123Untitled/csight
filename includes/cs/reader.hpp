#ifndef ___CS_READER_HPP___
#define ___CS_READER_HPP___


#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/types.hpp"
#include "cs/network/socket.hpp"
#include "cs/system/descriptor.hpp"
#include "cs/diagnostics/exception.hpp"


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- R E A D E R ---------------------------------------------------------

	template <cs::size_t ___size>
	class reader final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = cs::reader<___size>;



		public:

			// -- public types ------------------------------------------------

			/* size type */
			using size_type = decltype(___size);


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			reader(void) noexcept
			: /* _buffer{} uninitialized */ _size{0U} {
			}

			/* copy constructor */
			reader(const ___self&) noexcept = default;

			/* move constructor */
			reader(___self&&) noexcept = default;

			/* destructor */
			~reader(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const ___self&) noexcept -> ___self& = default;

			/* move assignment operator */
			auto operator=(___self&&) noexcept -> ___self& = default;


			// -- public modifiers --------------------------------------------

			/* read */
			auto read(const cs::socket& ___so) -> void {

				// read from socket
				const auto ___ret = ::recv(___so, _buffer, ___size, 0);

				// check for error
				if (___ret < 0)
					throw cs::runtime_error{"recv() failed"};

				// cast size
				_size = static_cast<size_type>(___ret);
			}

			/* read */
			auto read(const cs::descriptor& ___fd) -> void {

				// read from file descriptor
				const auto ___ret = ::read(___fd, _buffer, ___size);

				// check for error
				if (___ret < 0)
					throw cs::runtime_error{"read() failed"};

				// cast size
				_size = static_cast<size_type>(___ret);
			}


			// -- public accessors --------------------------------------------

			/* data */
			auto data(void) noexcept -> char (&)[___size] {
				return _buffer;
			}

			/* const data */
			auto data(void) const noexcept -> const char (&)[___size] {
				return _buffer;
			}

			/* size */
			auto size(void) const noexcept -> size_type {
				return _size;
			}

			/* capacity */
			constexpr auto capacity(void) const noexcept -> size_type {
				return ___size;
			}

			/* empty */
			auto empty(void) const noexcept -> bool {
				return _size == 0U;
			}

			/* eof */
			auto eof(void) const noexcept -> bool {
				return _size == 0U;
			}


		private:

			// -- private members --------------------------------------------

			/* buffer */
			char _buffer[___size];

			/* size */
			cs::size_t _size;

	}; // class reader

} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_READER_HPP___
