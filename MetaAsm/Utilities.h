#pragma once
#include "stdafx.h"

#include <Windows.h>


namespace utilities
{
	using namespace Types;

	template<Types::byte T>
	struct BYTE_ALIGN
		aligned_bytes
	{
		Types::_byte_ op[T];
	};
	
	struct _ll_node
	{
		Types::u8* page;
		unsigned short objects_allocd;
		_ll_node* next, *prev;
	};

	template<typename T>
	class FastCache
	{
	public:
		FastCache() : _pages_allocated(0), _max_pages(0), _can_exec(false), _is_init(false)
		{
			_c_page = &_root;
		}

		FastCache(unsigned initial_pages, unsigned max_pages, bool can_execute) : _pages_allocated(initial_pages), _can_exec(can_execute), _max_pages(max_pages), _is_init(true)
		{
			flag = PAGE_READWRITE;
			if (_can_exec){ flag = PAGE_EXECUTE_READWRITE; }
			
			if (_pages_allocated > _max_pages) { _pages_allocated = _max_pages}

			if (_pages_allocated > 0 && _pages_allocated  < 20)
			{
				_ll_node *temp = &_root;
				temp->prev = nullptr;
				temp->page = static_cast<u8*>(VirtualAlloc(NULL, 4096, MEM_COMMIT, flag));
				temp->objects_allocd = 0;
				temp->next = nullptr;

				for (unsigned i = 1; i < _pages_allocated; i++)
				{
					_ll_node* hist = temp;
					temp->next = static_cast<_ll_node*>(VirtualAlloc(NULL, sizeof(_ll_node), MEM_COMMIT, flag));
					temp = temp->next;
					temp->prev = hist;
					temp->page = static_cast<u8*>(VirtualAlloc(NULL, 4096, MEM_COMMIT, flag));
					temp->objects_allocd = 0;
					temp->next = nullptr;
				}
				_bytes_allocated = 4096 * _pages_allocated;
				_bytes_available = _bytes_allocated;
				_c_page = &_root;
			}


		}
		~FastCache()
		{
			_ll_node* temp = &_root;
			for (unsigned i = 0; i < _pages_allocated; i++)
			{
				VirtualFree(temp->page, 4096, MEM_FREE);
				temp = temp->next;
				VirtualFree(temp->prev, sizeof(_ll_node), MEM_FREE);
			}
		}

		bool AddPage()
		{
			if (_is_init)
			{

			}
			else
			{
				_c_page->page = temp->page = static_cast<u8*>(VirtualAlloc(NULL, 4096, MEM_COMMIT, flag));
				_c_page->next = static_cast<_ll_node*>(VirtualAlloc(NULL, sizeof(_ll_node), MEM_COMMIT, flag));
				_c_page->objects_allocd = 0;
				_c_page->prev = nullptr;
			}
		}

		void FlushCachePage()
		{
			ZeroMemory(_c_page->page, 4096);
			_byte_allocated -= _c_page->objects_allocd*sizeof(T);
			_bytes_available += _c_page->objects_allocd*sizeof(T)
			_c_page->objects_allocd = 0;
		}

		int FlushCache()
		{
			_ll_node* temp = _root;
			for (unsigned i = 0; i < _pages_allocated; i++)
			{
				ZeroMemory(temp->page, 4096);
				temp->objects_allocd = 0;
				temp = temp->next;
			}

			_c_page = &_root;
			_bytes_available = _pages_allocated * 4096;
			_bytes_allocated = 0;
			return _bytes_available;
		}

		T* Alloc()
		{
			if (((_c_page->objects_allocd + 1)*sizeof(T)) <= 4096)
			{
				_c_page->objects_allocd++; _bytes_available -= sizeof(T); _bytes_allocated += sizeof(T);
				return static_cast<T*>(_c_page->page + (_c_page->objects_allocd*sizeof(T)));
			}
			else
			{
				_ll_node *_prev_node = _c_page;
				if (_pages_allocated <= _max_pages)
				{
					_prev_node->next = temp->next = static_cast<_ll_node*>(VirtualAlloc(NULL, sizeof(_ll_node), MEM_COMMIT, flag));
					_c_page = _prev_node->next;
					_c_page->objects_allocd = 0;
					_c_page->page = temp->page = static_cast<u8*>(VirtualAlloc(NULL, 4096, MEM_COMMIT, flag));
					_c_page->prev = _prev_node;
					return Alloc();
				}
				else
				{
					return nullptr;
				}
			}
		}



	private:
		_ll_node _root, *_c_page;
		bool _can_exec, _is_init;
		unsigned _bytes_allocated, _bytes_available, _pages_allocated, _max_pages;
		DWORD flag;
	};









	template<typename T>
	class AsyncRef
	{
		struct _id
		{
			Types::u16 _h_ref;
			Types::u16 _uid;
			Types::u16 _family;
			aligned_bytes<2> _flags;
		};

		template<typename T>
		struct _node
		{
			T t_ptr;
			_id _this_id;
			_node *next, *prev;
		};

		template<typename T>
		struct _ref
		{
			_node<T>* _ptr_table;
			Types::u16* _ref_count;
		};

	public:
		AsyncRef(T& ptr, std::function onMerge)
		{
			_root->_ptr_table = new _node;
			_root->_ptr_table->t_ptr = ptr;
			_root->_ptr_table->next = nullptr;
			_root->_ref_count = new Types::u32;
			*_ref_count = 0;
		};

	private:
		_ref<T>* _root;
		_node<T>* _t_node;

		AsyncRef(AsyncRef& async)
		{
			if (_async._root != nullptr)
			{
				_root = _async._root;
				if (_async._root->_ptr_table.next == nullptr)
				{
					_root->_ptr_table.next = new _node;
					_t_node = _root->_ptr_table.next;
					_t_node->prev = _async._root->_ptr_table;
				}
				else { throw " "; }

			}
			else { throw "shit"; }
			++_ref_count;
		}

		bool incRefCount()
		{

		}

		bool decRefCount()
		{

		}

	};

}