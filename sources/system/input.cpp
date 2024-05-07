#include "cs/system/input.hpp"
#include "cs/network/dispatch.hpp"
#include "cs/hint.hpp"
#include "cs/reader.hpp"
#include "cs/core/parser.hpp"
#include "cs/network/server.hpp"
//#include "cs/system/open.hpp"

#include <unistd.h>
#include <fcntl.h>


// -- public lifecycle --------------------------------------------------------

/* default constructor */
cs::input::input(void)
: _stdin{STDIN_FILENO},
  _parser{}, _issues{},
  _term{} {

	if (::isatty(STDIN_FILENO)) {
		cs::hint::error("no data available");
		cs::dispatch::stop();
	}

	//
	//char buff[1024];
	//// get the calling process ID
	//const pid_t pid = ::getpid();
	//// get the current process session ID
	//const pid_t sid = ::getsid(pid);
	//
	//const char* tty = ::ttyname(STDOUT_FILENO);
	//const char* tid = ::ctermid(buff);
	//
	//std::cout << "tty: " << tty << std::endl;
	//std::cout << "tid: " << tid << std::endl;
	//
	//
	//// open original terminal stdin (not redirected) (macos only)
	//auto ___ctid = cs::open("/dev/tty", O_RDONLY | O_NOFOLLOW);
	////auto ___ctid = cs::open(tid, O_RDONLY | O_NOFOLLOW);
	//auto ___tty = cs::open(tty, O_RDONLY);
	//
	//
	//if (sid != ::tcgetsid(___ctid))
	//	throw cs::runtime_error("tid is not the controlling terminal");
	//else
	//	cs::hint::info("tid is the controlling terminal");
	//
	//if (sid != ::tcgetsid(___tty))
	//	throw cs::runtime_error("tty is not the controlling terminal");
	//else
	//	cs::hint::info("tty is the controlling terminal");
	//
	//_stdin = cs::move(___ctid);
	//
	//
	//std::cout << "stdin: " << _stdin << std::endl;
	//
	//
	//// set non-blocking
	////auto flags = ::fcntl(_stdin , F_GETFL, 0);
	////
	////if (flags < 0)
	////	throw cs::runtime_error("fcntl");
	////
	////flags |= O_NONBLOCK;
	////
	////if (::fcntl(_stdin, F_SETFL, flags) < 0)
	////	throw cs::runtime_error("fcntl");
	//
	//// dup stdin
	////close(STDIN_FILENO);
	////dup2(_stdin, STDIN_FILENO);
	//
	//
	//// set raw mode
	//struct termios raw;
	//
	//if (::tcgetattr(_stdin, &_term) < 0)
	//	throw cs::runtime_error("tcgetattr");
	//
	//raw = _term;
	//
	//raw.c_lflag &= static_cast<tcflag_t>(~(ECHO | ICANON));
	//// min number of bytes of input needed before read can return
	//raw.c_cc[VMIN] = 1;
	//
	//if (::tcsetattr(_stdin, TCSAFLUSH, &raw) < 0)
	//	throw cs::runtime_error("tcsetattr");

}

/* destructor */
cs::input::~input(void) noexcept {

	// restore terminal attributes
	//if (::tcsetattr(_stdin, TCSAFLUSH, &_term) < 0)
	//	cs::hint::error("tcsetattr");
}

// -- public overriden methods ------------------------------------------------

/* read */
auto cs::input::read(void) -> void {

	cs::reader<1024> ___rd;

	___rd.read(_stdin);
	std::cout << "size: " << ___rd.size() << std::endl;

	if (___rd.eof()) {
		cs::hint::info("eof");
		cs::dispatch::stop();
		//
		//if (_issues.empty()) {
		//	cs::hint::info("no issues found");
		//	cs::dispatch::stop();
		//	return;
		//}
		//
		//for (auto& iss : _issues)
		//	iss.print();
		//
		//cs::dispatch::disable(*this);
		//cs::dispatch::add(cs::make_unique<cs::server>());
		return;
	}

	//_parser.parse(___rd, _issues);


	//// buffer
	//char ___ch;
	//
	//// read
	//auto n = ::read(_stdin, &___ch, 1U);
	//
	//if (n < 0)
	//	throw cs::runtime_error("error while reading from stdin");
	//
	//switch (___ch) {
	//
	//	case 'q':
	//		// fallthrough
	//	case '\x1b':
	//		cs::dispatch::stop();
	//		break;
	//	default:
	//		break;
	//}
}

/* send */
auto cs::input::send(void) -> void {

}

/* descriptor */
auto cs::input::descriptor(void) const noexcept -> int {
	return STDIN_FILENO;
}
