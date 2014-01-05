// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <functional>
#include "Types.h"

using namespace Types;

#define BYTE_ALIGN _declspec(align(1))


template<byte T>
struct BYTE_ALIGN 
aligned_bytes
{
	_byte_ op[T];
};

template<typename T>
struct Node
{
	T t_ptr;
	Node *next, *prev;
};


class FastAlloc
{
public:



private:


};









template<typename T>
class AsyncRef
{
	struct _id
	{
		u16 _h_ref;
		u16 _uid;
		u16 _family;
		aligned_bytes<2> _flags;
	};

	struct _ref
	{
		Node* _ptr_table;
		u16* _ref_count;
		_id _this_id;
	};

public:
	AsyncRef(T& ptr, std::function onMerge) 
	{
		_ref->_ptr_table = new Node;
		_ref->_ptr_table->t_ptr = ptr;
		_ref->_ptr_table->next = nullptr;
		_ref->_ref_count = new u32;
		*_ref_count = 0;
	};

private:
	_ref* _root;


	AsyncRef()
	{
		if (_ptr_table != nullptr)
		{
			_ptr_table.next = new Node;
			_ptr_table = _ptr_table.next;
		}
		else { throw "shit"; }
		++_ref_count;
	}

	
};


// TODO: reference additional headers your program requires here










