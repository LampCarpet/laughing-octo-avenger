/*
#pragma once
#include "stdafx.h"
#include "Utilities.h"
#include "stdtwo.h"


using namespace std;

namespace WinCryptHandler
{

	void WinError(LPTSTR lpszFunction, DWORD dw)
	{
		// Retrieve the system error message for the last-error code

		LPVOID lpMsgBuf;
		LPVOID lpDisplayBuf;

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0, NULL);

		// Display the error message and exit the process

		lpDisplayBuf = (LPVOID) LocalAlloc(LMEM_ZEROINIT,
			(lstrlen((LPCTSTR) lpMsgBuf) + lstrlen((LPCTSTR) lpszFunction) + 40) * sizeof(TCHAR));
		StringCchPrintf((LPTSTR) lpDisplayBuf,
			LocalSize(lpDisplayBuf) / sizeof(TCHAR),
			TEXT("%s failed with error %d: %s"),
			lpszFunction, dw, lpMsgBuf);

		wcerr << (wchar_t*) lpDisplayBuf << endl;

		cin.get();
		LocalFree(lpMsgBuf);
		LocalFree(lpDisplayBuf);
	}

	class BoundArray
	{
	public:
		BoundArray(BoundArray& ba)
		{
			_size = ba._size;
			_data = make_shared<BYTE>(ba._data);
		}

		BoundArray(unsigned size) : _size(size), _data(new BYTE[size]){}

		BoundArray(BYTE* data, unsigned size) : _data(new BYTE[size]), _size(size)
		{
			if (memcpy_s(_data.get(), _size, data, size) > 0)
			{
				throw L"BoundArray() failed to copy memory.";
			}
		}

		~BoundArray()
		{
			_data.reset();
		}


		void Copy(unsigned offset, unsigned size, BYTE* data)
		{
			if ((offset + size) > _size)
			{
				throw L"Copy() argument out of range.";
			}
			else
			{
				unsigned tocpy = 0;

				if (size > offset) { tocpy = size - offset; }
				else{ tocpy = offset - size; }

				if (memcpy_s(_data.get() + offset, tocpy, data, tocpy) > 0)
				{
					throw L"BoundArray() failed to copy memory.";
				}
			}
		}

		BYTE* GetPtr()
		{
			return _data.get();
		}

		unsigned GetSize()
		{
			return _size;
		}

	private:
		BoundArray(){};

		shared_ptr<BYTE> _data;
		unsigned _size;
	};

	template<DWORD prov_type, unsigned max_size, DWORD ...flags>

	class WinCryptHandle : utilities::FastCache<>
	{
	public:
		WinCryptHandle(wchar_t* key_container_name, wchar_t* provider) 
			:_provider(provider),
			_container(key_container_name),
			_provT(prov_type),
			_fl(sizeof...(flags)),
			_hCrypt(NULL)
		{
			DWORD flga[] = { flags... };
			DWORD _res = 0;
			for (DWORD i = 0; i < _fl; i++){ _res |= flga[i]; }
			
			if (!CryptAcquireContext(&_hCrypt, _container.c_str(), NULL, prov_type, _res))
			{
				if (!CryptAcquireContext(&_hCrypt, _container.c_str(), NULL, prov_type, CRYPT_NEWKEYSET))
				{
					WinError(L"WinCrypt", GetLastError());
					throw L"WinCrypt Failed To Aquire Context.";
				}
			}

			if (!CryptGetUserKey(_hCrypt, AT_SIGNATURE, &_sigKey))
			{
				if (!CryptGenKey(_hCrypt, AT_SIGNATURE, 0, &_sigKey))
				{
					WinError(L"CryptGenKey(sig)", GetLastError());
					throw L"CryptGenRandom() exception.";
				}
			}

			if (!CryptGetUserKey(_hCrypt, AT_KEYEXCHANGE, &_exKey))
			{
				if (!CryptGenKey(_hCrypt, AT_KEYEXCHANGE, 0, &_exKey))
				{
					WinError(L"CryptGenKey(ex)", GetLastError());
					throw L"CryptGenRandom() exception.";
				}
			}
			this->AddPage();
		}

		~WinCryptHandle()
		{
			if (_sigKey)
			{
				if (!CryptDestroyKey(_sigKey))
				{
					WinError(L"~WinCrypt()", GetLastError());
				}
				_sigKey = NULL;
			}
			if (_exKey)
			{
				if (!CryptDestroyKey(_exKey))
				{
					WinError(L"~WinCrypt()", GetLastError());
				}
				_exKey = NULL;
			}
			if (_hCrypt)
			{
				if (!CryptReleaseContext(_hCrypt, 0))
				{
					WinError(L"~WinCrypt()", GetLastError());
				}
				_hCrypt = NULL;
			}


		}



		string GenRandomString(unsigned lowerBound, unsigned upperBound)
		{

			std::uniform_int_distribution<unsigned> distribution(lowerBound,upperBound);
			unsigned size = distribution(eng);

			if (size > max_size) { throw L"GenRandomString() size argument out of range."; }

			u8* ptr = &Alloc();

			if (!CryptGenRandom(_hCrypt, size, ptr))
			{
				WinError(L"GenRandomString()", GetLastError());
				throw L"CryptGenRandom() exception.";
			}

			string l(reinterpret_cast<char*>(ptr), size);
			FlushCachePage();
			return l;
		}


	private:
		size_t _fl;
		DWORD _provT;
		wstring _provider, _container;
		HCRYPTPROV _hCrypt;
		HCRYPTKEY _sigKey, _exKey;
		std::default_random_engine eng;
	};




}

*/