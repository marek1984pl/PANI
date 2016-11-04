/*---------------------------------------------------------------------
PANI - projekt
---------------------------------------------------------------------*/

#include "stdafx.h"
#include <string>

#include "porownaj.h"
#include "brzegi.h"

cBrzegi & cBrzegi::operator=(const cBrzegi & b)
{
	if (this == &b)
		return *this;
	wymiary = b.wymiary;
	brzegi = new Brzeg(*b.brzegi);
	return *this;
}

void cBrzegi::czytaj(char * nazwa_pliku)
{
	fin.open(nazwa_pliku);
	getline(fin, tmp);
	if (!porownaj("<WARUNKI BRZEGOWE>   (wygenerowany plik - NIE EDYTOWAC)", tmp))
		std::cerr << "Zly rodzaj pliku..." << std::endl;

	fin >> tmp;
	if (!porownaj("ThElPlStressSimulator_1.0", tmp))
		std::cerr << "Zly rodzaj pliku..." << std::endl;

	fin >> wymiary.l_wym >> wymiary.l_wez >> wymiary.l_ele >> wymiary.l_obs >> wymiary.l_brz;
	fin >> wymiary.l_brz;

	brzegi = new Brzeg[wymiary.l_brz];

	for (int i = 0; i < wymiary.l_brz; i++)
	{
		fin >> brzegi[i].nr >> brzegi[i].obszar >> brzegi[i].l_wez >> brzegi[i].l_warunkow_brz;

		if (brzegi[i].l_warunkow_brz == 1)
		{
			fin >> brzegi[i].l_wez >> brzegi[i].id >> brzegi[i].def >> brzegi[i].l_param_def;

			if (brzegi[i].l_param_def == 1)
			{
				fin >> tmp >> brzegi[i].id2[0] >> tmp >> tmp;
			}
			if (brzegi[i].l_param_def == 2)
			{
				fin >> brzegi[i].id2[0] >> tmp >> tmp >> brzegi[i].wart[0];
				fin >> brzegi[i].id2[1] >> tmp >> tmp >> brzegi[i].wart[1];
			}
		}
	}

	fin.close();
}

std::ostream & operator<<(std::ostream & os, const cBrzegi & b)
{
	os << "---------------------------------------------------------------------" << std::endl;
	os << std::endl << "<PLIK *.BC>" << std::endl << std::endl;
	os << "Wymiary: " << std::endl << "Liczba wymiarow: " << b.wymiary.l_wym << std::endl << "Liczba wezlow: " << b.wymiary.l_wez << std::endl << "\nLiczba elementow: " << b.wymiary.l_ele << std::endl << "Liczba obszarow: " << b.wymiary.l_obs << std::endl << "Liczba brzegow: " << b.wymiary.l_brz << std::endl;
	os << std::endl;

	os << "WARUNKI BRZEGOWE: " << std::endl;
	for (int i = 0; i < b.wymiary.l_brz; i++)
	{
		os << std::endl << "Warunek brzegowy nr. " << b.brzegi[i].nr << "\tObszar: " << b.brzegi[i].obszar << "\tLiczba wezlow: " << b.brzegi[i].l_wez << "\tLiczba warunkow brzegowych: " << b.brzegi[i].l_warunkow_brz;
		os << std::endl << std::endl;

		if (b.brzegi[i].l_warunkow_brz == 1)
		{
			os << "Liczba wezlow: " << b.brzegi[i].l_wez << "\tID brzegu: " << b.brzegi[i].id << "\nDefinicja: " << b.brzegi[i].def << "\tLiczba param. definiujacych: " << b.brzegi[i].l_param_def;
			os << std::endl;

			if (b.brzegi[i].l_param_def == 1)
			{
				os << b.brzegi[i].id2[0];
				os << std::endl << std::endl << std::endl;
			}
			if (b.brzegi[i].l_param_def == 2)
			{
				os << "ID: " << b.brzegi[i].id2[0] << "\tWartosc: " << b.brzegi[i].wart[0];
				os << std::endl;
				os << "ID: " << b.brzegi[i].id2[1] << "\tWartosc: " << b.brzegi[i].wart[1];
				os << std::endl << std::endl << std::endl;
			}
		}
	}
	return os;
}