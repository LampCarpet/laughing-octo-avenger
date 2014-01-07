#pragma once
#include "stdafx.h"
#include "stdtwo.h"


namespace utilities
{

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

	typedef struct BYTE_ALIGN
	CompressedPtr
	{
	private:
		Types::u8 page;
		Types::u8 offset[3];
	};

	template<typename T>
	class FastCache
	{
		enum Flags : Types::u8
		{
			CAN_EXECUTE = 0x1, IS_INIT = 0x2, USE_STRICT = 0x4
		};

#ifdef DEBUG
		void ValidateList(const char* function)
		{
			cout <<endl<< function << endl;
			cout << "_pages_allocated: " << (unsigned)_pages_allocated << endl;
			cout << "_objects_available: " << (unsigned)_objects_available << endl;
			_ll_node *temp = &_root;
			for (unsigned i = 0; i < _pages_allocated; i++)
			{
				if (temp != nullptr)
				{
					if (temp->next != nullptr)
					{
						if (temp != temp->next->prev)
						{
							cout << "invalid prev link at: " << i + 1<<endl;
						}
					}
					else if (i == _pages_allocated){  break; }
					else { cout << "invalid next link at: " << i << endl; break; }
				}
				else
				{
					cout << "invalid object at: " << i <<endl;
				}
			}
			temp = temp->next;
		}
#endif 

	public:
		FastCache() : _pages_allocated(0), _max_pages(0), _objects_available(0)
		{
			_c_page = &_root;
			_n_page = _c_page;
			_opt_flg = 0;
		}


		FastCache(Types::u8 initial_pages, Types::u8 max_pages, Types::u8 use_strict_max, Types::u8 can_execute)
		: _pages_allocated(initial_pages), 
		_max_pages(max_pages),
		_opt_flag(0)
		{
			flag = PAGE_READWRITE;
			if (can_execute){ flag = PAGE_EXECUTE_READWRITE; SetCanExecute(true); }
			if (use_strict_max){ SetStrictMax(true); }

			if (_pages_allocated > _max_pages) { _pages_allocated = _max_pages; }

			if (_pages_allocated > 0)
			{
				_ll_node *temp = &_root;
				temp->page = static_cast<Types::u8*>(VirtualAlloc(NULL, 4096, MEM_COMMIT, flag));
				temp->next = static_cast<_ll_node*>(VirtualAlloc(NULL, sizeof(_ll_node), MEM_COMMIT, flag));
				temp->objects_allocd = 0;
				_ll_node* hist = &_root;
				temp = temp->next;
				temp->prev = nullptr;

				for (unsigned i = 1; i < _pages_allocated; i++)
				{
					temp->prev = hist;
					temp->page = static_cast<Types::u8*>(VirtualAlloc(NULL, 4096, MEM_COMMIT, flag));
					temp->next = static_cast<_ll_node*>(VirtualAlloc(NULL, sizeof(_ll_node), MEM_COMMIT, flag));
					temp->objects_allocd = 0;
					
					hist = temp;
					temp = temp->next;
					temp->prev = hist;
				}

				_objects_available = (4096 * _pages_allocated)/sizeof(T);
				_c_page = &_root;
				_n_page = temp;
				SetIsInit(true);
			}


		}
		~FastCache()
		{
			_ll_node* temp = &_root;

			for (unsigned i = 0; i < _pages_allocated; _pages_allocated--)
			{
				VirtualFree(temp->page, 4096, MEM_FREE);
				_objects_available -= temp->objects_allocd;
				temp = temp->next;
				VirtualFree(temp, sizeof(_ll_node), MEM_FREE);
			}
		}



		void SetCanExecute(bool exec)
		{
			_set_flag(Flags::CAN_EXECUTE, exec);
		}

		void SetStrictMax(bool strict_max)
		{
			_set_flag(Flags::USE_STRICT, strict_max);
		}

		void SetIsInit(bool is_init)
		{
			_set_flag(Flags::IS_INIT,is_init);
		}

		void AddPage()
		{
			_ll_node *tmp = _c_page;
			for (Types::u8 i = 0; i < _pages_allocated; i++)
			{
				if (tmp->next == nullptr)
				{
					break;
				}
				else
				{
					tmp = tmp->next;
				}
			}

			tmp->next = static_cast<_ll_node*>(VirtualAlloc(NULL, sizeof(_ll_node), MEM_COMMIT, flag));;
			_n_page = tmp->next;
			_n_page->page = static_cast<Types::u8*>(VirtualAlloc(NULL, 4096, MEM_COMMIT, flag));
			_n_page->next = nullptr;
			_n_page->objects_allocd = 0;
			_n_page->prev = tmp;
			_pages_allocated++;
			_objects_available += 4096/sizeof(T);
		}

		void FlushCachePage()
		{
			_objects_available -= _c_page->objects_allocd;
			ZeroMemory(_c_page->page, 4096);
			_c_page->objects_allocd = 0;
		}

		int FlushCache()
		{
			_ll_node* temp = &_root;
			for (unsigned i = 0; i < _pages_allocated-1; i++)
			{
				ZeroMemory(temp->page, 4096);
				temp->objects_allocd = 0;
				temp = temp->next;
			}

			_c_page = &_root;
			_objects_available = (_pages_allocated * 4096)/sizeof(T);
			return _objects_available;
		}

		T* AllocateObject()
		{
			if (((_c_page->objects_allocd + 1)*sizeof(T)) <= 4096)
			{
				_c_page->objects_allocd++;
				_objects_available--;
				return reinterpret_cast<T*>(_c_page->page + (_c_page->objects_allocd*sizeof(T)));
			}
			else
			{
				_ll_node *_prev_node = _c_page;
				if (_pages_allocated < _max_pages && !_get_flag(Flags::USE_STRICT))
				{
					AddPage();
					_c_page->next = _n_page;
					_n_page->prev = _c_page;
					_c_page = _n_page;
					
					_c_page->objects_allocd++;
					_objects_available--;
					return reinterpret_cast<T*>(_c_page->page + (_c_page->objects_allocd*sizeof(T)));
				}
				else
				{
					return nullptr;
				}
			}
		}

		unsigned GetObjectsRemaining() const
		{
			return _objects_available;
		}


	private:
		bool _get_flag(Flags get)
		{
			if ((_opt_flag^get) > _opt_flag)
			{
				return true;
			}
			else { return false; }
		}

		void _set_flag(Flags set, bool state)
		{
			//if state is desired to be true and flag is false 
			//flip bit
			if (state && !_get_flag(set)) 
			{
				_opt_flag^=set;
			}
			if (!state && _get_flag(set))
			{
				_opt_flag^=set;
			}
		}

		_ll_node _root, *_c_page, *_n_page;
		Types::u8 _opt_flag, _pages_allocated, _max_pages, _objects_available;
		int flag;
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