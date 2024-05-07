#ifndef ___CS_HTTP_PARSER_HPP___
#define ___CS_HTTP_PARSER_HPP___

#include "cs/config.hpp"
#if ___cs_requirements

#include "cs/types.hpp"
#include "cs/http/request.hpp"
#include "cs/reader.hpp"


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {


	// -- H T T P  N A M E S P A C E ------------------------------------------

	namespace http {


		// -- P A R S E R -----------------------------------------------------

		class parser final {


			private:

				// -- private types -------------------------------------------

				/* self type */
				using ___self = cs::http::parser;


			public:

				// -- public lifecycle ----------------------------------------

				/* default constructor */
				parser(void) noexcept;

				/* copy constructor */
				parser(const ___self&) noexcept = default;

				/* move constructor */
				parser(___self&&) noexcept = default;

				/* destructor */
				~parser(void) noexcept = default;


				// -- public assignment operators -----------------------------

				/* copy assignment operator */
				auto operator=(const ___self&) noexcept -> ___self& = default;

				/* move assignment operator */
				auto operator=(___self&&) noexcept -> ___self& = default;


				// -- public methods ------------------------------------------

				/* parse */
				template <auto ___size>
				auto parse(cs::http::request& ___rq, const cs::reader<___size>& ___rd) -> void {

					// set data
					_request = &___rq;
					_data = ___rd.data();
					_end = _data + ___rd.size();

					// call parse
					_parse();
				}


				// -- public accessors ----------------------------------------

				/* complete */
				auto complete(void) const noexcept -> bool;

				/* clear */
				auto clear(void) noexcept -> void;


			private:

				// -- private constants ---------------------------------------

				/* char type */
				enum ___char : unsigned {

					// 0-31, 127
					CNTRL,
					// '\r'
					RETURN,
					// '\n'
					NEWLINE,
					// \t, ' '
					SPACE,
					// 0-9
					NUMBER,
					// a-z
					LOWER,
					// A-Z
					UPPER,
					// ':'
					COLON,
					// '-'
					HYPHEN,
					// other + extended ascii
					OTHER,

					// end of buffer (manual marker)
					EOBUFF, // not generate by char table, but used in state table

					MAX_CHARTYPE
				};

				/* state type */
				enum ___state : unsigned {

					// method
					START,
					IN_METHOD,

					// uri
					WAIT_URI,
					IN_URI,

					// version
					WAIT_VERSION,
					IN_VERSION,

					// headers
					WAIT_HEADER,
					IN_HEADER,

					// separator
					WAIT_SEPARATOR,
					IN_SEPARATOR,

					// field
					WAIT_FIELD,
					IN_FIELD,

					// crlf
					WAIT_RETURN, // when whitespace before cr
					IN_RETURN,  // after cr
					WAIT_NEWLINE, // when second crlf (end of request, or before body)

					// body
					WAIT_BODY,
					IN_BODY,

					// complete
					COMPLETE,

					// end / error
					ERROR,

					// max
					MAX_STATE
				};

				/* action type */
				enum ___action : unsigned {

					// increment
					INCREMENT,

					// new
					NEW_HEADER,

					// complete
					COMPLETE_METHOD,
					COMPLETE_URI,
					COMPLETE_VERSION,
					COMPLETE_HEADER,
					COMPLETE_FIELD,

					// other
					SKIP,

					// error
					HANDLE_ERROR,

					// max
					MAX_ACTION
				};


				// -- private methods -----------------------------------------

				/* parse */
				auto _parse(void) -> void;


				/* increment */
				auto increment(void) noexcept -> void;

				/* new header */
				auto new_header(void) -> void;

				/* complete method */
				auto complete_method(void) -> void;

				/* complete uri */
				auto complete_uri(void) -> void;

				/* complete version */
				auto complete_version(void) -> void;

				/* complete header */
				auto complete_header(void) -> void;

				/* complete field */
				auto complete_field(void) -> void;

				/* skip */
				inline auto skip(void) noexcept -> void {}

				/* error */
				auto error(void) noexcept -> void;


				// -- private structs -----------------------------------------

				/* transition */
				struct ___transition final {
					 ___state _state;
					___action _action;
				};


				// -- private members -----------------------------------------

				/* transition */
				const ___transition* _transition;

				/* request */
				cs::http::request* _request;

				/* data */
				const char* _data;

				/* end */
				const char* _end;

				/* count */
				cs::size_t _count;


				// -- private types -------------------------------------------

				/* procedure type */
				using ___procedure = auto (parser::*)(void) -> void;


				// -- private static members ----------------------------------

				/* action table */
				static constexpr ___procedure _actions[MAX_ACTION] {
					&___self::increment,
					&___self::new_header,
					&___self::complete_method,
					&___self::complete_uri,
					&___self::complete_version,
					&___self::complete_header,
					&___self::complete_field,
					&___self::skip,
					&___self::error,
				};

				/* state machine table */
				static constexpr ___transition _machine[MAX_STATE][MAX_CHARTYPE] {

					// START
					// expect first method character
					{
						// CNTRL
						{ ERROR, HANDLE_ERROR },
						// RETURN
						{ START, SKIP }, // error tolerant
						// NEWLINE
						{ START, SKIP }, // error tolerant
						// SPACE
						{ START, SKIP }, // error tolerant
						// NUMBER
						{ ERROR, HANDLE_ERROR },
						// LOWER
						{ ERROR, HANDLE_ERROR }, // method case sensitive
						// UPPER
						{ IN_METHOD, INCREMENT },
						// COLON
						{ ERROR, HANDLE_ERROR },
						// HYPHEN
						{ ERROR, HANDLE_ERROR },
						// OTHER
						{ ERROR, HANDLE_ERROR },
						// EOBUFF
						{ START, SKIP },
					},

					// IN_METHOD
					// expect method characters or space
					{
						// CNTRL
						{ ERROR, HANDLE_ERROR },
						// RETURN
						{ ERROR, HANDLE_ERROR },
						// NEWLINE
						{ ERROR, HANDLE_ERROR },
						// SPACE
						{ WAIT_URI, COMPLETE_METHOD },
						// NUMBER
						{ ERROR, HANDLE_ERROR },
						// LOWER
						{ ERROR, HANDLE_ERROR },
						// UPPER
						{ IN_METHOD, INCREMENT },
						// COLON
						{ ERROR, HANDLE_ERROR },
						// HYPHEN
						{ ERROR, HANDLE_ERROR },
						// OTHER
						{ ERROR, HANDLE_ERROR },
						// EOBUFF
						{ IN_METHOD, COMPLETE_METHOD },
					},

					// WAIT_URI
					// expect first uri character
					// or space (error tolerant)
					{
						// CNTRL
						{ ERROR, HANDLE_ERROR },
						// RETURN
						{ ERROR, HANDLE_ERROR },
						// NEWLINE
						{ ERROR, HANDLE_ERROR },
						// SPACE
						{ WAIT_URI, SKIP }, // error tolerant
						// NUMBER
						{ IN_URI, INCREMENT },
						// LOWER
						{ IN_URI, INCREMENT },
						// UPPER
						{ IN_URI, INCREMENT },
						// COLON
						{ IN_URI, INCREMENT },
						// HYPHEN
						{ IN_URI, INCREMENT },
						// OTHER
						{ IN_URI, INCREMENT },
						// EOBUFF
						{ WAIT_URI, SKIP },
					},

					// IN_URI
					// expect uri characters
					// or crlf (missing version)
					// or space
					{
						// CNTRL
						{ ERROR, HANDLE_ERROR },
						// RETURN
						{ IN_RETURN, COMPLETE_URI },
						// NEWLINE
						{ ERROR, HANDLE_ERROR },
						// SPACE
						{ WAIT_VERSION, COMPLETE_URI },
						// NUMBER
						{ IN_URI, INCREMENT },
						// LOWER
						{ IN_URI, INCREMENT },
						// UPPER
						{ IN_URI, INCREMENT },
						// COLON
						{ IN_URI, INCREMENT },
						// HYPHEN
						{ IN_URI, INCREMENT },
						// OTHER
						{ IN_URI, INCREMENT },
						// EOBUFF
						{ IN_URI, COMPLETE_URI },
					},

					// WAIT_VERSION
					// expect first version character
					// or crlf (missing version)
					// or space (error tolerant)
					{
						// CNTRL
						{ ERROR, HANDLE_ERROR },
						// RETURN
						{ IN_RETURN, SKIP },
						// NEWLINE
						{ ERROR, HANDLE_ERROR },
						// SPACE
						{ WAIT_VERSION, SKIP }, // error tolerant
						// NUMBER
						{ IN_VERSION, INCREMENT },
						// LOWER
						{ IN_VERSION, INCREMENT },
						// UPPER
						{ IN_VERSION, INCREMENT },
						// COLON
						{ IN_VERSION, INCREMENT },
						// HYPHEN
						{ IN_VERSION, INCREMENT },
						// OTHER
						{ IN_VERSION, INCREMENT },
						// EOBUFF
						{ WAIT_VERSION, SKIP },
					},

					// IN_VERSION
					// expect version characters
					// or crlf
					// or space (error tolerant)
					{
						// CNTRL
						{ ERROR, HANDLE_ERROR },
						// RETURN
						{ IN_RETURN, COMPLETE_VERSION },
						// NEWLINE
						{ ERROR, HANDLE_ERROR },
						// SPACE
						{ WAIT_RETURN, COMPLETE_VERSION },
						// NUMBER
						{ IN_VERSION, INCREMENT },
						// LOWER
						{ IN_VERSION, INCREMENT },
						// UPPER
						{ IN_VERSION, INCREMENT },
						// COLON
						{ IN_VERSION, INCREMENT },
						// HYPHEN
						{ IN_VERSION, INCREMENT },
						// OTHER
						{ IN_VERSION, INCREMENT },
						// EOBUFF
						{ IN_VERSION, COMPLETE_VERSION },
					},

					// WAIT_HEADER
					// expect first header character
					// or space (error tolerant)
					// or cr (second crlf before body)
					{
						// CNTRL
						{ ERROR, HANDLE_ERROR },
						// RETURN
						{ WAIT_NEWLINE, SKIP }, // second crlf before body
						// NEWLINE
						{ ERROR, HANDLE_ERROR },
						// SPACE
						{ WAIT_HEADER, SKIP }, // error tolerant

						// maybe restrict header character types...
						// A-Z, a-z, 0–9, hyphen ( - ), or underscore ( _ ).

						// NUMBER
						{ IN_HEADER, NEW_HEADER },
						// LOWER
						{ IN_HEADER, NEW_HEADER },
						// UPPER
						{ IN_HEADER, NEW_HEADER },
						// COLON
						{ ERROR, HANDLE_ERROR },
						// HYPHEN
						{ IN_HEADER, NEW_HEADER },
						// OTHER
						{ IN_HEADER, NEW_HEADER },
						// EOBUFF
						{ WAIT_HEADER, SKIP },
					},

					// IN_HEADER
					// expect header characters
					// or : (separator)
					// or space (error tolerant)
					{
						// CNTRL
						{ ERROR, HANDLE_ERROR },
						// RETURN
						{ ERROR, HANDLE_ERROR },
						// NEWLINE
						{ ERROR, HANDLE_ERROR },
						// SPACE
						{ WAIT_SEPARATOR, COMPLETE_HEADER }, // error tolerant

						// maybe restrict header character types...
						// A-Z, a-z, 0–9, hyphen ( - ), or underscore ( _ ).

						// NUMBER
						{ IN_HEADER, INCREMENT },
						// LOWER
						{ IN_HEADER, INCREMENT },
						// UPPER
						{ IN_HEADER, INCREMENT },
						// COLON
						{ IN_SEPARATOR, COMPLETE_HEADER },
						// HYPHEN
						{ IN_HEADER, INCREMENT },
						// OTHER
						{ IN_HEADER, INCREMENT },
						// EOBUFF
						{ IN_HEADER, COMPLETE_HEADER },
					},

					// WAIT_SEPARATOR
					// expect separator
					// or space (wait separator is tolerant state)
					{
						// CNTRL
						{ ERROR, HANDLE_ERROR },
						// RETURN
						{ ERROR, HANDLE_ERROR },
						// NEWLINE
						{ ERROR, HANDLE_ERROR },
						// SPACE
						{ WAIT_SEPARATOR, SKIP }, // error tolerant
						// NUMBER
						{ ERROR, HANDLE_ERROR },
						// LOWER
						{ ERROR, HANDLE_ERROR },
						// UPPER
						{ ERROR, HANDLE_ERROR },
						// COLON
						{ IN_SEPARATOR, SKIP },
						// HYPHEN
						{ ERROR, HANDLE_ERROR },
						// OTHER
						{ ERROR, HANDLE_ERROR },
						// EOBUFF
						{ WAIT_SEPARATOR, SKIP },
					},

					// IN_SEPARATOR
					// expect first field character
					// or space
					// or crlf (empty value)
					{
						// CNTRL
						{ ERROR, HANDLE_ERROR },
						// RETURN
						{ IN_RETURN, SKIP }, // handle this case (empty)
						// NEWLINE
						{ ERROR, HANDLE_ERROR },
						// SPACE
						{ WAIT_FIELD, SKIP },
						// NUMBER
						{ IN_FIELD, INCREMENT },
						// LOWER
						{ IN_FIELD, INCREMENT },
						// UPPER
						{ IN_FIELD, INCREMENT },
						// COLON
						{ IN_FIELD, INCREMENT }, // maybe error
						// HYPHEN
						{ IN_FIELD, INCREMENT },
						// OTHER
						{ IN_FIELD, INCREMENT },
						// EOBUFF
						{ IN_SEPARATOR, SKIP },
					},

					// WAIT_FIELD
					// expect first field character
					// or space
					// or crlf (empty value)
					{
						// CNTRL
						{ ERROR, HANDLE_ERROR },
						// RETURN
						{ IN_RETURN, SKIP }, // handle this case (empty)
						// NEWLINE
						{ ERROR, HANDLE_ERROR },
						// SPACE
						{ WAIT_FIELD, SKIP },
						// NUMBER
						{ IN_FIELD, INCREMENT },
						// LOWER
						{ IN_FIELD, INCREMENT },
						// UPPER
						{ IN_FIELD, INCREMENT },
						// COLON
						{ IN_FIELD, INCREMENT }, // maybe error
						// HYPHEN
						{ IN_FIELD, INCREMENT },
						// OTHER
						{ IN_FIELD, INCREMENT },
						// EOBUFF
						{ WAIT_FIELD, SKIP },
					},

					// IN_FIELD
					// expect field characters
					// or crlf (end of field, space are allowed in field)
					{
						// CNTRL
						{ ERROR, HANDLE_ERROR },
						// RETURN
						{ IN_RETURN, COMPLETE_FIELD },
						// NEWLINE
						{ ERROR, HANDLE_ERROR },
						// SPACE
						{ IN_FIELD, INCREMENT },
						// NUMBER
						{ IN_FIELD, INCREMENT },
						// LOWER
						{ IN_FIELD, INCREMENT },
						// UPPER
						{ IN_FIELD, INCREMENT },
						// COLON
						{ IN_FIELD, INCREMENT },
						// HYPHEN
						{ IN_FIELD, INCREMENT },
						// OTHER
						{ IN_FIELD, INCREMENT },
						// EOBUFF
						{ IN_FIELD, COMPLETE_FIELD },
					},

					// WAIT_RETURN (when whitespace before cr)
					// expect cr
					{
						// CNTRL
						{ ERROR, HANDLE_ERROR },
						// RETURN
						{ IN_RETURN, SKIP },
						// NEWLINE
						{ ERROR, HANDLE_ERROR },
						// SPACE
						{ WAIT_RETURN, SKIP },
						// NUMBER
						{ ERROR, HANDLE_ERROR },
						// LOWER
						{ ERROR, HANDLE_ERROR },
						// UPPER
						{ ERROR, HANDLE_ERROR },
						// COLON
						{ ERROR, HANDLE_ERROR },
						// HYPHEN
						{ ERROR, HANDLE_ERROR },
						// OTHER
						{ ERROR, HANDLE_ERROR },
						// EOBUFF
						{ WAIT_RETURN, SKIP },
					},

					// IN_RETURN (after cr)
					// expect lf
					{
						// CNTRL
						{ ERROR, HANDLE_ERROR },
						// RETURN
						{ ERROR, HANDLE_ERROR },
						// NEWLINE
						{ WAIT_HEADER, SKIP },
						// SPACE
						{ ERROR, HANDLE_ERROR },
						// NUMBER
						{ ERROR, HANDLE_ERROR },
						// LOWER
						{ ERROR, HANDLE_ERROR },
						// UPPER
						{ ERROR, HANDLE_ERROR },
						// COLON
						{ ERROR, HANDLE_ERROR },
						// HYPHEN
						{ ERROR, HANDLE_ERROR },
						// OTHER
						{ ERROR, HANDLE_ERROR },
						// EOBUFF
						{ IN_RETURN, SKIP },
					},

					// WAIT_NEWLINE (when second crlf)
					// expect lf
					{
						// CNTRL
						{ ERROR, HANDLE_ERROR },
						// RETURN
						{ ERROR, HANDLE_ERROR },
						// NEWLINE
						{ COMPLETE, SKIP }, // wait body ?
						// SPACE
						{ ERROR, HANDLE_ERROR },
						// NUMBER
						{ ERROR, HANDLE_ERROR },
						// LOWER
						{ ERROR, HANDLE_ERROR },
						// UPPER
						{ ERROR, HANDLE_ERROR },
						// COLON
						{ ERROR, HANDLE_ERROR },
						// HYPHEN
						{ ERROR, HANDLE_ERROR },
						// OTHER
						{ ERROR, HANDLE_ERROR },
						// EOBUFF
						{ WAIT_NEWLINE, SKIP },
					},

					// WAIT_BODY
					// expect body characters (all)
					{
						// CNTRL
						{ IN_BODY, INCREMENT },
						// RETURN
						{ IN_BODY, INCREMENT },
						// NEWLINE
						{ IN_BODY, INCREMENT },
						// SPACE
						{ IN_BODY, INCREMENT },
						// NUMBER
						{ IN_BODY, INCREMENT },
						// LOWER
						{ IN_BODY, INCREMENT },
						// UPPER
						{ IN_BODY, INCREMENT },
						// COLON
						{ IN_BODY, INCREMENT },
						// HYPHEN
						{ IN_BODY, INCREMENT },
						// OTHER
						{ IN_BODY, INCREMENT },
						// EOBUFF
						{ WAIT_BODY, SKIP },
					},

					// IN_BODY
					// expect body characters (all)
					{
						// CNTRL
						{ IN_BODY, INCREMENT },
						// RETURN
						{ IN_BODY, INCREMENT },
						// NEWLINE
						{ IN_BODY, INCREMENT },
						// SPACE
						{ IN_BODY, INCREMENT },
						// NUMBER
						{ IN_BODY, INCREMENT },
						// LOWER
						{ IN_BODY, INCREMENT },
						// UPPER
						{ IN_BODY, INCREMENT },
						// COLON
						{ IN_BODY, INCREMENT },
						// HYPHEN
						{ IN_BODY, INCREMENT },
						// OTHER
						{ IN_BODY, INCREMENT },
						// EOBUFF
						{ IN_BODY, SKIP },
					},

					// COMPLETE
					// expect end of buffer
					{
						// CNTRL
						{ ERROR, HANDLE_ERROR },
						// RETURN
						{ ERROR, HANDLE_ERROR },
						// NEWLINE
						{ ERROR, HANDLE_ERROR },
						// SPACE
						{ ERROR, HANDLE_ERROR },
						// NUMBER
						{ ERROR, HANDLE_ERROR },
						// LOWER
						{ ERROR, HANDLE_ERROR },
						// UPPER
						{ ERROR, HANDLE_ERROR },
						// COLON
						{ ERROR, HANDLE_ERROR },
						// HYPHEN
						{ ERROR, HANDLE_ERROR },
						// OTHER
						{ ERROR, HANDLE_ERROR },
						// EOBUFF
						{ COMPLETE, SKIP },
					},

					// ERROR
					// expect end of buffer
					{
						// CNTRL
						{ ERROR, HANDLE_ERROR },
						// RETURN
						{ ERROR, HANDLE_ERROR },
						// NEWLINE
						{ ERROR, HANDLE_ERROR },
						// SPACE
						{ ERROR, HANDLE_ERROR },
						// NUMBER
						{ ERROR, HANDLE_ERROR },
						// LOWER
						{ ERROR, HANDLE_ERROR },
						// UPPER
						{ ERROR, HANDLE_ERROR },
						// COLON
						{ ERROR, HANDLE_ERROR },
						// HYPHEN
						{ ERROR, HANDLE_ERROR },
						// OTHER
						{ ERROR, HANDLE_ERROR },
						// EOBUFF
						{ ERROR, SKIP },
					},
				};

				/* char table */
				static constexpr ___char _ctable[256] {

					// control 0x00 - 0x08
					CNTRL, CNTRL, CNTRL, CNTRL, CNTRL, CNTRL, CNTRL, CNTRL,
					CNTRL,

					// tab 0x09
					SPACE,

					// newline 0x0A
					NEWLINE,

					// control 0x0B - 0x0C
					CNTRL, CNTRL,

					// return 0x0D
					RETURN,

					// control 0x0E - 0x1F
					CNTRL, CNTRL, CNTRL, CNTRL, CNTRL, CNTRL, CNTRL, CNTRL,
					CNTRL, CNTRL, CNTRL, CNTRL, CNTRL, CNTRL, CNTRL, CNTRL,
					CNTRL, CNTRL,

					// space 0x20
					SPACE,

					// other 0x21 - 0x2F
					OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
					OTHER, OTHER, OTHER, OTHER,

					// colon 0x3A
					HYPHEN,

					// 0x3B - 0x40
					OTHER, OTHER,

					// digits 0x30 - 0x39
					NUMBER, NUMBER, NUMBER, NUMBER, NUMBER, NUMBER, NUMBER,
					NUMBER, NUMBER, NUMBER,

					// colon 0x3A
					COLON,

					// other 0x3B - 0x40
					OTHER, OTHER, OTHER, OTHER, OTHER,
					OTHER,

					// upper 0x41 - 0x5A
					UPPER, UPPER, UPPER, UPPER, UPPER, UPPER, UPPER, UPPER,
					UPPER, UPPER, UPPER, UPPER, UPPER, UPPER, UPPER, UPPER,
					UPPER, UPPER, UPPER, UPPER, UPPER, UPPER, UPPER, UPPER,
					UPPER, UPPER, OTHER,

					// other 0x5B - 0x60
					OTHER, OTHER, OTHER, OTHER, OTHER,

					// lower 0x61 - 0x7A
					LOWER, LOWER, LOWER, LOWER, LOWER, LOWER, LOWER, LOWER,
					LOWER, LOWER, LOWER, LOWER, LOWER, LOWER, LOWER, LOWER,
					LOWER, LOWER, LOWER, LOWER, LOWER, LOWER, LOWER, LOWER,
					LOWER, LOWER,

					// other 0x7B - 0x7E
					OTHER, OTHER, OTHER, OTHER,

					// delete 0x7F
					CNTRL,

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
					OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
				};

		}; // class parser

	} // namespace http

} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_HTTP_PARSER_HPP___
