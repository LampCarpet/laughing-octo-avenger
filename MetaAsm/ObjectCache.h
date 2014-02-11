#pragma once
#include "stdafx.h"

#include <Windows.h>
#include <strsafe.h>

#ifdef DEBUG
#include <iostream>
#endif



namespace Utilities
{
	struct _ll_node
	{
		Types::u8* page;
		Types::u16 objects_allocd;
	};

	template<typename T, int flag = PAGE_READWRITE>
	class ObjectCache
	{
	public:
		ObjectCache();
		ObjectCache(Types::u8 initial_pages, Types::u8 max_pages, bool large_pages = false, bool use_strict_max = false);
	
		~ObjectCache();


	};


}



















