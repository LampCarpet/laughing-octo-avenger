#include "stdafx.h"
#include "x86_64_instruction_set.h"
#include "Utilities.h"
#include "tmp_common.h"
#include "C-lang-defs.h"

#include <cstdarg>

using namespace std;
using namespace x86_64_instruction_base;
using namespace x86_64_instruction_set;
using namespace tmp_common;

typedef Instruction_Prototype<PrefixGroupDescriptor::QUAD_PREFIX,
	Optional::INCLUDED,
	OpCodeType::TRIPLE_BYTE_OPCODE,
	Optional::INCLUDED,
	Optional::INCLUDED,
	DisplacementType::TRIPLE_BYTE_DISPLACEMENT,
	ImmediateDataSize::OCTO_BYTE_IMMEDATE> InstructionBuffer;

using namespace utilities;

class Image 
{

	struct sector
	{
		unsigned id, begin, end, data_len;
		Types::u8* data;
	};

public:

private:
	ObjectCache<ObjectCache<sector>> _index;
};


class ExecEnv
{
public:
	
private:
};


template<unsigned size>
struct object
{
public:
	void init(unsigned i)
	{
		buf[0] = i;
		buf[size - 1] = i;
	}
	bool validate(unsigned v)
	{
		if (buf[0] != v || buf[size-1] != v)
		{
			return false;
		}
		else
		{
			return true;
		}

	}
	unsigned get()
	{
		return buf[0];
	}
	
	unsigned buf[size];
};


void DynamicCacheTesting()
{
	const unsigned ts = 20, os = 32;
	utilities::ObjectCache<object<os>> cache;
	object<os>* vTest[ts*2];
	ZeroMemory(vTest, sizeof(object<os>*)*(ts*2));
	
	cout << "Beginning AllocateObject() run..." << endl;
	for (unsigned i = 0; i < ts; i++)
	{
		if ((vTest[i] = cache.AllocateObject()) == nullptr){ cout << endl << "----- nullptr at: "<<i<<" -----" << endl; }
		if ((vTest[i+1] = cache.AllocateObject()) == nullptr){ cout << endl << "----- nullptr at: " << i+1 << " -----" << endl; }
		vTest[i]->init(i);
		vTest[i + 1]->init(i + 1);
		int dif = 0;
		if ((dif = ((unsigned) vTest[i+1] - (unsigned) vTest[i]) != sizeof(object<os>)) && dif < 4096)
		{ cout << endl << "Address misaligned at: " << i << endl << vTest[i+1] << endl << vTest[i] << endl << (unsigned) vTest[i+1] - (unsigned) vTest[i] << endl; }
	
	}

	cout << "Begining coherency test..." << endl;
	unsigned errs = 0;
	
	for (unsigned x = 0; x < ts; x++)
	{
		object<32>* t = vTest[x];
		if (!vTest[x]->validate(x) )
		{
			cout << "[" << x << "] " << "invalid object." << endl;
			errs++;
		}
		if (!vTest[x+1]->validate(x + 1))
		{
			cout << "[" << x+1 << "] " << "invalid object." << endl;
			errs++;
		}
	}
	
	for (unsigned i = 0; i < ts; i++)
	{
		if (!cache.DeAllocateObject(vTest[i]))
		{
			cout << "DeAllocateObject() failed at: " << i <<endl;
			cout << "Object address: " << vTest[i]<< endl;
		}
	}
	
	if (errs > 0) { cout << errs << " errors were detected." << endl; }
	else{ cout << "Coherency test completed successfully." << endl; }
	cout << "Objects allocated: " << cache.GetObjectsAllocated() << endl;

	cout << "Beginning FlushCachePage()..." << endl;
	cache.FlushCachePage();

	cout << "Objects available after FlushCachePage(): " << cache.GetObjectsAvailable() << endl;

	cout << "Beginning FlushCache()..." << endl;
	cache.FlushCache();
	cout << "Objects available after FlushCache(): " << cache.GetObjectsAvailable() << endl;
	cout << "Objects allocated after FlushCache(): " << cache.GetObjectsAllocated() << endl;
	cout << "Pushing " << ts << " objects..." << endl;

	for (unsigned ps = 0; ps < ts; ps++)
	{
		object<32> obj; 
		obj.init(0);
		if (cache.Push(obj) == USHRT_MAX)
		{
			cout << "Push() failed at: " << ps << endl;
		}
	}

	cout << "Popping " << cache.GetObjectsAllocated() << " objects..."<< endl;

	for (unsigned po = 0; po < cache.GetObjectsAllocated(); po++)
	{
		object<32> obj = cache.Pop();
		if (!obj.validate(po))
		{
			cout << "Pop() failed at: " << po << endl;
			cout << "with value: " << obj.get() << endl;
		}
	}
	cout << "object<> type allocation: " << sizeof(object<os>) << endl;
}

void InstTesting()
{
	Instruction_Prototype<PrefixGroupDescriptor::SINGLE_PREFIX,
		Optional::INCLUDED,
		OpCodeType::SINGLE_BYTE_OPCODE,
		Optional::INCLUDED,
		Optional::INCLUDED,
		SINGLE_BYTE_DISPLACEMENT,
		SINGLE_BYTE_IMMEDIATE> i;


	cout << sizeof(i);
	cout << endl;
}

