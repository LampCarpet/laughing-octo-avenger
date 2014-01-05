#pragma once
#include "stdafx.h"

/*

	----------------------------------
	|Prefix(s) | REX code (x64) | OpCode
    ----------------------------------
	 1-4 bytes |  1 byte        | 
				 (optional)
*/




namespace x86_64_instruction_base
{
	// A mandatory prefix may be used to express distinct funcionality
	// and should be place after an optional one, with the exception of REX
	
	

	typedef Types::byte Instruction_Prefix_Group;

	struct Instruction_Prefix
	{

	};

	enum InstPrefix_Group_1 : Instruction_Prefix_Group
	{
		//The LOCK prefix forces an operation that ensures exclusive use of shared 
		//memory in a multiprocessor environmen
		LOCK = 0xf0,
		// use only with MOVS, CMPS, SCAS, LODS, STOS, INS, and OUTS
		// Some instructions may use f2 and f3 to express distinct functionality
		REPNE_REPNZ = 0xf2,
		REP_REPE_REPZ = 0xf3
	};

	enum Optional :Types::u8
	{
		INCLUDED = 1,
		Excluded = 0
	};

	enum InstPrefix_Group_2 : Instruction_Prefix_Group
	{
		// use with any branch instruction is reserved
		CS_SEG_OVERRIDE = 0x2e,
		SS_SEG_OVERRIDE = 0x36,
		DS_SEG_OVERRIDE = 0x3e,
		ES_SEG_OVERRIDE = 0x26,
		FS_SEG_OVERRIDE = 0x64,
		GS_SEG_OVERRIDE = 0x65,
		//use only with jcc
		BRANCH_NOT_TAKEN = 0x2e,
		BRANCH_TAKEN = 0x3e
	};

	enum InstPrefix_Group_3 : Instruction_Prefix_Group
	{
		//The operand size override prefix allows a program to switch 
		//between 16 and 32 bit operand sizes
		//Some SSE2/SSE3/SSSE3/SSE4 instructions and instructions using a three-byte 
		//sequence of primary opcode bytes may use 66H as a mandatory prefix to express
		//distinct functionality
		OPERAND_SIZE_OVERRIDE = 0x66
	};

	enum InstPrefix_Group_4 : Instruction_Prefix_Group
	{
		//The address-size override prefix allows programs to switch between 16 and 32 bit addressing
		// Either size can be the default the prefix selects the non default size
		ADDRESS_SIZE_OVERRIDE = 0x67
	};

	enum Opcode_Constant : Types::byte
	{
		OPCODE_PREFIX_0x66 = 0x66,
		OPCODE_PREFIX_0xF2 = 0xF2,
		OPCODE_PREFIX_0xF3 = 0xF3
	};

	//Not all instructions require a REX prefix in 64-bit mode. A prefix is necessary only if 
	//an instruction references one of the extended registers or uses a 64 bit operand
	//When a REX prefix is used in conjunction with an instruction containing a mandatory prefix, the
	//mandatory prefix must come before the REX so the REX prefix can be immediately
	//preceding the opcode or the escape byte.
	enum RexPrefix_Optional_x64 : Types::byte
	{
		NO_REX_PREFIX = 0,
		HAS_REX_PREFIX = 1
	};

	enum  PrefixGroupDescriptor : Types::byte
	{
		NO_PREFIX = 0,
		SINGLE_PREFIX = 1,
		DOUBLE_PREFIX = 2,
		TRIPLE_PREFIX = 3,
		QUAD_PREFIX = 4,
	};



	struct BYTE_ALIGN
	Rex_Prefix_Form
	{
		volatile Types::byte _p : 4;
		volatile Types::byte _w : 1;
		volatile Types::byte _r : 1;
		volatile Types::byte _x : 1;
		volatile Types::byte _b : 1;
	};


	struct BYTE_ALIGN
	Mod_Reg_RM_Form 
	{
		volatile Types::byte mod : 2;
		volatile Types::byte reg : 3;
		volatile Types::byte rm : 3;
	};

	struct BYTE_ALIGN 
	Scale_Index_Base_Form
	{
		volatile Types::byte scale : 2;
		volatile Types::byte index : 3;
		volatile Types::byte base : 3;
	};

	enum OpCode_Type : Types::u8
	{
		NO_OP = 0,
		SINGLE_BYTE_OPCODE = 1,
		ESCAPED_OPCODE = 2,
		DOUBLE_BYTE_OPCODE = 2,
		PREFIXED_OPCODE = 3,
		TRIPLE_BYTE_OPCODE = 3
	};
	
