#include "stdafx.h"
#include "x86_64_instruction_set.h"
#include "Utilities.h"

using namespace std;
using namespace x86_64_instruction_base;
using namespace x86_64_instruction_set;


typedef Instruction_Prototype<PrefixGroupDescriptor::QUAD_PREFIX,
	Optional::INCLUDED,
	OpCode_Type::TRIPLE_BYTE_OPCODE,
	Optional::INCLUDED,
	Optional::INCLUDED,
	Displacement_Type::TRIPLE_BYTE_DISPLACEMENT,
	Immediate_Data_Size::QUAD_BYTE_IMMEDIATE> InstMax;


class ExecEnv
{
public:
	
private:
};


void InstTesting()
{
	Instruction_Prototype<PrefixGroupDescriptor::SINGLE_PREFIX,
		Optional::INCLUDED,
		OpCode_Type::SINGLE_BYTE_OPCODE,
		Optional::INCLUDED,
		Optional::INCLUDED,
		SINGLE_BYTE_DISPLACEMENT,
		SINGLE_BYTE_IMMEDIATE> i;


	cout << sizeof(i);
	cout << endl;
}


int _tmain(int argc, _TCHAR* argv[])
{
	utilities::FastCache<InstMax> cache(2,20,false);
	InstMax *t;

	for (unsigned i = 0; i < 10000; i++)
	{
		t = cache.Alloc();
	}
	
	cout << "Bytes allocated: "<<cache.GetByteAllocated()<<endl;
	cout << "Bytes available: " << cache.GetByteAllocated() << endl<<endl;
	
	cache.FlushCachePage();

	cout << "Bytes allocated: " << cache.GetByteAllocated() << endl;
	cout << "Bytes available: " << cache.GetByteAllocated() << endl << endl;

	cache.FlushCache();

	cout << "Bytes allocated: " << cache.GetByteAllocated() << endl;
	cout << "Bytes available: " << cache.GetByteAllocated() << endl;

	cin.get();
	return 0;
}











