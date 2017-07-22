#include <stdio.h>
#include <string.h>
#include "re.h"

#define RE_CROAK(lua_State, fmt, ...) do {     \
	luaL_error(lua_State, fmt, ##__VA_ARGS__); \
} while(0)

static int re_compile(lua_State *);
static int re_gc(lua_State *);
static int re_match(lua_State *);
static int re_groups(lua_State *);

static const struct luaL_Reg libre_methods [3] = {
	{ "compile",    re_compile },
	{ "match",      re_match   },
	{ NULL,         NULL       }
};

static const lua_CFunction re_methods [3] = {
	[RE_GC] = re_gc,
	[RE_MATCH] = re_match,
	[RE_GROUPS] = re_groups
};

static int
re_compile(lua_State *L) {
	PCRE2_SIZE pattern_sz, options_sz;
	char strerror[ESIZE];
	PCRE2_SPTR pattern = (PCRE2_SPTR) lua_tolstring(L, 1, &pattern_sz);
	if (!pattern_sz)
		RE_CROAK(L, "No pattern passed");
	PCRE2_SPTR options = (PCRE2_SPTR) lua_tolstring(L, 2, &options_sz);
	uint32_t flags = 0;
	uint8_t i;
#define RE_SETFLAG(flags, option) flags |= option; break;
	for (i = 0; i < options_sz; i++) {
		switch(options[i]) {
			case 'x': RE_SETFLAG(flags, PCRE2_EXTENDED);
			case 'i': RE_SETFLAG(flags, PCRE2_CASELESS);
			case 'm': RE_SETFLAG(flags, PCRE2_MULTILINE);
			case 's': RE_SETFLAG(flags, PCRE2_DOTALL);
			case 'n': RE_SETFLAG(flags, PCRE2_NO_AUTO_CAPTURE);
			default: RE_CROAK(L, "Undefined option: %c", options[i]);
		}
	}
#undef RE_SETFLAG
	re_t *re = re_new(pattern, flags, strerror);
	if (!re)
		RE_CROAK(L, strerror);
	re_lightuserdata(re, L, re_methods);
	return 1;
}

static int
re_gc(lua_State *L) {
	re_t *re = (re_t *) lua_touserdata(L, 1);
	re_free(re);
	return 1;
}

static int
re_match(lua_State *L) {
	size_t subject_sz;
	re_t *re = (re_t *) lua_touserdata(L, 1);
	PCRE2_SPTR subject = (PCRE2_SPTR) lua_tolstring(L, 2, &subject_sz);
	re->matches =
		pcre2_match(re->code, subject, subject_sz, 0, 0, re->match, NULL);
	if (re->matches < 0)
		switch(re->matches) {
			case PCRE2_ERROR_NOMATCH:
				lua_pushnil(L);
				return 1;
			default:
				RE_CROAK(L, "Matching error %d\n", re->matches);
		}
	lua_pushnumber(L, re->matches);
	return 1;
}

static int
re_groups(lua_State *L) {
	PCRE2_UCHAR *captured = NULL;
	PCRE2_SIZE captured_sz = 0;
	lua_pushliteral(L, "re");
	lua_rawget(L, 1);
	re_t *re = (re_t *) lua_touserdata(L, -1);
	if (!re->matches)
		RE_CROAK(L, "There is no match groups for this pattern\n");
	if (lua_isnumber(L, 2)) {
		uint32_t number = (uint32_t) lua_tointeger(L, 2);
		pcre2_substring_get_bynumber(re->match, number, &captured, &captured_sz);
	} else if (lua_isstring(L, 2)) {
		PCRE2_SPTR name = (PCRE2_SPTR) lua_tostring(L, 2);
		pcre2_substring_get_byname(re->match, name, &captured, &captured_sz);
	} else {
		RE_CROAK(L, "Invalid key for the capture groups\n");
	}
	if (!captured)
		lua_pushnil(L);
	else
		lua_pushlstring(L, captured, captured_sz);
	pcre2_substring_free(captured);
	return 1;
}


/* functions exported from library */
LUA_API int
luaopen_re(lua_State *L) {
	lua_newtable(L);
	luaL_register(L, NULL, libre_methods);
	return 1;
}

