#pragma once
#include "stdafx.h"
#include "UTF8.h"




namespace UTF8
{
	UTF8_String::UTF8_String()
	{
		_bytes = 0;
		_code_p = 0;
	}

	UTF8_String::UTF8_String(const utf8* start, bool null_term = true, size_t bytes = 0)
	{
		if (bytes <= 0)
		{

		}
		else
		{
			errno_t err = 0;
			utf8* _data = new utf8[bytes];
			if ((err = memcpy_s(_data, bytes, start, bytes)) > 0)
			{
				delete _data;
				throw err;
			}
			else
			{
				_bytes = bytes;
				_cps = new utf32[_bytes];
				_code_p = _parse_code_points(_data, _cps, _bytes);


			}
		}

	}

	enum utf_enc_header : int {
		CP_ENC_LEN_1B = 0,
		CP_ENC_LEN_2B = 1,
		CP_ENC_LEN_3B = 2,
		CP_ENC_LEN_4B = 3,
		CP_ENC_LEN_5B = 4,
		CP_ENC_LEN_6B = 5,
		CP_ENC_CONT = 6,
		CP_INVALID = 7,

		//always the highest value so that lookup tables cannot over flow
		CP_ENC_LOOKUP_MAX
	};


	void _naive_utf8_to_utf32(utf8* raw, utf32* cp, size_t length)
	{
		const utf8 utf32_shft_lookup[CP_ENC_LOOKUP_MAX] = { 0x0, 0x6, 0x5, 0x4, 0x3, 0x2, 0x2, 0x0 };
		const utf8 utf8_header_lookup[CP_ENC_LOOKUP_MAX] = { 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0X80, 0xFF };
		const utf8 utf32_sig_byte_lookup[CP_ENC_LOOKUP_MAX] = { 0x04, 0x03, 0x03, 0x2, 0x1, 0x0, 0x0 };

		size_t cp_i = 0;
		utf_enc_header cp_header = CP_INVALID;


		//raw_index should always point to a valid utf-8 header
		//if it points to a CP_ENC_CONT then that byte is ignored until a header is found
		// otherwise if the bit pattern is invalid then it is marked CP_INVALID
		for (size_t raw_index = 0; raw_index < length;)
		{
			//First pass determines the type of byte
			for (int enc_index = CP_ENC_LEN_1B; enc_index < CP_INVALID; enc_index++)
			{
				//utf-8 is a signed type so an XOR of by a valid header will always be
				// greater than the original byte
				if ((raw[raw_index] ^ utf8_header_lookup[enc_index]) > raw[raw_index])
				{
					cp_header = static_cast<utf_enc_header>(enc_index);
				}
			}


			if (cp_header != CP_INVALID)
			{
				if (cp_header != CP_ENC_CONT)
				{
					utf32 ucsp = 0;


					//encode the header into a temp int
					reinterpret_cast<utf8*>(ucsp)[utf32_sig_byte_lookup[cp_header]] +=
						//zero out all header bits
						((raw[raw_index] ^ utf8_header_lookup[cp_header])
						//shift align the remaining encoded bits
						>> utf32_shft_lookup[cp_header]
						);

					//header+1 should equal a CP_ENC_CONT if header > 0
					for (int x = 1; x < cp_header; x++)
					{
						//is this a CP_ENC_CONT
						if ((raw[raw_index + x] ^ utf8_header_lookup[CP_ENC_CONT]) > raw[raw_index + x])
						{
							reinterpret_cast<utf8*>(ucsp)[utf32_sig_byte_lookup[cp_header]]
						    = (raw[raw_index]^utf8_header_lookup[CP_ENC_CONT])<<2;
							raw_index++;
						}
						else //poorly formed cp
						{
							raw_index++; //go to next byte in raw and start again 
							break; // ignore code point
						}
					}

					cp[cp_i] = ucsp;
					cp_i++;
					raw_index += cp_header;
					break;
				}
				else
				{
					// if raw_index starts on a CP_ENC_CONT then it is ignored
					// until a valid header is reached, no need to mark as invalid 
					//for ill formed strings as this would lead to strings of invalid cps
					raw_index++;
					break;
				}
			}
			else // if raw_index is an invalid bit pattern then it is marked
			{
				cp[cp_i] = static_cast<utf32>(utf8_header_lookup[CP_INVALID]);
				cp_i++;
				raw_index++;
				break;
			}
		}
	}

