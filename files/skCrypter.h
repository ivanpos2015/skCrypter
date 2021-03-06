#pragma once

/*____________________________________________________________________________________________________________

Original Author: skadro
Github: https://github.com/skadro-official
License: See end of file

skCrypter
		Compile-time, Usermode + Kernelmode, safe and lightweight string crypter library for C++11+

							*Not removing this part is appreciated*
____________________________________________________________________________________________________________*/

#ifdef _KERNEL_MODE
	namespace std
	{
		// STRUCT TEMPLATE remove_reference
		template <class _Ty>
		struct remove_reference {
			using type = _Ty;
			using _Const_thru_ref_type = const _Ty;
		};

		template <class _Ty>
		using remove_reference_t = typename remove_reference<_Ty>::type;

		// STRUCT TEMPLATE remove_const
		template <class _Ty>
		struct remove_const {
			using type = _Ty;
		};

		template <class _Ty>
		using remove_const_t = typename remove_const<_Ty>::type;
	}
#else
	#include <xtr1common>
#endif

template<class _Ty>
using clean_type = typename std::remove_const_t<std::remove_reference_t<_Ty>>;

namespace skc
{
	template <int _size, char _key1, char _key2, typename T>
	class skCrypter
	{
	public:
		constexpr skCrypter(T* data)
		{		
			crypt(data);
		}

		T* get()
		{
			return _storage;
		}

		int size() // (w)char count
		{
			return _size;
		}

		char key()
		{
			return _key1;
		}

		T* encrypt()
		{
			if (!isEncrypted())
				crypt(_storage);

			return _storage;
		}

		T* decrypt()
		{
			if (isEncrypted())
				crypt(_storage);

			return _storage;
		}

		bool isEncrypted()
		{
			return _storage[_size - 1] != 0;
		}

		void clear() // set full storage to 0
		{
			for (int i = 0; i < _size; i++)
			{
				_storage[i] = 0;
			}
		}

		operator T* ()
		{
			decrypt();

			return _storage;
		}
		
	private:
		constexpr void crypt(T* data)
		{
			for (int i = 0; i < _size; i++)
			{
				_storage[i] = data[i] ^ (_key1 + i % _key2);
			}
		}	

		T _storage[_size]{};
	};
}
#define skCrypt(str) skCrypt_key(str, __TIME__[4], __TIME__[7])
#define skCrypt_key(str, key1, key2) []() { \
			constexpr static auto crypted = skc::skCrypter \
				<sizeof(str) / sizeof(str[0]), key, key2, clean_type<decltype(str[0])>>((clean_type<decltype(str[0])>*)str); \
					return crypted; }()

/*________________________________________________________________________________

MIT License

Copyright(c) 2020 skadro

Permission is hereby granted, free of charge, to any person obtaining a copy
of this softwareand associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright noticeand this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

________________________________________________________________________________*/
