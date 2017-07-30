/*
 *  This file is a part of the lua-re project
 *  Copyright (C) 2017 Igor Munkin (imun@cpan.org)
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 *  USA
 */

#include "re.h"

#define RE_STRERROR(strerror, fmt, ...)        \
	if (strerror)                              \
		sprintf(strerror, fmt, ##__VA_ARGS__);

re_t *
re_new(PCRE2_SPTR pattern, uint32_t flags, char *strerror) {
	PCRE2_SIZE eoffset;
	int32_t ecode;
	re_t *re = (re_t *) malloc(sizeof(re_t));
	if (unlikely(!re)) {
		RE_STRERROR(strerror, "Cannot allocate memory for a new re object");
		goto except;
	}
	bzero(re, sizeof(re_t));
	PCRE2_SIZE pattern_sz = strlen(pattern);
	re->code = pcre2_compile(pattern, pattern_sz, flags, &ecode, &eoffset, NULL);
	if (unlikely(!re->code)) {
		char pcre2error[ESIZE];
		pcre2_get_error_message(ecode, pcre2error, sizeof(pcre2error));
		RE_STRERROR(strerror, "PCRE compilation failed at %lu: %s",
			eoffset, pcre2error);
		goto except;
	}
	re->match = pcre2_match_data_create_from_pattern(re->code, NULL);
	if (unlikely(!re->match)) {
		RE_STRERROR(strerror, "Cannot allocate memory for capture groups");
		goto except;
	}
	re->matches = 0;
	re->flags = flags;
	return re;
except:
	if(re) {
		if (re->code)
			pcre2_code_free(re->code);
		if (re->match)
			pcre2_match_data_free(re->match);
		free(re);
	}
	return NULL;
}

void
re_free(re_t *re) {
	if(re) {
		if (re->code)
			pcre2_code_free(re->code);
		if (re->match)
			pcre2_match_data_free(re->match);
		free(re);
	}
	return;
}

void
re_lightuserdata(re_t *re, lua_State *L, const lua_CFunction re_methods[4]) {
	lua_pushlightuserdata(L, re);
	luaL_newmetatable(L, "__tnt_re_lua");
	lua_pushliteral(L, "__index");
	lua_createtable(L, 0, 1);
	lua_pushliteral(L, "__gc");
	lua_pushcfunction(L, re_methods[RE_GC]);
	lua_rawset(L, -3);
	lua_pushliteral(L, "match");
	lua_pushcfunction(L, re_methods[RE_MATCH]);
	lua_rawset(L, -3);
	lua_pushliteral(L, "groups");
	lua_createtable(L, 0, 1);
	lua_pushliteral(L, "re");
	lua_pushlightuserdata(L, re);
	lua_rawset(L, -3);
	luaL_newmetatable(L, "__tnt_re_groups");
	lua_pushliteral(L, "__index");
	lua_pushcfunction(L, re_methods[RE_GROUPS]);
	lua_rawset(L, -3);
	lua_pushliteral(L, "__newindex");
	lua_pushcfunction(L, re_methods[RE_CONST]);
	lua_rawset(L, -3);
	lua_setmetatable(L, -2);
	lua_rawset(L, -3);
	lua_rawset(L, -3);
	lua_setmetatable(L, -2);
	return;
}
