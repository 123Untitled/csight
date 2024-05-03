#include "cs/system/input.hpp"
#include "cs/network/dispatch.hpp"

#include <unistd.h>
#include <fcntl.h>


// -- public lifecycle --------------------------------------------------------

/* default constructor */
cs::input::input(void) {

	// set non-blocking
	auto flags = ::fcntl(STDIN_FILENO, F_GETFL, 0);

	if (flags < 0)
		throw cs::runtime_error("fcntl");

	flags |= O_NONBLOCK;

	if (::fcntl(STDIN_FILENO, F_SETFL, flags) < 0)
		throw cs::runtime_error("fcntl");

	// set raw mode
	struct termios raw;

	if (::tcgetattr(STDIN_FILENO, &_term) < 0)
		throw cs::runtime_error("tcgetattr");

	raw = _term;

	raw.c_lflag &= static_cast<tcflag_t>(~(ECHO | ICANON));

	if (::tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) < 0)
		throw cs::runtime_error("tcsetattr");

}

/* destructor */
cs::input::~input(void) noexcept {

	// restore terminal attributes
	if (::tcsetattr(STDIN_FILENO, TCSAFLUSH, &_term) < 0)
		::write(STDERR_FILENO, "error: tcsetattr\n", 17);

	else
		::write(STDOUT_FILENO, "restore terminal attributes\n", 28);

}

// -- public overriden methods ------------------------------------------------

/* read */
auto cs::input::read(void) -> void {

	// buffer
	char ___ch;

	// read
	auto n = ::read(STDIN_FILENO, &___ch, 1U);

	if (n < 0)
		throw std::runtime_error("read");

	if (n == 0)
		throw std::runtime_error("EOF");

	switch (___ch) {

		case 'q':
			// fallthrough
		case '\x1b':
			cs::dispatch::stop();
			break;
		default:
			break;
	}
}

/* send */
auto cs::input::send(void) -> void {

}

/* descriptor */
auto cs::input::descriptor(void) const noexcept -> int {
	return STDIN_FILENO;
}
