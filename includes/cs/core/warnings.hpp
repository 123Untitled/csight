#ifndef ___CS_CORE_WARNINGS_HPP___
#define ___CS_CORE_WARNINGS_HPP___

#include "cs/config.hpp"
#if ___cs_requirements


// -- C S  N A M E S P A C E --------------------------------------------------

namespace cs {

	struct warnings final {
		const char* _origin;
		const char* _replacement;
	};

	static constexpr warnings wmap[] {

		// to change !
		{"no viable constructor or deduction guide for deduction of template arguments of",
			"no viable constructor or deduction guide for deduction of template arguments of"},


		{"has been explicitly marked deleted here",
			"annihilated here"},

		{"invalid explicitly-specified argument for template parameter",
			"faulty template argument"},

		{"set but not used",
			"lone wolf"},

		{"out-of-line definition of",
			"rebel definition of"},

		{"does not match any declaration in",
			"declared nowhere in"},

		{"call to deleted constructor of",
			"banned ctor of"},

		{"static assertion failed",
			"static flop"},

		{"in member function",
			"in method"},

		{"does not satisfy",
			"doesn't cut it"},

		{"for 1st argument",
			"for 1st arg"},

		{"invalid operands to binary expression",
			"binary blunder"},

		{"previous definition is here",
			"early bird here"},

		{"requested here",
			"called here"},

		{"was not satisfied",
			"didn't make the cut"},

		{"definition of implicitly declared destructor",
			"implicit destructor defined"},

		{"undeclared identifier",
			"unknown identifier"},

		{"candidate function has been explicitly deleted",
			"vanished function"},

		{"couldn't infer template argument",
			"couldn't crack template argument"},

		{"expanded from macro",
			"macro-burst"},

		{"overload resolution selected deleted operator",
			"banned operator picked"},

		{"evaluated to false",
			"evaluated to nope"},

		{"candidate function not viable",
			"function lost its mojo"},

		{"no known conversion from",
			"no magic for"},

		{"candidate template ignored",
			"template ghosted"},

		{"candidate function template not viable",
			"unworkable function template"},

		{"constraints not satisfied for class template",
			"failed template wish"},

		{"use of undeclared identifier",
			"mysterious identifier"},

		{"no matching function for call to",
			"function call has no match"},

		{"unknown type name",
			"obscure"},

		{"did you mean",
			"suggest"},

		{"expected unqualified-id",
			"wrong syntax"},

		{"too many template arguments for function template",
			"excess template arguments"},

		{"deduced conflicting types for parameter",
			"type battle royale for the parameter"},

		{"in instantiation of function template specialization",
			"during template magic"},

		{"in instantiation of member function",
			"in member function"},

		{"required from here",
			"required here"},

	};


} // namespace cs

#endif // ___cs_requirements
#endif // ___CS_CORE_WARNINGS_HPP___
