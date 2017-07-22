#ifndef _RE_H_
#define _RE_H_

#include <tarantool/module.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#define PCRE2_CODE_UNIT_WIDTH 8

#include <pcre2.h>

#define ESIZE 256

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
