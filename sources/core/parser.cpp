#include "cs/config.hpp"
#include "cs/core/parser.hpp"
#include <unistd.h>

#include <iostream>

#if ___cs_requirements


// -- private lifecycle -------------------------------------------------------

/* descriptor constructor */
cs::parser::parser(cs::list<cs::issue>& ___lst, const int ___fd) noexcept
: _list{___lst}, _fd{___fd}, /* _buffer{} */ _data{nullptr},
	_issue{} {
}


// -- public static methods ---------------------------------------------------

/* parse */
auto cs::parser::parse(const int ___fd) -> cs::list<cs::issue> {
	cs::list<cs::issue> ___lst;
	parser{___lst, ___fd}._parse();
	return ___lst;
}


// -- private modifiers -------------------------------------------------------

/* parse */
auto cs::parser::_parse(void) -> void {

	___transition ___trs {
		START,
		NO_ACTION,
		nullptr
	};

	while (((((((true))))))) {

		auto ___rd = ::read(_fd, _buffer, BUFFER_SIZE);

		if (___rd == -1)
			throw -1;

		if (___rd == 0)
			return;

		_buffer[___rd] = '\0';


		_data = _buffer;

		while (*_data != '\0') {

			___trs = _transition(___trs._state, _chartype(*_data));

			if (___trs._msg != nullptr) {
				std::cerr << "error: " << ___trs._msg << std::endl;
				break;
			}
			_action(*this, ___trs._action);

			++_data;
		}
	}

}



/* reset */
auto cs::parser::reset(void) noexcept -> void {
	_issue.clear();
}

/* complete issue */
auto cs::parser::complete_issue(void) noexcept -> void {
	//std::cout << "\x1b[1;31m" << "complete issue" << "\x1b[0m" << std::endl;
	//_issue.print();
	_list.emplace_back(static_cast<cs::issue&&>(_issue));
	_issue.clear(); // to be removed
}


/* add path */
auto cs::parser::add_path(void) noexcept -> void {
}

/* add line */
auto cs::parser::add_line(void) noexcept -> void {
}

/* add column */
auto cs::parser::add_column(void) noexcept -> void {
}

/* add diagnostic */
auto cs::parser::add_diagnostic(void) noexcept -> void {
}


/* count path */
auto cs::parser::count_path(void) noexcept -> void {
	_issue.path().push_back(*_data);
}

/* compute line */
auto cs::parser::compute_line(void) noexcept -> void {

	auto& ___ln = _issue.line();

	___ln *= 10U;
	___ln += static_cast<___size>(*_data - '0');
}

/* compute column */
auto cs::parser::compute_column(void) noexcept -> void {

	auto& ___col = _issue.column();

	___col *= 10U;
	___col += static_cast<___size>(*_data - '0');
}

/* count diagnostic */
auto cs::parser::count_diagnostic(void) noexcept -> void {
	_issue.message().push_back(*_data);
}


/* throw exception */
auto cs::parser::throw_exception(void) noexcept -> void {
	std::cout << "error" << std::endl;
}

/* no action */
auto cs::parser::no_action(void) noexcept -> void {
	//std::cout << "no action" << std::endl;
}



/* action */
auto cs::parser::_action(___self& ___obj, const ___action_type ___act) noexcept -> void {

	using ___action_proto = auto (parser::*)(void) noexcept -> void;

	static constexpr ___action_proto _table[MAX_ACTION] = {

		&___self::reset,
		&___self::complete_issue,

		&___self::add_path,
		&___self::add_line,
		&___self::add_column,
		&___self::add_diagnostic,

		&___self::count_path,
		&___self::compute_line,
		&___self::compute_column,
		&___self::count_diagnostic,

		&___self::throw_exception,
		&___self::no_action,
	};

	(___obj.*_table[___act])();
}

// -- private static methods --------------------------------------------------

