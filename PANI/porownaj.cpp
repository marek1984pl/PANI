/*---------------------------------------------------------------------
PANI - projekt
---------------------------------------------------------------------*/

#include "stdafx.h"
#include "porownaj.h"

bool porownaj(const std::string & wzor, const std::string & str)
{
	for (unsigned i = 0; i < wzor.size(); i++)
		if (toupper(wzor[i]) != toupper(str[i]))
			return false;
	return true;
}