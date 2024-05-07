#include "cs/http/method.hpp"


// -- public modifiers --------------------------------------------------------

/* clear */
auto cs::http::method::clear(void) noexcept -> void {
	//_size = 0U;
	_name.clear();
	_uri.clear();
	_version.clear();
}


// -- public accessors --------------------------------------------------------

/* type */
auto cs::http::method::name(void) noexcept -> cs::vector<char>& {
	return _name;
}

/* const type */
auto cs::http::method::name(void) const noexcept -> const cs::vector<char>& {
	return _name;
}

/* uri */
auto cs::http::method::uri(void) noexcept -> cs::vector<char>& {
	return _uri;
}

/* const uri */
auto cs::http::method::uri(void) const noexcept -> const cs::vector<char>& {
	return _uri;
}

/* version */
auto cs::http::method::version(void) noexcept -> cs::vector<char>& {
	return _version;
}

/* const version */
auto cs::http::method::version(void) const noexcept -> const cs::vector<char>& {
	return _version;
}

/* type */
auto cs::http::method::type(void) const noexcept -> unsigned {

	// lookup struct
	struct ___lookup {
		const char* const key;
		const unsigned value;
	};

	// constants
	enum : cs::size_t {
		KEYWORDS =  9U,
		MIN_LEN  =  3U,
		MAX_LEN  =  7U,
		MIN_HASH =  3U,
		MAX_HASH = 12U,
	};

	// gperf table
	static constexpr unsigned char ___table[] {
		13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
		13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
		13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
		13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,  5,
		 0, 13, 13,  0,  0, 13, 13, 13, 13, 13, 13,  0,  5, 13, 13, 13,  0,
		13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
		13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
		13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
		13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
		13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
		13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
		13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
		13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
		13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
		13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
		13
	};

	// lookup table
	static constexpr ___lookup ___map[] {
		{ "",        UNDEF   },
		{ "",        UNDEF   },
		{ "",        UNDEF   },
		{ "GET",     GET     },
		{ "HEAD",    HEAD    },
		{ "TRACE",   TRACE   },
		{ "DELETE",  DELETE  },
		{ "OPTIONS", OPTIONS },
		{ "PUT",     PUT     },
		{ "POST",    POST    },
		{ "PATCH",   PATCH   },
		{ "",        UNDEF   },
		{ "CONNECT", CONNECT }
	};

	const auto size = _name.size();
	const auto data = _name.data();


	// check length
	if (size < MIN_LEN || size > MAX_LEN)
		return UNDEF;

	// get map index
	const auto ___idx = size
					  + ___table[static_cast<unsigned char>(*data)];

	// check index
	if (___idx > MAX_HASH)
		return UNDEF;

	// get indexed key
	const char* ___key = ___map[___idx].key;


	// compare first character
	if (*data != *___key)
		return UNDEF;

	// add null terminator
	//_name.push_back('\0');
	//_buffer[_size % (MAX_LEN + 1)] = '\0';

	// compare strings
	//if (__builtin_strcmp(data + 1U, ___key + 1U) != 0)
	//	return UNDEF;

	// manual compare (because name is not null-terminated)

	cs::size_t i = 1U;
	// compare strings
	for (; i < size && ___key[i] != '\0'; ++i) {
		if (data[i] != ___key[i])
			return UNDEF;
	}

	if (i != size || ___key[i] != '\0')
		return UNDEF;

	// return method
	return ___map[___idx].value;
}

