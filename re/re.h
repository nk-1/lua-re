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

#ifndef _RE_H_
#define _RE_H_

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define PCRE2_CODE_UNIT_WIDTH 8

#include <pcre2.h>

#define ESIZE 256

#ifndef unlikely
#define unlikely(x) __builtin_expect((x), 0)
#endif

typedef struct {
	pcre2_code *code;
	pcre2_match_data *match;
	uint32_t flags;
	int32_t matches;
} re_t;

enum re_methods {
	RE_GC,
	RE_MATCH,
	RE_GROUPS,
};

re_t * re_new(PCRE2_SPTR, uint32_t, char *);
void re_free(re_t *);
void re_lightuserdata(re_t *, lua_State *, const lua_CFunction[3]);

#endif /* _RE_H_ */
