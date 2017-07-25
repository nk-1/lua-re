#!/usr/bin/env tarantool

local re = require('re')
local tap = require('tap')

local test = tap.test('re tests')
test:plan(1)

test:test('re', function(test)
	local a = [[(?<qq>\d)(\d+)]]
	local re_a = re.compile(a)
	test:plan(5)
	test:is(re_a:match('0123456789'), 3, "Match")
	test:is(re_a.groups.qq, '0', "Named capture group (qq)")
	test:is(re_a.groups[0], '0123456789', "Numbered capture group (0)")
	test:is(re_a.groups[1], '0', "Numbered capture group (1)")
	test:is(re_a.groups[2], '123456789', "Numbered capture group (2)")
end)

os.exit(test:check() == true and 0 or -1)