/* transition */
auto cs::parser::_transition(const ___state_type ___st,
						const ___char_type  ___ch) noexcept -> ___transition {

	static constexpr ___transition _table[MAX_STATE][MAX_CHAR_TYPE] = {

		// START
		{
			// CTRL
			{ SKIP_LINE, RESET, "unexpected control character" },

			// RETURN
			{ IN_RETURN, NO_ACTION, nullptr },
			// NEWLINE
			{ START, RESET, nullptr }, // maybe not reset
			// SPACE
			{ IN_PATH, COUNT_PATH, nullptr },

			// NUMBER
			{ IN_PATH, COUNT_PATH, nullptr },
			// LETTER
			{ IN_PATH, COUNT_PATH, nullptr },

			// COLON
			{ SKIP_LINE, RESET, nullptr }, // no path ?
			// DOT
			{ IN_PATH, COUNT_PATH, nullptr },
			// CIRCUMFLEX
			{ IN_PATH, COUNT_PATH, nullptr },
			// TILDE
			{ IN_PATH, COUNT_PATH, nullptr },

			// OTHER
			{ IN_PATH, COUNT_PATH, nullptr },

			// EXTEND
			{ IN_PATH, COUNT_PATH, nullptr },

		},

		// IN_RETURN
		{
			// CTRL
			{ SKIP_LINE, RESET, "unexpected control character after return" },

			// RETURN
			{ IN_RETURN, NO_ACTION, nullptr },
			// NEWLINE
			{ START, COMPLETE_ISSUE, nullptr },
			// SPACE
			{ SKIP_LINE, RESET, "unexpected space after return" },

			// NUMBER
			{ SKIP_LINE, RESET, "unexpected digit after return" },
			// LETTER
			{ SKIP_LINE, RESET, "unexpected letter after return" },

			// COLON
			{ SKIP_LINE, RESET, "unexpected colon after return" },
			// DOT
			{ SKIP_LINE, RESET, "unexpected dot after return" },
			// CIRCUMFLEX
			{ SKIP_LINE, RESET, "unexpected circumflex after return" },
			// TILDE
			{ SKIP_LINE, RESET, "unexpected tilde after return" },

			// OTHER
			{ SKIP_LINE, RESET, "unexpected character after return" },

			// EXTEND
			{ SKIP_LINE, RESET, "unexpected extended character after return" },

		},

		// IN_PATH
		{
			// CTRL
			{ SKIP_LINE, RESET, "unexpected control character in path" },

			// RETURN
			{ SKIP_LINE, RESET, nullptr },
			// NEWLINE
			{ START, RESET, nullptr },
			// SPACE
			{ IN_PATH, COUNT_PATH, nullptr },

			// NUMBER
			{ IN_PATH, COUNT_PATH, nullptr },
			// LETTER
			{ IN_PATH, COUNT_PATH, nullptr },

			// COLON
			{ IN_LINE, ADD_PATH, nullptr },
			// DOT
			{ IN_PATH, COUNT_PATH, nullptr },
			// CIRCUMFLEX
			{ IN_PATH, COUNT_PATH, nullptr },
			// TILDE
			{ IN_PATH, COUNT_PATH, nullptr },

			// OTHER
			{ IN_PATH, COUNT_PATH, nullptr },

			// EXTEND
			{ IN_PATH, COUNT_PATH, nullptr },

		},

		// IN_LINE
		{
			// CTRL
			{ SKIP_LINE, RESET, "unexpected control character in line" },

			// RETURN
			{ SKIP_LINE, RESET, nullptr },
			// NEWLINE
			{ START, RESET, nullptr },
			// SPACE
			{ SKIP_LINE, RESET, nullptr },

			// NUMBER
			{ IN_LINE, COMPUTE_LINE, nullptr },
			// LETTER
			{ SKIP_LINE, RESET, nullptr },

			// COLON
			{ IN_COLUMN, ADD_LINE, nullptr },
			// DOT
			{ SKIP_LINE, RESET, nullptr },
			// CIRCUMFLEX
			{ SKIP_LINE, RESET, nullptr },
			// TILDE
			{ SKIP_LINE, RESET, nullptr },

			// OTHER
			{ SKIP_LINE, RESET, nullptr },

			// EXTEND
			{ SKIP_LINE, RESET, nullptr },

		},

		// IN_COLUMN
		{
			// CTRL
			{ SKIP_LINE, RESET, "unexpected control character in column" },

			// RETURN
			{ SKIP_LINE, RESET, nullptr },
			// NEWLINE
			{ START, RESET, nullptr },
			// SPACE
			{ SKIP_LINE, RESET, nullptr },

			// NUMBER
			{ IN_COLUMN, COMPUTE_COLUMN, nullptr },
			// LETTER
			{ SKIP_LINE, RESET, nullptr },

			// COLON
			{ IN_DIAGNOSTIC, ADD_COLUMN, nullptr },
			// DOT
			{ SKIP_LINE, RESET, nullptr },
			// CIRCUMFLEX
			{ SKIP_LINE, RESET, nullptr },
			// TILDE
			{ SKIP_LINE, RESET, nullptr },

			// OTHER
			{ SKIP_LINE, RESET, nullptr },

			// EXTEND
			{ SKIP_LINE, RESET, nullptr },

		},

		// IN_DIAGNOSTIC
		{
			// CTRL
			{ UNDEFINED, THROW_EXCEPTION, "unexpected control character in diagnostic" },

			// RETURN
			{ IN_RETURN, ADD_DIAGNOSTIC, nullptr },
			// NEWLINE
			{ START, COMPLETE_ISSUE, nullptr },
			// SPACE
			{ IN_DIAGNOSTIC, COUNT_DIAGNOSTIC, nullptr },

			// NUMBER
			{ IN_DIAGNOSTIC, COUNT_DIAGNOSTIC, nullptr },
			// LETTER
			{ IN_DIAGNOSTIC, COUNT_DIAGNOSTIC, nullptr },

			// COLON
			{ IN_DIAGNOSTIC, COUNT_DIAGNOSTIC, nullptr },
			// DOT
			{ IN_DIAGNOSTIC, COUNT_DIAGNOSTIC, nullptr },
			// CIRCUMFLEX
			{ IN_DIAGNOSTIC, COUNT_DIAGNOSTIC, nullptr },
			// TILDE
			{ IN_DIAGNOSTIC, COUNT_DIAGNOSTIC, nullptr },

			// OTHER
			{ IN_DIAGNOSTIC, COUNT_DIAGNOSTIC, nullptr },

			// EXTEND
			{ IN_DIAGNOSTIC, COUNT_DIAGNOSTIC, nullptr },
		},

		// SKIP_LINE
		{
			// CTRL
			{ SKIP_LINE, NO_ACTION, nullptr },

			// RETURN
			{ SKIP_LINE, NO_ACTION, nullptr },
			// NEWLINE
			{ START, NO_ACTION, nullptr },
			// SPACE
			{ SKIP_LINE, NO_ACTION, nullptr },

			// NUMBER
			{ SKIP_LINE, NO_ACTION, nullptr },
			// LETTER
			{ SKIP_LINE, NO_ACTION, nullptr },

			// COLON
			{ SKIP_LINE, NO_ACTION, nullptr },
			// DOT
			{ SKIP_LINE, NO_ACTION, nullptr },
			// CIRCUMFLEX
			{ SKIP_LINE, NO_ACTION, nullptr },
			// TILDE
			{ SKIP_LINE, NO_ACTION, nullptr },

			// OTHER
			{ SKIP_LINE, NO_ACTION, nullptr },

			// EXTEND
			{ SKIP_LINE, NO_ACTION, nullptr },

		},

	};

	return _table[___st][___ch];
}


