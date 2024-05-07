#include "cs/map.hpp"


// -- public lifecycle --------------------------------------------------------

/* default constructor */
cs::core::map::map(void)
: _data{cs::calloc<___node*>(DEFAULT_SIZE)},
  _size{0U},
  _capacity{DEFAULT_SIZE}
#ifdef CS_CORE_MAP_DEBUG
  , _nprobe{0U}
  , _nresize{0U}
#endif
{
}

/* move constructor */
cs::core::map::map(___self&& ___ot) noexcept
: _data{___ot._data},
  _size{___ot._size},
  _capacity{___ot._capacity} {
	// zero other
	___ot._init();
}

/* destructor */
cs::core::map::~map(void) noexcept {
	_clear();
}


// -- public assignment operators ---------------------------------------------

/* move assignment operator */
auto cs::core::map::operator=(___self&& ___ot) noexcept -> ___self& {

	// check self assignment
	if (this == &___ot)
		return *this;

	// clear self
	_clear();

	// move data
	_copy_members(___ot);

	// zero other
	___ot._init();

	// return self
	return *this;
}


// -- public accessors --------------------------------------------------------

/* size */
auto cs::core::map::size(void) const noexcept -> size_type {
	return _size;
}

/* capacity */
auto cs::core::map::capacity(void) const noexcept -> size_type {
	return _capacity;
}

/* empty */
auto cs::core::map::empty(void) const noexcept -> bool {
	return _size == 0U;
}


// -- public static accessors -------------------------------------------------

/* load factor */
auto cs::core::map::load_factor(void) noexcept -> float {
	return _load_factor;
}

/* default size */
auto cs::core::map::default_size(void) noexcept -> size_type {
	return DEFAULT_SIZE;
}


// -- public modifiers --------------------------------------------------------

/* insert */
auto cs::core::map::insert(cs::string&& file,
						   cs::core::issue&& ___iss) -> void {

	// check if size is full
	if (_need_resize() == true)
		_resize();

	// hash key
	const auto hash = ___self::_hash(file);
	auto index = hash % _capacity;

	// find empty slot
	for (size_type q = 1U; _data[index] != nullptr; ++q) {

		// check hash and file exists
		if (_data[index]->hash() == hash
		 && _data[index]->entry().file() == file) {
			_data[index]->entry()._issues.push(cs::move(___iss));
			// clear file (move semantic responsibility)
			file.clear();
			return; }

		// quadratic probing, todo: maybe check infinite loop
		index = (index + (q * q)) % _capacity;

#ifdef CS_CORE_MAP_DEBUG
		++_nprobe;
#endif
	}

	// allocate new node
	_data[index] = cs::malloc<___node>();

	// construct node
	cs::lifecycle<___node>::construct(_data[index],
			hash,
			cs::move(file),
			cs::move(___iss));

	// increment size
	++_size;
}


/* find */
auto cs::core::map::find(const cs::string& file) const -> const_pointer {

	// hash key
	const auto hash = ___self::_hash(file) % _capacity;
	auto index = hash % _capacity;

	// find empty slot
	for (size_type q = 1U; q < _capacity; ++q) {

		if (_data[index] == nullptr)
			return nullptr;

		// check key exists
		if (_data[index]->hash() == hash
		 && _data[index]->entry().file() == file)
			return &_data[index]->entry();

		// quadratic probing
		index = (index + (q * q)) % _capacity;
	}

	return nullptr;
}



// -- private methods ---------------------------------------------------------

/* init */
auto cs::core::map::_init(void) noexcept -> void {
		_data = nullptr;
		_size = 0U;
	_capacity = 0U;
}

/* copy members */
auto cs::core::map::_copy_members(const ___self& ___ot) noexcept -> void {
		_data = ___ot._data;
		_size = ___ot._size;
	_capacity = ___ot._capacity;
}

/* clear */
auto cs::core::map::_clear(void) noexcept -> void {

	___node**  it = _data;
	___node** end = it + _capacity;

	// free data
	for (;it != end; ++it) {

		// continue if null
		if (*it == nullptr)
			continue;

		// destruct node
		cs::lifecycle<___node>::destroy(*it);
		// free node
		cs::free(*it);
	}

	// free data
	cs::free(_data);
}

/* resize */
auto cs::core::map::_resize(void) -> void {

#ifdef CS_CORE_MAP_DEBUG
	++_nresize;
#endif

	// compute new capacity
	const auto ___cp = (_capacity << 1U) + 1U; // odd

	// allocate zeroed memory
	auto ___new = cs::calloc<___node*>(___cp);

	// re-hash map
	for (size_type i = 0U; i < _capacity; ++i) {

		if (_data[i] == nullptr)
			continue;

		// hash key
		auto idx = _data[i]->hash() % ___cp;

		// find empty slot
		for (size_type q = 1U; ___new[idx] != nullptr; ++q) {
			// quadratic probing
			idx = (idx + (q * q)) % ___cp;

#ifdef CS_CORE_MAP_DEBUG
			++_nprobe;
#endif
		}

		___new[idx] = _data[i];
	}

	// free old data
	cs::free(_data);

	// set new data
	_data = ___new;

	// set new capacity
	_capacity = ___cp;
}


// -- private accessors -------------------------------------------------------

/* compute load factor */
auto cs::core::map::_compute_load_factor(void) const noexcept -> float {
	return static_cast<float>(_size) / static_cast<float>(_capacity);
}

/* need resize */
auto cs::core::map::_need_resize(void) const noexcept -> bool {
	return _compute_load_factor() >= _load_factor;
}


// -- private static methods --------------------------------------------------

/* hash (murmur) */
auto cs::core::map::_hash(const cs::string& key) noexcept -> size_type {

	uint32_t c1 = 0xcc9e2d51;
	uint32_t c2 = 0x1b873593;
	uint32_t r1 = 15;
	uint32_t r2 = 13;
	uint32_t m = 5;
	uint32_t n = 0xe6546b64;
	uint32_t h = 0;
	uint32_t k = 0;

	// 32 bit extract from `key'
	uint8_t* d = (uint8_t*) key.data();

	const uint32_t* chunks = nullptr;
	// tail - last 8 bytes
	const uint8_t* tail = nullptr;

	int i = 0;

	// len
	const auto len = (uint32_t)key.size();

	// chunk length
	int l = len / 4;

	// seed
	h = 2'147'483'647U;

	chunks = (const uint32_t *) (d + l * 4); // body
	tail = (const uint8_t *) (d + l * 4); // last 8 byte chunk of `key'

	// for each 4 byte chunk of `key'
	for (i = -l; i != 0; ++i) {
		// next 4 byte chunk of `key'
		k = chunks[i];

		// encode next 4 byte chunk of `key'
		k *= c1;
		k = (k << r1) | (k >> (32 - r1));
		k *= c2;

		// append to hash
		h ^= k;
		h = (h << r2) | (h >> (32 - r2));
		h = h * m + n;
	}

	k = 0;

	// remainder
	switch (len & 3) { // `len % 4'

		case 3:
			k ^= (uint32_t)(tail[2] << 16);
		case 2:
			k ^= (uint32_t)(tail[1] << 8);

		case 1:
				k ^= tail[0];
				k *= c1;
				k = (k << r1) | (k >> (32 - r1));
				k *= c2;
				h ^= k;
	}

	h ^= len;

	h ^= (h >> 16);
	h *= 0x85ebca6b;
	h ^= (h >> 13);
	h *= 0xc2b2ae35;
	h ^= (h >> 16);

	return h;
}

