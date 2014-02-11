#pragma once
#include "stdafx.h"




namespace tmp_common
{

	template<bool condition, class Then, class Else>
	struct 
	IF
	{
		typedef Then RET;
	};

	template <class Then, class Else>
	struct
	IF<false, Then, Else>
	{
		typedef Else RET;
	};

	template <unsigned Case, class Action>
	struct CASE : Action
	{
		const static unsigned value = Case;
	};

	template<unsigned param, class ...Then>
	struct SWITCH
	{

	};

}