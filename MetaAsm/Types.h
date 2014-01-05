#pragma once
#include "stdafx.h"



namespace Types
{
	#define BYTE_ALIGN _declspec(align(1))
	typedef signed char s8;
	typedef unsigned char u8;
		
	typedef signed short s16;
	typedef unsigned short u16;
		
	typedef signed int s32;
	typedef unsigned int u32;
		
	typedef signed long s64;
	typedef unsigned long u64;

	typedef u8 byte;
	typedef void* ptr;

	enum Bool : s8 { DEFAULT = -1, FALSE_0 = 0,TRUE_1 = 1};

	enum TypeID : u32{};

	struct Array
	{
		TypeID id;
		u32 size;
		u32 elements;
		void* data;
	};

	BYTE_ALIGN struct
	_byte_
	{
		byte bit_0 : 1;
		byte bit_1 : 1;
		byte bit_2 : 1;
		byte bit_3 : 1;
		byte bit_4 : 1;
		byte bit_5 : 1;
		byte bit_6 : 1;
		byte bit_7 : 1;
	};
}
