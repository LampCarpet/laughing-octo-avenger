#pragma once
#include "stdafx.h"
#include "x86_64_instruction_base.h"





namespace x86_64_instruction_set
{
	using namespace x86_64_instruction_base;

	template<
		      Instruction_Prefix ...prefixes,
			  op o,
			  Rex_Prefix_Form x64_inst_reg,
			  Displacement_Type disp_len, 
			  Immediate_Data_Size imm_len, 
			>
	auto inst_a() -> Instruction<
								  prefixes...,
								  o,
								  x64_inst_reg,
								  Mod_Reg_RM_Form,
								  Scale_Index_Base_Form,
								  Displacement<disp_len>, 
								  Immediate<imm_len>
								>
	{

	}

	

}