#ifndef __CORE__STRING_H__
#define __CORE__STRING_H__

#include <core/common.hpp>

//A string includes basic C string utilities and resizable data stuffs
class String {
public:


	static char* uFormat(const char* pFormat, ...) {
		return format(pFormat);
	}

	static string_size_t uLength_Basic(const char* pString) {
		string_size_t ret = 0;
		while (pString[ret++]);
		return ret;
	}
};

#endif