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
	Instruction_Prototype<PrefixGroupDescriptor::SINGLE_PREFIX,
				Optional::INCLUDED,
				OpCode_Type::SINGLE_BYTE_OPCODE,
				Optional::INCLUDED,
				Optional::INCLUDED,
				SINGLE_BYTE_DISPLACEMENT, 
				SINGLE_BYTE_IMMEDIATE> i;
	
	
	cout << sizeof(i);
	cout << endl;
	
	cin.get();
	return 0;
}

