#ifndef ___CS_HINT_HPP___
#define ___CS_HINT_HPP___


#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/types.hpp"
#include <unistd.h>


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- H I N T  N A M E S P A C E ------------------------------------------

	namespace hint {


		/* success */
		template <cs::size_t ___sz>
		auto success(const char (&___msg)[___sz]) noexcept -> void {
			::write(STDOUT_FILENO, "\x1b[32m", 5);
			::write(STDOUT_FILENO, ___msg, ___sz);
			::write(STDOUT_FILENO, "\x1b[0m\n", 5);
		}

		/* warning */
		template <cs::size_t ___sz>
		auto warning(const char (&___msg)[___sz]) noexcept -> void {
			::write(STDOUT_FILENO, "\x1b[33m", 5);
			::write(STDOUT_FILENO, ___msg, ___sz);
			::write(STDOUT_FILENO, "\x1b[0m\n", 5);
		}

		/* error */
		template <cs::size_t ___sz>
		auto error(const char (&___msg)[___sz]) noexcept -> void {
			::write(STDOUT_FILENO, "\x1b[31m", 5);
			::write(STDOUT_FILENO, ___msg, ___sz);
			::write(STDOUT_FILENO, "\x1b[0m\n", 5);
		}

		/* info */
		template <cs::size_t ___sz>
		auto info(const char (&___msg)[___sz]) noexcept -> void {
			::write(STDOUT_FILENO, "\x1b[34m", 5);
			::write(STDOUT_FILENO, ___msg, ___sz);
			::write(STDOUT_FILENO, "\x1b[0m\n", 5);
		}

	} // namespace hint

} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_HINT_HPP___