#define SCHAR
#ifndef SCHAR
typedef wchar_t lchar;
#define LCHAR(x)static_cast<lchar*>(x)
#endif

#ifdef SCHAR
typedef char lchar;
#define LCHAR(x)static_cast<lchar*>(x)
#endif

struct ConstStr
{
	const lchar* str;
	const unsigned len;
};

char alphaK [] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

ConstStr core[] = { { LCHAR("int"), 3 }, {"=",0} };

const lchar lang [] = "int i; int x = 0; void t(){}";
unsigned len = 29;

class Context
{
public:

private:
};

char* test = "int  dfhi9usydghf4[];; auto jklhg] double   ";

#include <iomanip>
template<typename T>
void PrintAsHex(const char* preamble, T arg)
{
	cout << preamble;
	cout.setf(std::ios_base::hex, std::ios_base::basefield);
	cout.setf(std::ios_base::showbase);
	cout.setf(std::ios_base::uppercas23e);
	cout << std::left << arg;
	cout << endl;
}

template <typename T, typename ptype= const char*>
void PrintAsBin(ptype preamble, T arg)
{
	const UCHAR bin_lookup[] = { 1, 2, 4, 8, 16, 32, 64, 128 };

	cout << preamble<< ' ';
	for (int i = sizeof(T)-1; i > -1; i--)
	{
		for (int x = 7; x > -1; x--)
		{
			UCHAR tchar = reinterpret_cast<UCHAR*>(&arg)[i];
			if ((tchar ^ bin_lookup[x]) < tchar)
			{
				cout << '1';
			}
			else{ cout << '0'; }
		}
		cout << ' ';
	}
	cout << endl;
}


template<typename T>
T BinToValue(const char* bin_arg)
{
	T val = 0;
	int iter = 0, len = strlen(bin_arg);

	for (int i = sizeof(T) -1; i > -1; i--)
	{
		for (int x = 7; x > -1; )
		{
			UCHAR* tchar = &reinterpret_cast<UCHAR*>(&val)[i];
			if (iter < len)
			{
				switch (bin_arg[iter])
				{
				case '1':
					*tchar ^= _Pow_int<UCHAR>(2, x);
				case '0':
					x--;
					iter++;
					break;
				case ' ':
				default:
					iter++;
				}
			}
			else { break; }
		}
	}
	return val;
}


void utf8stest()
{

	const short bin_lookup[] = { 1, 2, 4, 8, 16, 32, 64, 128 };
	unsigned base = 256, enc = 0x80808080, n = 0x0;


	n = 0xb6b6b6b6;

	unsigned t0, t1, t2;
	cout << hex << "Stage 1: " << (t0 = BinToValue<unsigned>("11101100 10111000 10100001 10101010"));
	PrintAsBin<unsigned>("----", t0);
	cout << hex << "Stage 2: " << (t1 = BinToValue<unsigned>("00001010 00111000 00100001 00101010"));
	PrintAsBin<unsigned>(" ---- ", t1);
	PrintAsBin<unsigned>("s1-s2: ", t0 - t1);
	cout << hex << t0 - t1 << "\n";
	cout << hex << "Stage 3: " << (t2 = BinToValue<unsigned>("00000000 00000111 00001000 01101010"));
	PrintAsBin<unsigned>("----", t2);
	PrintAsBin<unsigned>("s2 - s3: ", t1 - t2);
	PrintAsBin<unsigned>("s1-s3: ", t0 - t2);
	cout << hex << t0 - t2 << endl;

	cin.get();
}


enum ascii_char : const char
{
	CHAR_LOWER_A = 'a',
	CHAR_LOWER_B = 'b',
	CHAR_LOWER_C = 'c',
	CHAR_LOWER_D = 'd',
	CHAR_LOWER_E = 'e',
	CHAR_LOWER_F = 'f',
	CHAR_LOWER_G = 'g',
	CHAR_LOWER_H = 'h',
	CHAR_LOWER_I = 'i',
	CHAR_LOWER_J = 'j',
	CHAR_LOWER_K = 'k',
	CHAR_LOWER_L = 'l',
	CHAR_LOWER_M = 'm',
	CHAR_LOWER_N = 'n',
	CHAR_LOWER_O = 'o',
	CHAR_LOWER_P = 'p',
	CHAR_LOWER_Q = 'q',
	CHAR_LOWER_R = 'r',
	CHAR_LOWER_S = 's',
	CHAR_LOWER_T = 't',
	CHAR_LOWER_U = 'u',
	CHAR_LOWER_V = 'v',
	CHAR_LOWER_W = 'w',
	CHAR_LOWER_X = 'x',
	CHAR_LOWER_Y = 'y',
	CHAR_LOWER_Z = 'z',
};

enum ascii_symbol : char
{
	CHAR_FORWARD_SLASH = '/'
};


typedef void* CLASS;




template<const char ...dlim>
std::vector<string> 
Filter(const char* str, const unsigned str_length)
{
	
}





int _tmain(int argc, _TCHAR* argv [])
{
	T1();
	cin.get();
}











