/*-----------------------------------------------------------------------
PANI - projekt
-----------------------------------------------------------------------*/

#include "stdafx.h"

#include "materialy.h"
#include "porownaj.h"

#include <string>

cMaterialy & cMaterialy::operator=(const cMaterialy & m)
{
	if (this == &m)
		return *this;
	ile = m.ile;
	materialy = new sMaterial(*m.materialy);
	return *this;
}

void cMaterialy::czytaj(char * nazwa_pliku)
{
	fin.open(nazwa_pliku);

	std::getline(fin, tmp);
	if (!porownaj("<WLASNOSCI MATERIALOWE>   (wygenerowany plik - NIE EDYTOWAC)", tmp))
		std::cerr << "Zly typ pliku..." << std::endl;

	while (!fin.eof())
	{
		std::getline(fin, tmp);
		if (porownaj("701", tmp))
		{
			fin >> ile;
			materialy = new sMaterial[ile];
			for (int i = 0; i < ile; i++)
				fin >> materialy[i].typ >> materialy[i].nr >> materialy[i].wartosc;
		}
	}

	fin.close();
}

std::ostream & operator<<(std::ostream & os, const cMaterialy & m)
{
	os << "---------------------------------------------------------------------" << std::endl;
	os << std::endl << "<PLIK *.M>" << std::endl << std::endl;
	os << "Modul Younga: " << m.materialy[0].wartosc << std::endl;
	os << "Modul wzmocnienia: " << m.materialy[1].wartosc << std::endl;
	os << "Granica plastycznosci: " << m.materialy[2].wartosc << std::endl;
	os << "Liczba Poissona: " << m.materialy[3].wartosc << std::endl;
	os << "Wspolczynnik liniowej rozszerzalnosci cieplnej: " << m.materialy[4].wartosc << std::endl;
	return os;
}