	void utf8_to_utf32(utf8* raw, utf32* cp, size_t length)
	{
		const utf8 utf32_shft_lookup[CP_ENC_LOOKUP_MAX] = { 0x0, 0x5, 0x4, 0x3, 0x2, 0x1, 0x6, 0x0 };
		const utf8 utf8_header_lookup [CP_ENC_LOOKUP_MAX] = { 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0X80, 0xFF };
		const utf8 utf32_sig_byte_lookup[CP_ENC_LOOKUP_MAX] = { 0x04, 0x03, 0x03, 0x2, 0x1, 0x0, 0x0 };

		size_t cp_i = 0;
		utf_enc_header cp_header = CP_INVALID;


		//raw_index should always point to a valid utf-8 header
		//if it points to a CP_ENC_CONT then that byte is ignored until a header is found
		// otherwise if the bit pattern is invalid then it is marked CP_INVALID
		for (size_t raw_index = 0; raw_index < length;)
		{
			//First pass determines the type of byte
			for (int enc_index = CP_ENC_LEN_1B; enc_index < CP_INVALID; enc_index++)
			{
				//utf-8 is a signed type so an XOR of by a valid header will always be
				// greater than the original byte
				if ((raw[raw_index] ^ utf8_header_lookup[enc_index]) > raw[raw_index])
				{
					cp_header = static_cast<utf_enc_header>(enc_index);
				}
			}

			if (raw_index + cp_header > length) { return;  }

			const int64_t 
			utf32_lookup[CP_ENC_LOOKUP_MAX] = {0x0000000000000000,
											   0x000000000000C080,
											   0x0000000000E08080,
								   		       0x00000000F0808080,
											   0x000000F880808080,
											   0x0000FC8080808080};
			utf32 *n_cp = &cp[cp_i];
			utf8 *enc_str = &raw[raw_index];
			switch (cp_header)
			{
			case CP_ENC_LEN_1B:
				//this is an ascii byte so it doesnt need treatment
				*n_cp = raw[raw_index];
				break;
			case CP_ENC_LEN_2B:
				//shift the header encoded bits into char one, xor the header 
				//*n_cp = (*enc_str^utf8_header_lookup[CP_ENC_LEN_2B])
					//*n_cp = ((reinterpret_cast<char*>(enc_str)[0] ^ utf8_header_lookup[CP_ENC_LEN_2B]) >> 2 );
				break;

			case CP_ENC_LEN_3B:
				//*n_cp = ();
			case CP_ENC_LEN_4B:
			case CP_ENC_LEN_5B:
			case CP_ENC_LEN_6B:
			
				break;
			case CP_ENC_CONT:
				raw_index++;
				break;
			case CP_INVALID:
			default:

				break;
			}
			cp_i++;
			raw_index+=cp_header;



			//align bits
			int64_t rdata = *reinterpret_cast<int64_t*>(&raw[raw_index]);
			rdata = rdata >> utf32_shft_lookup[cp_header];
			//strip the encoding
			int64_t stmp = static_cast<int64_t>(reinterpret_cast<utf8*>(utf32_lookup[cp_header])[cp_header])^rdata;
			
		}
	}


	size_t UTF8_String::_parse_code_points(const utf8* raw, utf32* cp_out, size_t length)
	{

		bool open = false;
		size_t cp_count = 0;
		utf32 cp = 0;

		for (size_t l = 0; l < length; l++)
		{
			switch (l)
			{
			case CP_ENC_LEN_1B:
				break;
			case CP_ENC_LEN_2B:
				break;
			}
		}


		for (size_t i = 0; i < length; i++)
		{
			if (raw[i] < 0 && !open)
			{

			}
		}
		return 0;
	}
























}






