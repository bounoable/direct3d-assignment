#pragma once
#include <Windows.h>
#include <cwchar>
#include <sstream>

inline LPCWSTR toDebugString(const char* str)
{
	std::wstringstream ss;
	ss << str;

	return ss.str().c_str();
}

inline LPCWSTR toDebugString(const int val)
{
	std::wstringstream ss;
	ss << val;

	return ss.str().c_str();
}