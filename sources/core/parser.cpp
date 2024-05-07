#include "cs/config.hpp"
#include "cs/core/parser.hpp"
#include "cs/type_traits/move.hpp"
#include <unistd.h>
#include "cs/system/access.hpp"

#include <iostream>

#if ___cs_requirements


// -- public lifecycle --------------------------------------------------------

/* default constructor */
cs::parser::parser(void) noexcept
: _transition{&_machine[START][EOBUFF]}, _back{START}, _data{nullptr}, _end{nullptr},
  _map{nullptr}, _file{}, _issue{}, _count{0U} {
}


// -- private modifiers -------------------------------------------------------

/* parse */
auto cs::parser::_parse(void) -> void {

	// loop over data
	for (;_data != _end; ++_data) {

		// get next transition
		_transition = &(_machine[_transition->_state])
			[_ctable[static_cast<unsigned char>(*_data)]];

		// execute action
		(this->*_actions[_transition->_action])();
	}

	// end of buffer marker
	_transition = &(_machine[_transition->_state][EOBUFF]);

	// execute action
	(this->*_actions[_transition->_action])();

}



/* reset */
auto cs::parser::reset(void) noexcept -> void {
	_issue.clear();
	_file.clear();
	_count = 0U;
	std::cout << "RESET" << std::endl;
}

/* increment */
auto cs::parser::increment(void) noexcept -> void {
	++_count;
}

/* complete issue */
auto cs::parser::complete_issue(void) -> void {
	std::cout << "COMPLETE ISSUE" << std::endl;
	_map->insert(cs::move(_file), cs::move(_issue));
}

/* complete file */
auto cs::parser::complete_file(void) -> void {

	// append buffer to file
	_file.append(cs::string_view{_data - _count, _count});

	// check file existence
	if (not cs::access<F_OK>(_file)) {

		std::cout << "\x1b[33mbad [\x1b[0m" << _file << "\x1b[33m]\x1b[0m" << std::endl;
		_transition = &(_machine[IN_FILE][NO_ACTION]);
		_count = 1U;
	}
	else {
		std::cout << "\x1b[32mGOOD [\x1b[0m" << _file << "\x1b[32m]\x1b[0m" << std::endl;
		_count = 0U;
	}

}

/* save file */
auto cs::parser::save_file(void) -> void {
	_file.append(cs::string_view{_data - _count, _count});
	_count = 0U;
}


/* save diagnostic */
auto cs::parser::save_diagnostic(void) -> void {
	std::cout << "EOBUFF DIAGNOSTIC" << std::endl;
	_issue.message().append(cs::string_view{_data - _count, _count});
	_count = 0U;
}


/* complete diagnostic */
auto cs::parser::complete_diagnostic(void) -> void {
	_issue.message().append(cs::string_view{_data - _count, _count});
	std::cout << "COMPLETE DIAGNOSTIC" << 
	" \x1b[33m[\x1b[0m" << _issue.message() << "\x1b[33m]\x1b[0m" << std::endl;
	_count = 0U;
	_map->insert(cs::move(_file), cs::move(_issue));
	//_file.clear(); // clear if file already exists in map (fix now map clear file if already exists)
}

/* complete status */
auto cs::parser::complete_status(void) -> void {
	std::cout << "\x1b[36mcomplete status [\x1b[0m" << cs::string_view{_data - _count, _count}
	<< "\x1b[36m]\x1b[0m" << std::endl;
	_count = 0U;
}



/* compute line */
auto cs::parser::compute_line(void) noexcept -> void {
	_count = 0U; // to be moved in another action (goal is to reset count after valid file)

	auto& ___ln = _issue.line();

	___ln *= 10U;
	___ln += static_cast<size_type>(*_data - '0');
	std::cout << "\x1b[36mcompute line [\x1b[0m" << ___ln << "\x1b[36m]\x1b[0m" << std::endl;
}

/* compute column */
auto cs::parser::compute_column(void) noexcept -> void {
	_count = 0U; // to be moved in another action (goal is to reset count after valid file)

	auto& ___col = _issue.column();

	___col *= 10U;
	___col += static_cast<size_type>(*_data - '0');
	std::cout << "\x1b[36mcompute column [\x1b[0m" << ___col << "\x1b[36m]\x1b[0m" << std::endl;
}

// -- escape sequences --------------------------------------------------------

/* handle escape */
auto cs::parser::handle_escape(void) noexcept -> void {
	std::cout << "HANDLE ESCAPE" << std::endl;
	// complete buffer
	(this->*_actions[_machine[_transition->_state][EOBUFF]._action])();
	// save current state
	_back = _transition->_state;
	// set new state
	_transition = &(_machine[WAIT_ESCAPE][EOBUFF]);
}

/* complete escape */
auto cs::parser::complete_escape(void) noexcept -> void {

	std::cout << "COMPLETE ESCAPE" << std::endl;
	// info: when escape sequence is valid, terminated by 'm'

	// back to previous state
	_transition = &(_machine[_back][EOBUFF]);
}



auto in_word_set(const char* data, const unsigned int len) noexcept -> const char* {

	enum : unsigned {
		MIN_LEN  = 4,
		MAX_LEN  = 11,
	};

	static constexpr unsigned char _table[] {
		12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
		12,  0,  0, 12, 12,  5, 12, 12, 12, 12,
		 0, 12, 12, 12, 12, 12, 12, 12, 12,  0,
		12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
		12, 12, 12, 12, 12, 12
	};

	static const char* _map[] {
		"", "", "", "",
		"note",
		"error",
		"",
		"warning",
		"",
		"info",
		"",
		"fatal error"
	};

	if (len > MAX_LEN || len < MIN_LEN)
		return 0;

	const unsigned key = len + _table[(unsigned char)data[0U]];

	if (key > MAX_LEN)
		return 0;

	const char* s = _map[key];

	if (*data == *s && !__builtin_strcmp(data + 1, s + 1))
		return s;

	return 0;
}







#endif // ___cs_requirements
