#pragma once
#include "stdafx.h"


namespace UTF8
{
	typedef unsigned char utf8;
	typedef unsigned int utf32;

	class UTF8_String
	{
	public:
		UTF8_String();
		UTF8_String(const utf8* start, bool null_term, size_t bytes);

		~UTF8_String();



	private:
		size_t _parse_code_points(const utf8* raw, utf32* cp_out, size_t length);

		utf32* _cps;
		size_t _bytes, _code_p;
	};
}