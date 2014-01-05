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
	Immediate_Data_Size::OCTO_BYTE_IMMEDATE> InstructionBuffer;

class InstructionPool
{

};


class ExecEnv
{
public:
	
private:
};

void CacheTesting()
{
	utilities::FastCache<InstructionBuffer> cache(Types::u8(2), Types::u8(20), false, false);
	InstructionBuffer *t;

	for (unsigned i = 0; i < 10000; i++)
	{
		t = cache.AllocateObject();
	}

	cout << "Objects remaing: " << cache.GetObjectsRemaining() << endl;

	cache.FlushCachePage();

	cout << "Objects remaing: " << cache.GetObjectsRemaining() << endl;

	cache.FlushCache();

	cout << "Objects remaing: " << cache.GetObjectsRemaining() << endl;

}

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
	CacheTesting();

	cin.get();
	return 0;
}