/* char type */
auto cs::parser::_chartype(const char ___ch) noexcept -> ___char_type {

	static constexpr ___char_type _table[256] = {

		CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL,
		CTRL, CTRL,
		NEWLINE,
		CTRL, CTRL,
		RETURN,
		CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL,
		CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL,
		CTRL, CTRL,
		SPACE,
		OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
		OTHER, OTHER, OTHER, OTHER, OTHER,
		DOT,
		OTHER,
		NUMBER, NUMBER, NUMBER, NUMBER, NUMBER, NUMBER, NUMBER, NUMBER,
		NUMBER, NUMBER,
		COLON,
		OTHER, OTHER, OTHER, OTHER, OTHER, OTHER,
		LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,
		LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,
		LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,
		LETTER, LETTER,
		OTHER, OTHER, OTHER,
		CIRCUMFLEX,
		OTHER, OTHER,
		LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,
		LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,
		LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,
		LETTER, LETTER,
		OTHER, OTHER, OTHER,
		TILDE,
		CTRL,
		EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND,
		EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND,
		EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND,
		EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND,
		EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND,
		EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND,
		EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND,
		EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND,
		EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND,
		EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND,
		EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND,
		EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND,
		EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND,
		EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND,
		EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND,
		EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND, EXTEND,
	};

	return _table[static_cast<unsigned char>(___ch)];
}

#endif // ___cs_requirements
