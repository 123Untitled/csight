#ifndef CS_ISSUE_HEADER
#define CS_ISSUE_HEADER

#include "cs/config.hpp"
#include "cs/containers/vector.hpp"


#if ___cs_requirements


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- I S S U E -----------------------------------------------------------

	class issue final {

		public:

			// -- public types ------------------------------------------------

			/* size type */
			using size_type = typename cs::vector<char>::size_type;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = cs::issue;


			// -- private members ---------------------------------------------

			/* path */
			cs::vector<char> _path;

			/* line */
			size_type _line;

			/* column */
			size_type _column;

			/* message */
			cs::vector<char> _message;

			/* code */
			cs::vector<char> _code;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			issue(void) noexcept;

			/* copy constructor */
			issue(const ___self&) = default;

			/* move constructor */
			issue(___self&&) noexcept = default;

			/* destructor */
			~issue(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const ___self&) -> ___self& = default;

			/* move assignment operator */
			auto operator=(___self&&) -> ___self& = default;


			// -- public modifiers --------------------------------------------

			/* clear */
			auto clear(void) noexcept -> void;


			/* path */
			auto path(cs::vector<char>&&) noexcept -> void;

			/* line */
			auto line(const size_type) noexcept -> void;

			/* column */
			auto column(const size_type) noexcept -> void;

			/* message */
			auto message(cs::vector<char>&&) noexcept -> void;

			/* code */
			auto code(cs::vector<char>&&) noexcept -> void;


			// -- public accessors --------------------------------------------

			/* path */
			auto path(void) noexcept -> cs::vector<char>&;

			/* const path */
			auto path(void) const noexcept -> const cs::vector<char>&;

			/* line */
			auto line(void) noexcept -> size_type&;

			/* const line */
			auto line(void) const noexcept -> const size_type&;

			/* column */
			auto column(void) noexcept -> size_type&;

			/* const column */
			auto column(void) const noexcept -> const size_type&;

			/* message */
			auto message(void) noexcept -> cs::vector<char>&;

			/* const message */
			auto message(void) const noexcept -> const cs::vector<char>&;

			/* code */
			auto code(void) noexcept -> cs::vector<char>&;

			/* const code */
			auto code(void) const noexcept -> const cs::vector<char>&;


			/* print */
			auto print(void) const noexcept -> void;


	}; // class issue

} // namespace cs

#endif // ___cs_requirements

#endif // CS_ISSUE_HEADER
