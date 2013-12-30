#include "stdafx.h"
#include "x86_64_instruction_base.h"
#include "x86_64_instruction_set.h"

#include <memory>
#include <iostream>

using namespace std;
using namespace x86_64_instruction_base;
using namespace x86_64_instruction_set;

class ExecEnv
{
public:
	
private:
};





int _tmain(int argc, _TCHAR* argv[])
{
	Instruction<PrefixGroupDescriptor::SINGLE_PREFIX,
				RexPrefix_Optional_x64::HAS_REX_PREFIX,
				OpCode_Type::SINGLE_BYTE_OPCODE,
				SINGLE_BYTE_DISPLACEMENT, 
				SINGLE_BYTE_IMMEDIATE> i;
	
	
	cout << sizeof(i);
	cout << endl;
	
	cin.get();
	return 0;
}