	template<OpCode_Type optype>
	struct BYTE_ALIGN
	OpCode  
	{
		Types::byte op[optype];
	};

	namespace OpType
	{
		

		struct BYTE_ALIGN
		Single_Byte
		{
			Types::_byte_ operand;

		private:
			friend class InstBase;
			const static OpCode_Type opType = SINGLE_BYTE_OPCODE;
		};

		struct BYTE_ALIGN
		Escaped
		{
			volatile const Types::byte escape_opcode = 0x0f;
			volatile Types::_byte_ operand;

		private:
			friend class InstBase;
			const static OpCode_Type opType = ESCAPED_OPCODE;
		};


		struct BYTE_ALIGN
		Prefixed
		{
			volatile Opcode_Constant prefix;
			volatile const Types::byte escape_opcode = 0x0f;
			volatile Types::_byte_ operand;

		private:
			friend class InstBase;
			const static OpCode_Type opType = PREFIXED_OPCODE;
		};

		struct BYTE_ALIGN
		NoOperand
		{
		private:
			friend class InstBase;
			const static OpCode_Type opType = NO_OP;
		};
	}


	template<Instruction_Prefix_Group length>
	struct BYTE_ALIGN
	Instruction_prefix
	{
		Instruction_Prefix_Group prefixes[length];

	private:
		friend class InstBase;
	};


	enum Displacement_Type : Types::u8
	{
		NO_DISPLACEMENT = 0,
		SINGLE_BYTE_DISPLACEMENT = 1,
		DOUBLE_BYTE_DISPLACEMENT = 2,
		TRIPLE_BYTE_DISPLACEMENT = 3
	};

	enum Immediate_Data_Size : Types::u8
	{
		NO_IMMEDIATE_DATA = 0,
		SINGLE_BYTE_IMMEDIATE = 1,
		DOUBLE_BYTE_IMMEDIATE = 2,
		QUAD_BYTE_IMMEDIATE = 4,
		OCTO_BYTE_IMMEDATE = 8
	};
	
	template<PrefixGroupDescriptor prefix_descriptor>
	struct BYTE_ALIGN
	Prefix_Group_Wrapper
	{
		struct BYTE_ALIGN
		{
		private:
			Types::byte data[prefix_descriptor];
		}Prefix_Group;
	};

	template<OpCode_Type op_type>
	struct BYTE_ALIGN
	OpCode_Wrapper
	{
		struct BYTE_ALIGN
		{
		private:
			Types::byte data[op_type];
		}OpCode;
	};

	template<Optional option>
	struct BYTE_ALIGN
	Optional_Mod_Reg_Wrapper
	{
		struct BYTE_ALIGN
		{
			private:
				Mod_Reg_RM_Form data[option];
		}Mod_Reg_RM;
	};

	template<Displacement_Type size>
	struct BYTE_ALIGN
	Displacement_Wrapper
	{
		struct BYTE_ALIGN
		{
		private:
			Types::_byte_ disp[size];
		}Displacement;
	};	

	template<Optional option>
	struct BYTE_ALIGN
	Optional_Rex_Wrapper
	{
		struct BYTE_ALIGN
		{
			private:
			Rex_Prefix_Form form[option];
		}Rex_Prefix;
	};

	template<Immediate_Data_Size imm_size>
	struct BYTE_ALIGN
	Immediate_Data_Wrapper
	{
		struct BYTE_ALIGN
		{
		private:
			Types::byte data[imm_size];
		}Immediate_Data;
	};

	template<Optional option>
	struct BYTE_ALIGN
	Optional_SIB_Wrapper
	{
		struct BYTE_ALIGN
		{
		private:
			Scale_Index_Base_Form data [option];
		}Scale_Index_Base;
	};

	template<
		PrefixGroupDescriptor prefixes,
		Optional x64_inst_reg,
		OpCode_Type op_code,
		Optional sib_opt,
		Optional mod_rm_reg,
		Displacement_Type disp_len,
		Immediate_Data_Size imm_len
	>
	struct BYTE_ALIGN
	Instruction_Prototype : Prefix_Group_Wrapper<prefixes>,
							Optional_Rex_Wrapper<x64_inst_reg>,
							OpCode_Wrapper<op_code>,
							Optional_Mod_Reg_Wrapper<mod_rm_reg>,
							Optional_SIB_Wrapper<sib_opt>,
							Displacement_Wrapper<disp_len>,
							Immediate_Data_Wrapper<imm_len>
	{

	};



}





















