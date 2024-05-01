#ifndef ___PARSER_HPP___
#define ___PARSER_HPP___

#include "cs/config.hpp"
#include "cs/containers/list.hpp"
#include "cs/containers/vector.hpp"
#include "cs/core/issue.hpp"

#if ___cs_requirements


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- P A R S E R ---------------------------------------------------------

	class parser final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = parser;

			/* size type */
			using ___size = unsigned long long;


		public:


			// -- public static methods ---------------------------------------

			/* parse */
			static auto parse(const int) -> cs::list<cs::issue>;




		private:

			/* char type */
			enum ___char_type : unsigned {

				CTRL,

				RETURN,
				NEWLINE,
				SPACE,

				NUMBER,
				LETTER,

				COLON,
				DOT,
				CIRCUMFLEX,
				TILDE,

				OTHER,
				EXTEND,

				MAX_CHAR_TYPE,
			};

			/* state type */
			enum ___state_type : unsigned {

				START,

				IN_RETURN,

				IN_PATH,
				IN_LINE,
				IN_COLUMN,
				IN_DIAGNOSTIC,

				SKIP_LINE,
				MAX_STATE,
				UNDEFINED,
			};

			/* action type */
			enum ___action_type : unsigned {

				RESET,
				COMPLETE_ISSUE,

				ADD_PATH,
				ADD_LINE,
				ADD_COLUMN,
				ADD_DIAGNOSTIC,

				COUNT_PATH,
				COMPUTE_LINE,
				COMPUTE_COLUMN,
				COUNT_DIAGNOSTIC,

				THROW_EXCEPTION,
				NO_ACTION,

				MAX_ACTION,
			};

			/* transition */
			struct ___transition final {
				___state_type _state;
				___action_type _action;
				const char* _msg;
			};


			// -- private constants -------------------------------------------

			/* buffer size */
			enum : ___size {
				BUFFER_SIZE = 1024,
			};


			// -- private members ---------------------------------------------

			/* list */
			cs::list<cs::issue>& _list;

			/* descriptor */
			const int _fd;

			/* buffer */
			char _buffer[BUFFER_SIZE + 1];

			/* data */
			const char* _data;

			/* issue */
			cs::issue _issue;


			// -- private modifiers -------------------------------------------

			/* parse */
			auto _parse(void) -> void;


			// -- private methods ---------------------------------------------

			/* reset */
			auto reset(void) noexcept -> void;

			/* complete issue */
			auto complete_issue(void) noexcept -> void;

			/* add path */
			auto add_path(void) noexcept -> void;

			/* add line */
			auto add_line(void) noexcept -> void;

			/* add column */
			auto add_column(void) noexcept -> void;

			/* add diagnostic */
			auto add_diagnostic(void) noexcept -> void;

			/* count path */
			auto count_path(void) noexcept -> void;

			/* compute line */
			auto compute_line(void) noexcept -> void;

			/* compute column */
			auto compute_column(void) noexcept -> void;

			/* count diagnostic */
			auto count_diagnostic(void) noexcept -> void;

			/* throw exception */
			auto throw_exception(void) noexcept -> void;

			/* no action */
			auto no_action(void) noexcept -> void;


			// -- private static methods --------------------------------------

			/* char type */
			static auto _chartype(const char) noexcept -> ___char_type;

			/* transition */
			static auto _transition(const ___state_type, const ___char_type) noexcept -> ___transition;

			/* action */
			static auto _action(___self&, const ___action_type) noexcept -> void;


			// -- private lifecycle -------------------------------------------

			/* deleted default constructor */
			parser(void) = delete;

			/* descriptor constructor */
			parser(cs::list<cs::issue>&, const int) noexcept;

			/* deleted copy constructor */
			parser(const ___self&) = delete;

			/* deleted move constructor */
			parser(___self&&) = delete;

			/* destructor */
			~parser(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const ___self&) -> ___self& = delete;

			/* deleted move assignment operator */
			auto operator=(___self&&) -> ___self& = delete;

	}; // class parser

} // namespace cs

#endif // ___cs_requirements

#endif // ___PARSER_HPP___
