#pragma once
#include "stdafx.h"
#include "x86_64_instruction_base.h"




namespace x86_64_instruction_set
{
	using namespace x86_64_instruction_base;

	typedef Instruction_Prototype<
			PrefixGroupDescriptor::NO_PREFIX,
			Optional::EXCLUDED,
			OpCodeType::NO_OP,
			Optional::EXCLUDED,
			Optional::EXCLUDED,
			DisplacementType::NO_DISPLACEMENT,
			ImmediateDataSize::NO_IMMEDIATE_DATA>
		
			EmptyInstruction;
	


	void T1()
	{
		Types::_byte_ bt;
		
		EmptyInstruction e;
		
		std::cout << std::endl;
		std::cout << sizeof(e) << std::endl;
	}





	

}























