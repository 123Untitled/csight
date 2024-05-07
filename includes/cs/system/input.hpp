#ifndef ___CS_SYSTEM_INPUT_HPP___
#define ___CS_SYSTEM_INPUT_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/io_event.hpp"
#include "cs/system/descriptor.hpp"
#include "cs/core/parser.hpp"
#include "cs/containers/list.hpp"

#include <termios.h>


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- I N P U T -----------------------------------------------------------

	class input final : public cs::io_event {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = cs::input;


			// -- private members ---------------------------------------------

			/* stdin */
			cs::descriptor _stdin;

			/* parser */
			cs::parser _parser;

			/* issues */
			cs::list<cs::issue> _issues;

			/* terminal attributes */
			struct termios _term;


		public:


			// -- public lifecycle --------------------------------------------

			/* default constructor */
			input(void);

			/* deleted copy constructor */
			input(const ___self&) = delete;

			/* move constructor */
			input(___self&&) noexcept = default;

			/* destructor */
			virtual ~input(void) noexcept;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const ___self&) -> ___self& = delete;

			/* move assignment operator */
			auto operator=(___self&&) noexcept -> ___self& = default;


			// -- public overriden methods ------------------------------------

			/* read */
			auto read(void) -> void override;

			/* send */
			auto send(void) -> void override;

			/* descriptor */
			auto descriptor(void) const noexcept -> int override;


	}; // class input

} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_SYSTEM_INPUT_HPP___
