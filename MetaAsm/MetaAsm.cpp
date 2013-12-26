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
	Rex_Prefix_Form r;
	
	inst_a<InstPrefix_Group_1::LOCK, OpcodeType::SINGLE_BYTE_OPCODE,r, SINGLE_BYTE_DISPLACEMENT,SINGLE_BYTE_IMMEDIATE>();
	

	cout << endl;
	
	cin.get();
	return 0;
}

