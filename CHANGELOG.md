## 0.2.0 (2020-09-14)

- Reimplemented Document class to use std::map, to simplify the code
  and ensure that Item instances never use invalidated pointers.
- Added more efficient read-only DocumentView class.
- Document constructors now use std::string_view.
- Replaced catch2 with the more lightweight doctest testing framework.
- General code cleanup.

## 0.1.0 (2020-04-22)

- Initial release.
