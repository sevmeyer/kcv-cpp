kcv-cpp
=======

A lightweight [KCV] library for C++17.

- Only depends on the standard library
- Does not throw exceptions (the underlying
  standard classes may throw `std::bad_alloc`)
- Fully validates the format and encoding
- Efficient memory usage
- Logarithmic item lookup
- Single header
- Hassle-free [Boost Software License]

[KCV]: https://github.com/sevmeyer/kcv
[Boost Software License]: https://www.boost.org/users/license.html


Example
-------

Read and write values:

```cpp
kcv::Document doc{"B:yes I:21"};

bool b{};
int  i{};

doc["B"] >> b;
doc["I"] >> i;

doc["B"] << !b;
doc["I"] << i*2;

std::cout << doc.dump();

// B: no
// I: 42
```

Read or write multiple values per item:

```cpp
kcv::Document doc{"foo: 3.14 42"};

float f{};
int   i{};

doc["foo"] >> f >> i;

std::cout << f << "\n";
std::cout << i << "\n";

// 3.14
// 42
```

Read an unkown number of values:

```cpp
kcv::Document doc{"foo: 5 42 127"};
kcv::Item item{doc["foo"]};

for (int i{}; item >> i; )
  std::cout << i << "\n";

// 5
// 42
// 127
```

Write formatted values:

```cpp
kcv::Document doc{};

doc["foo"]
  << kcv::hex(42, 6)
  << kcv::nl()
  << kcv::sp(2)
  << kcv::fixed(1.5, 3)
  << kcv::general(3.14159, 4);

std::cout << doc.dump();

// foo: 0x00002a
//   1.500 3.142
```


Interface
---------

The Document class manages a string buffer and
provides access to the items.

```cpp
class Document
{
	// Constructs an empty document.
	Document()

	// Takes ownership of a KCV string and parses it.
	// If the parsing fails, or the non-zero maxItemCount
	// is exceeded, an empty document is constructed instead.
	// The string size is limited to 4 GiB.
	explicit Document(std::string data, std::size_t maxItemCount = 0)

	// Checks if the constructor accepted the data string.
	explicit operator bool() const

	// Retrieves an item. If the key does not exist, a new
	// item is inserted. If the key is invalid, or the non-zero
	// maxItemCount is exceeded, an inactive item is returned.
	//
	// IMPORTANT: Only use the most recent Item instance.
	// Old Item instances may contain invalidated references.
	//
	Item operator[](std::string_view key)

	// Converts the document to a string.
	// The items are ordered lexicographically.
	std::string dump() const
};
```

The Item class provides streaming access to the values.
The streaming position is reset for each instance.
Items are robust and can be used without error-checking.

```cpp
class Item
{
	// Checks if the item is active after construction,
	// or if the previous read or write was successul.
	explicit operator bool() const

	// Reads the next value and assigns it to a standard bool,
	// integral, floating-point, string, or string_view variable.
	// If the read fails, the target variable is left unchanged.
	// A string_view cannot contain any escape sequence and only
	// remains valid until the document is altered.
	// The read position is reset after each write.
	Item& operator>>(T& target)

	// Appends a standard bool, integral, floating-point,
	// string, or string_view value. Invalid values are ignored.
	// For strings, any double quote or backslash is escaped.
	// The write position is reset after each read.
	Item& operator<<(const T& value)
};
```

Value formatters are available to write
hexadecimal integers or rounded floats.

```cpp
// Writes a zero-padded hexadecimal integer.
hex(T value, int width = 1)

// Writes a floating-point value with a fixed
// number of digits after the decimal point.
fixed(T value, int precision = 6)

// Writes a floating-point value in general notation.
general(T value, int precision = 6)

// Writes newlines.
nl(int count = 1)

// Writes spaces.
sp(int count = 1)

// Writes tabs.
tab(int count = 1)
```


Complexity
----------

- Document construction: `O(chars + items*log(items))`
- Item lookup: `O(log(items))`

The document content is stored in a single `std::string` buffer. 
Additionally, the library maintains 128 bits of metadata per item
in a sorted `std::vector`. Values are read and written in-place.

To minimize the number of allocations, the `maxItemCount` parameter
is used to reserve enough space upfront for the metadata. Also, the
expected buffer size can be reserved in the `data` string before it
is moved to the document constructor.


Install
-------

This is a header-only library. Simply add the header
from the `include` directory to your project.

The unit tests can be compiled and run with:

```
mkdir build
cd build
cmake ..
make
./test/kcv-test
```
