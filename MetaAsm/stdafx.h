// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include "Types.h"

using namespace Types;

#define BYTE_ALIGN _declspec(align(1))


template<unsigned T>
struct BYTE_ALIGN aligned_bytes
{
	byte op[T];
};


// TODO: reference additional headers your program requires here
