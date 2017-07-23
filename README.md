lua-re
======

Module **re** provides bindings for [PCRE2](http://www.pcre.org/) library to
Lua 5.1

## Synopsis

```lua
local re = require "re"

local foo = [[(?<qq>\d)(\d+)]]
local re_foo = re.compile(foo, 'i')
--[[ Compile a regular expression pattern into a regular expression object,
which can be used for matching using its match() method, described below. The
expression’s behaviour can be modified by specifying flags
]]

print(re_foo:match('0123456789'))
--[[ Matches the specified string and returns number of captured groups. For
failed match returns nil. Thus current statement gives the following output:
3
]]

print(re_foo.groups[0])
print(re_foo.groups.qq)
--[[ Returns one numbered or named capture group of the match. If the specified
subgroup exists the captured string will be yielded. Otherwise nil will be
returned. Thus 2 previous statements give the following output:
0123456789
0
]]
```

## Author

Igor Munkin, <imun@cpan.org>

## Acknowledgements

Mons Anderson - The original idea, rationale and motivation

## Bugs

Feel free to report your bugs by mailing to <imun@cpan.org> or via
[github issues](https://github.com/igormunkin/lua-re/issues)

## Copyright and License

Copyright (C) 2017 by Igor Munkin

This library is released under the following license: LGPL
