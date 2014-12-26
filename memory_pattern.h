#pragma once

#include "header.h"

enum CacheState {  
	MOD, EXC, SHA, INV
};

struct CacheLine {
	uint32_t add;
	CacheState sta;
};