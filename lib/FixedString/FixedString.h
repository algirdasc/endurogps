#ifndef _FIXED_STRING_H
#define _FIXED_STRING_H

#include <Arduino.h>
#include <pgmspace.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>

extern bool FixedString_OverflowDetected;

#pragma pack(push, 1)

class FixedStringBase
{
private:
	uint16_t _length;
	uint16_t _capacity;
	bool AssertSpaceAvailable(int requestedLengthToAdd)
	{
		if (_length + requestedLengthToAdd > capacity())
		{
			FixedString_OverflowDetected = true;
			return false;
		}
		return true;
	}
	char* _str() const
	{
		return (char*)this + sizeof(FixedStringBase);
	}
public:
	FixedStringBase(uint16_t capacity)
	{
		_capacity = capacity;
	}
	const char* c_str() const
	{
		return _str();
	}
	const unsigned int capacity()
	{
		return _capacity;
	}
	
	const unsigned int length() const
	{
		return _length;
	}

	const unsigned int freeBytes() const
	{
		return _capacity - length();
	}	

	const char operator[](int index)
	{
		if (index < 0)
		{
			return 0;
		}
		if (index > length())
		{
			return 0;
		}
		return _str()[index];
	}

	FixedStringBase& operator=(const char *str)
	{
		clear();
		append(str);
		return *this;
	}
	
	FixedStringBase& operator+=(const char *str)
	{
		append(str);
		return *this;
	}
	FixedStringBase& operator =(const FixedStringBase& str)
	{
		clear();
		FixedStringBase& strWithoutConst = const_cast<FixedStringBase&>(str);
		append(const_cast<const char*>(strWithoutConst._str()), str.length());
		return *this;
	}
	FixedStringBase& operator +=(FixedStringBase& str)
	{
		append(str._str(), str.length());
		return *this;
	}
	
	void clear()
	{
		_length = 0;
		memset(_str(), 0, capacity() + 1);
	}

	bool append(const char* str, unsigned int len)
	{
		if (!AssertSpaceAvailable(len))
		{
			return false;
		}
		memcpy(_str() + _length, str, len);
		_length += len;
		return true;
	}
	bool append(FixedStringBase& str)
	{
		return append(str.c_str(), str.length());
	}

	bool append(const __FlashStringHelper* str, unsigned int len)
	{
		if (!AssertSpaceAvailable(len))
		{
			return false;
		}
		memcpy_P(_str() + _length, str, len);
		_length += len;
		return true;
	}
	bool append(const char* str)
	{
		const unsigned int len = strlen(str);
		return append(str, len);
	}
	bool append(const __FlashStringHelper* str)
	{
		const unsigned int len = strlen_P((PGM_P)str);
		return append(str, len);
	}
	bool append(char c)
	{
		if (!AssertSpaceAvailable(1))
		{
			return false;
		}
		_str()[_length] = c;
		_length++;
		return true;
	}

	bool operator==(const char* str)
	{
		return equals(str);
	}

	bool equals(const char* other)
	{
		auto otherLength = strlen(other);
		if (otherLength != _length)
		{
			return false;
		}
		return memcmp(c_str(), other, _length) == 0;
	}

	bool equals(const FixedStringBase& other)
	{
		auto otherLength = other.length();
		if (otherLength != _length)
		{
			return false;
		}
		return memcmp(c_str(), other.c_str(), _length) == 0;
	}
#if !TEST
	bool operator==(const __FlashStringHelper* other)
	{
		return equals(other);
	}
	bool equals(const __FlashStringHelper *other)
	{
		auto otherLength = strlen_P((PGM_P)other);
		if (otherLength != _length)
		{
			return false;
		}
		return memcmp_P(c_str(), other, _length) == 0;
	}
	bool startsWith(const __FlashStringHelper* other)
	{
		auto otherLength = strlen_P((PGM_P)other);
		if (otherLength > _length)
		{
			return false;
		}
		return memcmp_P(c_str(), other, otherLength) == 0;
	}

