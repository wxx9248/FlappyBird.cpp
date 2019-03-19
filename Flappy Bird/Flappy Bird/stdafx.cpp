#include "stdafx.h"

int strchrs(const char *str, const char *chrs, bool strictmode)
{
	size_t szstr = strlen(str), szchrs = strlen(chrs);
	int cnt = 0;

	for (size_t i = 0; i < szchrs; ++i)
		for (size_t j = 0; j < szstr; ++j)
			if (chrs[i] == str[j])
			{
				++cnt;
				if (strictmode)
					return cnt;
				else
					break;
			}

	return cnt;
}

