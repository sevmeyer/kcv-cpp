kcv-cpp
=======

A lightweight [KCV] library for C++17.

- Only depends on the standard library
- Does not throw exceptions (the underlying standard
  classes may throw when they run out of memory)
- Fully validates the format and encoding
- Logarithmic item lookup
- Efficient read-only version
- Single header
- Hassle-free [Boost Software License]

[KCV]: https://github.com/sevmeyer/kcv
[Boost Software License]: https://www.boost.org/users/license.html


Example
-------

Read and write values:

```cpp
kcv::Document doc{"Hello:no World:21"};

bool b{};
int  i{};

doc["Hello"] >> b;
doc["World"] >> i;

doc["Hello"] << !b;
doc["World"] << i*2;

std::cout << doc.dump();

// Hello: yes
// World: 42
```

Read or write multiple values per item:

```cpp
kcv::Document doc{"foo: 3.14 42"};

float f{};
int   i{};

doc["foo"] >> f >> i;

std::cout << f << "\n" << i << "\n";

// 3.14
// 42
```

Read an unkown number of values:

```cpp
kcv::Document doc{"foo: 5 42 100"};
kcv::Item item{doc["foo"]};

for (int i{}; item >> i; )
  std::cout << i << "\n";

// 5
// 42
// 100
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

The Document class manages the data and provides access
to the items. Item lookup has logarithmic complexity.

```cpp
class Document
{
	public:

	// Constructs an empty document.
	Document()

	// Parses a KCV string. If the parsing fails, or the non-zero
	// maxItemCount is exceeded, an empty document is constructed.
	explicit Document(std::string_view data, std::size_t maxItemCount = 0)

	// Checks if the constructor accepted the data string.
	explicit operator bool() const

	// Retrieves an item. If the key does not exist, a new item is
	// inserted. If the key is invalid, an inactive item is returned.
	Item operator[](std::string_view key)

	// Writes the document to a string.
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
	public:

	// Checks if the item is active after construction,
	// or if the most recent read or write was successul.
	explicit operator bool() const

	// Reads the next value and assigns it to a standard bool,
	// integral, floating-point, string, or string_view variable.
	// If the read fails, the target variable is left unchanged.
	// A string_view cannot contain any escape sequence and
	// remains valid until the item is altered.
	// Resets the write position.
	Item& operator>>(T& target)

	// Appends a standard bool, integral, floating-point,
	// string, or string_view value. Invalid values are ignored.
	// Within strings, any double quote or backslash is escaped.
	// The write position is reset after each read.
	// Resets the read position.
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

Read-only
---------

The `DocumentView` class is an efficient, read-only
implementation that does not own or allocate data.
It provides the same interface, but cannot create
new items or write values.

```cpp
template<std::size_t MaxItemCount>
class DocumentView
{
	public:

	DocumentView()
	explicit DocumentView(std::string_view data)

	explicit operator bool() const
	ItemView operator[](std::string_view key)
};

class ItemView
{
	public:

	explicit operator bool() const
	ItemView& operator>>(T& target)
};
```

Note that it only remains valid as long as the
provided data string_view is valid.

It stores MaxItemCount*64 bits of metadata on the stack.
The data string size is limited to 4 GiB, the item
size to 16 MiB, and the key size to 255 bytes.
Item lookup has logarithmic complexity.


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