	bool endsWith(const __FlashStringHelper* other)
	{
		auto otherLength = strlen_P((PGM_P)other);
		if (otherLength > _length)
		{
			return false;
		}
		return memcmp_P(c_str() + _length - otherLength, other, otherLength) == 0;
	}
#endif

	bool startsWith(const char* other)
	{
		auto otherLength = strlen(other);
		if (otherLength > _length)
		{
			return false;
		}
		return memcmp((char*)c_str(), other, otherLength) == 0;
	}

	void replace(char toReplace, char replaceWith)
	{
		for(size_t i =0; i < length(); i++)
		{
			if(_str()[i] == toReplace)
			{
				_str()[i] = replaceWith;
			}
		}
	}
	void trimStart(char c)
	{
		size_t charactersToTrim = 0;
		while(charactersToTrim < length() && c_str()[charactersToTrim] == c)
		{
			charactersToTrim++;
		}
		for(size_t i =0; i < length(); i++)
		{
			_str()[i] = _str()[i+charactersToTrim];
		}
		_length -= charactersToTrim;
		_str()[length()] = 0;
	}

	void appendFormat(const char *format, ...)
	{
		va_list argptr;
		va_start(argptr, format);
		auto remainingSpace = freeBytes();
		auto addedCharacters = vsnprintf((char*)c_str() + _length, remainingSpace, format, argptr);
		_length += addedCharacters;
		va_end(argptr);
	}

	void appendFormat(const __FlashStringHelper* format, ...)
	{
		va_list argptr;
		va_start(argptr, format);
		auto remainingSpace = freeBytes();
		auto addedCharacters = vsnprintf_P((char*)c_str() + _length, remainingSpace, (PGM_P)format, argptr);
		_length += addedCharacters;
		va_end(argptr);
	}


	void appendFormat(const char *format, va_list argptr)
	{		
		auto remainingSpace = freeBytes();
		auto addedCharacters = vsnprintf((char*)c_str() + _length, remainingSpace, format, argptr);
		_length += addedCharacters;
	}
	void appendFormatV(const __FlashStringHelper *format, va_list argptr)
	{
		auto remainingSpace = freeBytes();
		auto addedCharacters = vsnprintf_P((char*)c_str() + _length, remainingSpace, (PGM_P)format, argptr);
		_length += addedCharacters;
	}

	void debug()
	{
		printf("content: '%s' length: %d, free = %d\n", c_str(), length(), freeBytes());
		if (FixedString_OverflowDetected)
		{
			printf("Fatal error, string overun detected!\n");
		}
	}
};


template <unsigned int N>
class FixedString : public FixedStringBase
{
	char _string[N + 1]{};
public:
	const unsigned int capacity() const
	{
		return N;
	}

	FixedString():FixedStringBase(N)
	{
		clear();
	}

	FixedString(const char* str) :FixedStringBase(N)
	{
		clear();
		append(str);
	}
	FixedString(FixedStringBase& str) :FixedStringBase(N)
	{
		clear();
		append(str.c_str(), str.length());
	}
	FixedString(const FixedString& str) :FixedStringBase(N)
	{
		clear();
		append(str.c_str(), str.length());
	}

	FixedString<N> operator + (FixedStringBase& FixedString1)
	{
		FixedString<N> FixedString(*this);
		FixedString += FixedString1;
		return FixedString;
	}

	FixedString<N>& operator=(const __FlashStringHelper *str)
	{
		clear();
		append((PGM_P)str);
		return *this;
	}
};
typedef FixedString<16> FixedString16;
typedef FixedString<32> FixedString32;
typedef FixedString<64> FixedString64;
typedef FixedString<128> FixedString128;
typedef FixedString<256> FixedString256;
typedef FixedString<512> FixedString512;
typedef FixedString<1024> FixedString1024;

#pragma pack(pop)

#endif