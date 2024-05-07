#ifndef ___PARSER_HPP___
#define ___PARSER_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/reader.hpp"
#include "cs/map.hpp"



// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- P A R S E R ---------------------------------------------------------

	class parser final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using ___self = parser;

			/* size type */
			using size_type = cs::size_t;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			parser(void) noexcept;

			/* deleted copy constructor */
			parser(const ___self&) = delete;

			/* move constructor */
			parser(___self&&) noexcept = default;

			/* destructor */
			~parser(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const ___self&) -> ___self& = delete;

			/* move assignment operator */
			auto operator=(___self&&) noexcept -> ___self& = default;


			// -- public methods ----------------------------------------------

			/* parse */
			template <cs::size_t ___sz>
			auto parse(const cs::reader<___sz>& ___rd,
							 cs::core::map& ___map) -> void {

				// set data
				_map  = &___map;
				_data = ___rd.data();
				_end  = _data + ___rd.size();

				// call parse
				_parse();
			}


		private:

			/* char type */
			enum ___chart : unsigned {

				// control
				CTRL,

				// carriage return
				RETURN,

				// line feed
				NEWLINE,

				// escape
				ESCAPE,

				// horizontal tab, space
				SPACE,

				// digits [0-9]
				NUMBER,

				// letters [a-zA-Z]
				LETTER,

				// lower 'm' (end escape sequence)
				LOWER_M,

				// open bracket '['
				BRACKET,

				// colon ':'
				COLON,

				// semicolon ';'
				SEMICOLON,

				// dot '.'
				DOT,

				// circumflex '^'
				CIRCUMFLEX,

				// tilde '~'
				TILDE,

				// other printable characters
				OTHER,

				// end of buffer (manual marker)
				EOBUFF, // not generate by char table, but used in state table

				MAX_CHARTYPE,

			};

			/* state type */
			enum ___state : unsigned {

				START,

				IN_FILE,

				WAIT_LINE,
				IN_LINE,

				WAIT_COLUMN,
				IN_COLUMN,

				WAIT_STATUS,
				IN_STATUS,

				WAIT_DIAGNOSTIC,
				IN_DIAGNOSTIC,

				IN_RETURN,

				WAIT_ESCAPE,
				IN_ESCAPE,


				ABORT,

				MAX_STATE,
			};

			/* action type */
			enum ___action : unsigned {

				RESET,

				INCREMENT,

				COMPLETE_ISSUE,

				COMPLETE_FILE,
				EOBUFF_FILE,

				COMPLETE_STATUS,

				COMPLETE_DIAGNOSTIC,
				EOBUFF_DIAGNOSTIC,

				COMPUTE_LINE,
				COMPUTE_COLUMN,


				// start escape sequence
				HANDLE_ESCAPE, // \x1b
				// finish escape sequence
				COMPLETE_ESCAPE, // \x1b[...m

				NO_ACTION,

				MAX_ACTION,
			};

			/* transition */
			struct ___transition final {
				___state _state;
				___action _action;
			};


			// -- private members ---------------------------------------------

			/* transition */
			const ___transition* _transition;

			/* back state */
			___state _back;

			/* data */
			const char* _data;

			/* end */
			const char* _end;

			/* map */
			cs::core::map* _map;

			/* file */
			cs::string _file;

			/* issue */
			cs::core::issue _issue;

			/* count */
			size_type _count;


			// -- private modifiers -------------------------------------------

			/* parse */
			auto _parse(void) -> void;


			// -- private methods ---------------------------------------------

			/* reset */
			auto reset(void) noexcept -> void;

			/* increment */
			auto increment(void) noexcept -> void;


			/* complete issue */
			auto complete_issue(void) -> void;

			/* complete file */
			auto complete_file(void) -> void;

			/* complete status */
			auto complete_status(void) -> void;


			/* save file */
			auto save_file(void) -> void;

			/* complete diagnostic */
			auto complete_diagnostic(void) -> void;

			/* save diagnostic */
			auto save_diagnostic(void) -> void;


			/* compute line */
			auto compute_line(void) noexcept -> void;

			/* compute column */
			auto compute_column(void) noexcept -> void;


			// -- escape sequence ---------------------------------------------

			/* handle escape */
			auto handle_escape(void) noexcept -> void;

			/* complete escape */
			auto complete_escape(void) noexcept -> void;


			/* no action */
			inline auto no_action(void) noexcept -> void {}



			// -- private types -----------------------------------------------

			/* procedure type */
			using ___procedure = auto (parser::*)(void) -> void;


			// -- private static members --------------------------------------

			/* action table */
			static constexpr ___procedure _actions[MAX_ACTION] = {

				&___self::reset,

				&___self::increment,

				&___self::complete_issue,

				&___self::complete_file,
				&___self::save_file,

				&___self::complete_status,

				&___self::complete_diagnostic,
				&___self::save_diagnostic,

				&___self::compute_line,
				&___self::compute_column,

				&___self::handle_escape,
				&___self::complete_escape,

				&___self::no_action,
			};


			/* state machine table */
			static constexpr ___transition _machine[MAX_STATE][MAX_CHARTYPE] {

				// START
				// expect first file character (can be anything, except control)
				{
					// CTRL
					{ ABORT, NO_ACTION },
					// RETURN
					{ ABORT, NO_ACTION },

					// NEWLINE
					{ START, NO_ACTION }, // maybe not reset

					// ESCAPE
					{ START, HANDLE_ESCAPE }, // state is assigned in action

					// SPACE
					{ IN_FILE, INCREMENT },
					// NUMBER
					{ IN_FILE, INCREMENT },
					// LETTER
					{ IN_FILE, INCREMENT },
					// LOWER_M
					{ IN_FILE, INCREMENT },
					// BRACKET
					{ IN_FILE, INCREMENT },

					// COLON
					{ IN_FILE, INCREMENT }, // case of colon in path
					// SEMICOLON
					{ IN_FILE, INCREMENT },
					// DOT
					{ IN_FILE, INCREMENT },
					// CIRCUMFLEX
					{ IN_FILE, INCREMENT },
					// TILDE
					{ IN_FILE, INCREMENT },
					// OTHER
					{ IN_FILE, INCREMENT },

					// EOBUFF
					{ START, NO_ACTION },
				},


				// IN_FILE
				// expect file character (can be anything, except control)
				// or colon (to switch to wait line)
				{
					// CTRL
					{ ABORT, RESET },
					// RETURN
					{ ABORT, RESET },
					// NEWLINE
					{ START, RESET },

					// ESCAPE
					{ IN_FILE, HANDLE_ESCAPE }, // state is assigned in action

					// SPACE
					{ IN_FILE, INCREMENT },
					// NUMBER
					{ IN_FILE, INCREMENT },
					// LETTER
					{ IN_FILE, INCREMENT },
					// LOWER_M
					{ IN_FILE, INCREMENT },
					// BRACKET
					{ IN_FILE, INCREMENT },

					// COLON
					{ WAIT_LINE, COMPLETE_FILE }, // try to complete file

					// SEMICOLON
					{ IN_FILE, INCREMENT },
					// DOT
					{ IN_FILE, INCREMENT },
					// CIRCUMFLEX
					{ IN_FILE, INCREMENT },
					// TILDE
					{ IN_FILE, INCREMENT },
					// OTHER
					{ IN_FILE, INCREMENT },

					// EOBUFF
					{ IN_FILE, EOBUFF_FILE }, // save for next read
				},

				// WAIT LINE
				// expect first digit character
				{
					// CTRL
					{ ABORT, RESET },
					// RETURN
					{ ABORT, RESET },
					// NEWLINE
					{ START, RESET },

					// ESCAPE
					{ WAIT_LINE, HANDLE_ESCAPE }, // state is assigned in action

					// SPACE
					{ ABORT, RESET },

					// NUMBER
					{ IN_LINE, COMPUTE_LINE }, // switch to line state

					// LETTER
					{ ABORT, RESET },
					// LOWER_M
					{ ABORT, RESET },
					// BRACKET
					{ ABORT, RESET },
					// COLON
					{ ABORT, RESET },
					// SEMICOLON
					{ ABORT, RESET },
					// DOT
					{ ABORT, RESET },
					// CIRCUMFLEX
					{ ABORT, RESET },
					// TILDE
					{ ABORT, RESET },
					// OTHER
					{ ABORT, RESET },

					// EOBUFF
					{ WAIT_LINE, NO_ACTION }, // wait for next read
				},


				// IN_LINE
				// expect digit character
				// or colon (to switch to wait column)
				{
					// CTRL
					{ ABORT, RESET },
					// RETURN
					{ ABORT, RESET },
					// NEWLINE
					{ START, RESET },

					// ESCAPE
					{ IN_LINE, HANDLE_ESCAPE }, // state is assigned in action

					// SPACE
					{ ABORT, RESET },

					// NUMBER
					{ IN_LINE, COMPUTE_LINE }, // compute line

					// LETTER
					{ ABORT, RESET },
					// LOWER_M
					{ ABORT, RESET },
					// BRACKET
					{ ABORT, RESET },

					// COLON
					{ WAIT_COLUMN, NO_ACTION },

					// SEMICOLON
					{ ABORT, RESET },
					// DOT
					{ ABORT, RESET },
					// CIRCUMFLEX
					{ ABORT, RESET },
					// TILDE
					{ ABORT, RESET },
					// OTHER
					{ ABORT, RESET },

					// EOBUFF
					{ IN_LINE, NO_ACTION },
				},

				// WAIT_COLUMN
				// expect first digit character
				{
					// CTRL
					{ ABORT, RESET },
					// RETURN
					{ ABORT, RESET },
					// NEWLINE
					{ START, RESET },

					// ESCAPE
					{ WAIT_COLUMN, HANDLE_ESCAPE }, // state is assigned in action

					// SPACE
					{ ABORT, RESET },

					// NUMBER
					{ IN_COLUMN, COMPUTE_COLUMN }, // switch to column state

					// LETTER
					{ ABORT, RESET },
					// LOWER_M
					{ ABORT, RESET },
					// BRACKET
					{ ABORT, RESET },
					// COLON
					{ ABORT, RESET },
					// SEMICOLON
					{ ABORT, RESET },
					// DOT
					{ ABORT, RESET },
					// CIRCUMFLEX
					{ ABORT, RESET },
					// TILDE
					{ ABORT, RESET },
					// OTHER
					{ ABORT, RESET },

					// EOBUFF
					{ WAIT_COLUMN, NO_ACTION },
				},

				// IN_COLUMN
				// expect digit character
				// or colon (to switch to wait status)
				{
					// CTRL
					{ ABORT, RESET },
					// RETURN
					{ ABORT, RESET },
					// NEWLINE
					{ START, RESET },

					// ESCAPE
					{ IN_COLUMN, HANDLE_ESCAPE }, // state is assigned in action

					// SPACE
					{ ABORT, RESET },

					// NUMBER
					{ IN_COLUMN, COMPUTE_COLUMN },

					// LETTER
					{ ABORT, RESET },
					// LOWER_M
					{ ABORT, RESET },
					// BRACKET
					{ ABORT, RESET },

					// COLON
					{ WAIT_STATUS, NO_ACTION }, // switch to wait status

					// SEMICOLON
					{ ABORT, RESET },
					// DOT
					{ ABORT, RESET },
					// CIRCUMFLEX
					{ ABORT, RESET },
					// TILDE
					{ ABORT, RESET },
					// OTHER
					{ ABORT, RESET },

					// EOBUFF
					{ IN_COLUMN, NO_ACTION },
				},

				// WAIT_STATUS
				// expect first letter character
				// or space (skip)
				{
					// CTRL
					{ ABORT, RESET },
					// RETURN
					{ ABORT, RESET },
					// NEWLINE
					{ START, RESET },

					// ESCAPE
					{ WAIT_STATUS, HANDLE_ESCAPE }, // state is assigned in action

					// SPACE
					{ WAIT_STATUS, NO_ACTION },

					// NUMBER
					{ ABORT, RESET },

					// LETTER
					{ IN_STATUS, INCREMENT }, // switch to status state
					// LOWER_M
					{ IN_STATUS, INCREMENT }, // ('m' is not valid status character but for now accept it)

					// BRACKET
					{ ABORT, RESET },
					// COLON
					{ ABORT, RESET },
					// SEMICOLON
					{ ABORT, RESET },
					// DOT
					{ ABORT, RESET },
					// CIRCUMFLEX
					{ ABORT, RESET },
					// TILDE
					{ ABORT, RESET },
					// OTHER
					{ ABORT, RESET },

					// EOBUFF
					{ WAIT_STATUS, NO_ACTION },
				},

				// IN_STATUS
				// expect letter character
				// or colon (to switch to wait diagnostic)
				{
					// CTRL
					{ ABORT, RESET },
					// RETURN
					{ ABORT, RESET },
					// NEWLINE
					{ START, RESET },

					// ESCAPE
					{ IN_STATUS, HANDLE_ESCAPE }, // state is assigned in action

					// SPACE (fatal error case)
					{ IN_STATUS, INCREMENT },

					// NUMBER
					{ ABORT, RESET },

					// LETTER
					{ IN_STATUS, INCREMENT }, // increment status
					// LOWER_M
					{ IN_STATUS, INCREMENT }, // ('m' is not valid status character but for now accept it)

					// BRACKET
					{ ABORT, RESET },

					// COLON
					{ WAIT_DIAGNOSTIC, COMPLETE_STATUS }, // switch to diagnostic state

					/* create action to complete status
					 * !!!
					 */

					// SEMICOLON
					{ ABORT, RESET },
					// DOT
					{ ABORT, RESET },
					// CIRCUMFLEX
					{ ABORT, RESET },
					// TILDE
					{ ABORT, RESET },
					// OTHER
					{ ABORT, RESET },

					// EOBUFF
					{ IN_STATUS, NO_ACTION },
				},

				// WAIT_DIAGNOSTIC
				// expect first diagnostic character
				// or space
				{
					// CTRL
					{ ABORT, RESET },
					// RETURN
					{ ABORT, RESET },
					// NEWLINE
					{ START, RESET },

					// ESCAPE
					{ WAIT_DIAGNOSTIC, HANDLE_ESCAPE }, // state is assigned in action

					// SPACE
					{ WAIT_DIAGNOSTIC, NO_ACTION },

					// NUMBER
					{ IN_DIAGNOSTIC, INCREMENT },
					// LETTER
					{ IN_DIAGNOSTIC, INCREMENT },
					// LOWER_M
					{ IN_DIAGNOSTIC, INCREMENT },
					// BRACKET
					{ IN_DIAGNOSTIC, INCREMENT },
					// COLON
					{ IN_DIAGNOSTIC, INCREMENT },
					// SEMICOLON
					{ IN_DIAGNOSTIC, INCREMENT },
					// DOT
					{ IN_DIAGNOSTIC, INCREMENT },
					// CIRCUMFLEX
					{ IN_DIAGNOSTIC, INCREMENT },
					// TILDE
					{ IN_DIAGNOSTIC, INCREMENT },
					// OTHER
					{ IN_DIAGNOSTIC, INCREMENT },

					// EOBUFF
					{ WAIT_DIAGNOSTIC, NO_ACTION },
				},

				// IN_DIAGNOSTIC
				{
					// CTRL
					{ ABORT, RESET },

					// RETURN
					{ IN_RETURN, COMPLETE_DIAGNOSTIC },
					// NEWLINE
					{ START, COMPLETE_DIAGNOSTIC },

					// ESCAPE
					{ IN_DIAGNOSTIC, HANDLE_ESCAPE }, // state is assigned in action

					// SPACE
					{ IN_DIAGNOSTIC, INCREMENT },
					// NUMBER
					{ IN_DIAGNOSTIC, INCREMENT },
					// LETTER
					{ IN_DIAGNOSTIC, INCREMENT },
					// LOWER_M
					{ IN_DIAGNOSTIC, INCREMENT },
					// BRACKET
					{ IN_DIAGNOSTIC, INCREMENT },
					// COLON
					{ IN_DIAGNOSTIC, INCREMENT },
					// SEMICOLON
					{ IN_DIAGNOSTIC, INCREMENT },
					// DOT
					{ IN_DIAGNOSTIC, INCREMENT },
					// CIRCUMFLEX
					{ IN_DIAGNOSTIC, INCREMENT },
					// TILDE
					{ IN_DIAGNOSTIC, INCREMENT },
					// OTHER
					{ IN_DIAGNOSTIC, INCREMENT },

					// EOBUFF
					{ IN_DIAGNOSTIC, EOBUFF_DIAGNOSTIC },
				},

				// IN_RETURN
				{
					// CTRL
					{ ABORT, RESET },

					// RETURN
					{ IN_RETURN, NO_ACTION },
					// NEWLINE
					{ START, COMPLETE_ISSUE }, // here see

					// ESCAPE
					{ ABORT, RESET },

					// SPACE
					{ ABORT, RESET },

					// NUMBER
					{ ABORT, RESET },
					// LETTER
					{ ABORT, RESET },
					// LOWER_M
					{ ABORT, RESET },
					// BRACKET
					{ ABORT, RESET },
					// COLON
					{ ABORT, RESET },
					// SEMICOLON
					{ ABORT, RESET },
					// DOT
					{ ABORT, RESET },
					// CIRCUMFLEX
					{ ABORT, RESET },
					// TILDE
					{ ABORT, RESET },

					// OTHER
					{ ABORT, RESET },

					// EOBUFF
					{ IN_RETURN, NO_ACTION },

				},

				// WAIT_ESCAPE
				// expect '['
				// or \x1b (skip multiple esc)
				{
					// CTRL
					{ ABORT, RESET },
					// RETURN
					{ ABORT, RESET },
					// NEWLINE
					{ ABORT, RESET },

					// ESCAPE
					{ WAIT_ESCAPE, NO_ACTION },

					// SPACE
					{ ABORT, RESET },
					// NUMBER
					{ ABORT, RESET },
					// LETTER
					{ ABORT, RESET },
					// LOWER_M
					{ ABORT, RESET },

					// BRACKET
					{ IN_ESCAPE, NO_ACTION }, // switch to escape state

					// COLON
					{ ABORT, RESET },
					// SEMICOLON
					{ ABORT, RESET },
					// DOT
					{ ABORT, RESET },
					// CIRCUMFLEX
					{ ABORT, RESET },
					// TILDE
					{ ABORT, RESET },
					// OTHER
					{ ABORT, RESET },

					// EOBUFF
					{ WAIT_ESCAPE, NO_ACTION },
				},

				// IN_ESCAPE
				// expect [0-9;]
				// or 'm' to end
				{
					// CTRL
					{ ABORT, RESET },
					// RETURN
					{ ABORT, RESET },
					// NEWLINE
					{ ABORT, RESET },
					// ESCAPE
					{ ABORT, RESET },
					// SPACE
					{ ABORT, RESET },

					// NUMBER
					{ IN_ESCAPE, NO_ACTION },

					// LETTER
					{ ABORT, RESET },

					// LOWER_M
					{ IN_ESCAPE, COMPLETE_ESCAPE }, // state is assigned in action

					// BRACKET
					{ ABORT, RESET },
					// COLON
					{ ABORT, RESET },

					// SEMICOLON
					{ IN_ESCAPE, NO_ACTION },

					// DOT
					{ ABORT, RESET },
					// CIRCUMFLEX
					{ ABORT, RESET },
					// TILDE
					{ ABORT, RESET },
					// OTHER
					{ ABORT, RESET },

					// EOBUFF
					{ IN_ESCAPE, NO_ACTION },
				},



				// ABORT
				{
					// CTRL
					{ ABORT, NO_ACTION },
					// RETURN
					{ ABORT, NO_ACTION },
					// NEWLINE
					{ START, NO_ACTION },
					// ESCAPE
					{ ABORT, NO_ACTION },
					// SPACE
					{ ABORT, NO_ACTION },
					// NUMBER
					{ ABORT, NO_ACTION },
					// LETTER
					{ ABORT, NO_ACTION },
					// LOWER_M
					{ ABORT, NO_ACTION },
					// BRACKET
					{ ABORT, NO_ACTION },
					// COLON
					{ ABORT, NO_ACTION },
					// SEMICOLON
					{ ABORT, NO_ACTION },
					// DOT
					{ ABORT, NO_ACTION },
					// CIRCUMFLEX
					{ ABORT, NO_ACTION },
					// TILDE
					{ ABORT, NO_ACTION },
					// OTHER
					{ ABORT, NO_ACTION },
					// EOBUFF
					{ ABORT, NO_ACTION },
				},

			};


			// [info, note, warning, error, fatal error]
			// aefgilnortw (letter diagnostic states)

			// a 0x61
			// e 0x65
			// f 0x66
			// g 0x67
			// i 0x69
			// l 0x6C
			// n 0x6E
			// o 0x6F
			// r 0x72
			// t 0x74
			// w 0x77



			/* char table */
			static constexpr ___chart _ctable[256] {

				// 0x00 - 0x08
				CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL,
				CTRL,

				// horizontal tab 0x09
				SPACE,

				// newline 0x0A
				NEWLINE,

				// 0x0B - 0x0C
				CTRL, CTRL,

				// carriage return 0x0D
				RETURN,

				// 0x0E - 0x1F
				CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL,
				CTRL, CTRL, CTRL, CTRL, CTRL,

				// escape 0x1B
				ESCAPE,

				// 0x1C - 0x1F
				CTRL, CTRL, CTRL, CTRL,

				// space 0x20
				SPACE,

				// 0x21 - 0x2D
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER,

				// dot 0x2E
				DOT,

				// 0x2F
				OTHER,

				// number 0x30 - 0x39
				NUMBER, NUMBER, NUMBER, NUMBER, NUMBER, NUMBER, NUMBER, NUMBER,
				NUMBER, NUMBER,

				// colon 0x3A
				COLON,

				// semicolon 0x3B
				SEMICOLON,

				// 0x3C - 0x40
				OTHER, OTHER, OTHER, OTHER, OTHER,

				// upper case 0x41 - 0x5A
				LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,
				LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,
				LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,
				LETTER, LETTER,

				// open bracket 0x5B
				BRACKET,

				// 0x5C - 0x5D
				OTHER, OTHER,

				// circumflex 0x5E
				CIRCUMFLEX,

				// 0x5F - 0x60
				OTHER, OTHER,


				// lower case 0x61 - 0x7A
				LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,
				LETTER, LETTER, LETTER, LETTER,

				// LOWER_M 0x6D
				LOWER_M,

				LETTER, LETTER, LETTER,
				LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,
				LETTER, LETTER,

				// 0x7B - 0x7D
				OTHER, OTHER, OTHER,

				// tilde 0x7E
				TILDE,

				// delete 0x7F
				CTRL,

				// extended ascii 0x80 - 0xFF
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER
			};

	}; // class parser

} // namespace cs

#endif // ___cs_requirements

#endif // ___PARSER_HPP___
