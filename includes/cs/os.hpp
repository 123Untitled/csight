#ifndef ___CS_OS_HPP___
#define ___CS_OS_HPP___

// check for windows
#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__NT__)
#	define ___cs_os_windows
// check for macos
#elif defined(__APPLE__) || defined(__MACH__)
#	define ___cs_os_macos
// check for linux
#elif defined(__linux__)
#	define ___cs_os_linux
// check for unix
#elif defined(__unix__) || defined(__unix)
#	define ___cs_os_unix
#endif

#endif // ___CS_OS_HPP___
