/*---------------------------------------------------------------------
PANI - projekt
---------------------------------------------------------------------*/

#include "stdafx.h"
#include "porownaj.h"

bool porownaj(const std::string & str1, const std::string & str2)
{
	for (size_t i = 0; i < str1.size(); i++)
	{
		{
			if (toupper(str1[i]) != toupper(str2[i]))
				return false;
		}
	}
	return true;
}