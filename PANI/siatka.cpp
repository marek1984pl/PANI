/*---------------------------------------------------------------------
PANI - projekt
---------------------------------------------------------------------*/

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>

#include "porownaj.h"
#include "siatka.h"

sBrzeg & sBrzeg::operator=(const sBrzeg & b)
{
	if (this == &b)
		return *this;
	nr = b.nr;
	ilosc_wezlow = b.ilosc_wezlow;
	obszar = b.obszar;
	wezly_tworzace = new int(*b.wezly_tworzace);
	return *this;
}

cSiatka::cSiatka(const cSiatka & s)
{
	l_wym = s.l_wym;
	l_wez = s.l_wez;
	l_ele = s.l_ele;
	l_obs = s.l_obs;
	l_brz = s.l_brz;
	opis = s.opis;
	wezly = new sWezel(*s.wezly);
	koneksje = new sKoneksja(*s.koneksje);
	brzegi = new sBrzeg(*s.brzegi);
}

cSiatka & cSiatka::operator=(const cSiatka & s)
{
	if (this == &s)
		return *this;
	l_wym = s.l_wym;
	l_wez = s.l_wez;
	l_ele = s.l_ele;
	l_obs = s.l_obs;
	l_brz = s.l_brz;
	opis = s.opis;
	wezly = new sWezel(*s.wezly);
	koneksje = new sKoneksja(*s.koneksje);
	brzegi = new sBrzeg(*s.brzegi);
	return *this;
}

void cSiatka::czytaj(char * nazwa_pliku)
{
	fin.open(nazwa_pliku);

	// sprawdzamy czy czytamy odpowiedni plik
	if (porownaj("<SIATKA>", tmp))
		std::cerr << "Zly rodzaj pliku" << std::endl;

	// wczytujemy wymiary potrzebne do utworzenia tablic struktur	
	while (!fin.eof())
	{
		std::getline(fin, tmp);
		if (porownaj("[opis]", tmp))
			getline(fin, opis);
		if (porownaj("[liczba wymiarow]", tmp))
			fin >> l_wym;
		if (porownaj("[liczba wezlow]", tmp))
		{
			fin >> l_wez;
			wezly = new sWezel[l_wez];
		}
		if (porownaj("[liczba elementow]", tmp))
		{
			fin >> l_ele;
			koneksje = new sKoneksja[l_ele];
		}
		if (porownaj("[liczba brzegow]", tmp))
		{
			fin >> l_brz;
			brzegi = new sBrzeg[l_brz];
		}
		if (porownaj("[liczba obszarow]", tmp))
			fin >> l_obs;

		if (porownaj("[wspolrzedne wezlow]", tmp))
			for (int i = 0; i < l_wez; i++)
			{
				fin >> wezly[i].nr >> wezly[i].x >> wezly[i].y;
			}
		if (porownaj("[koneksje]", tmp))
			for (int i = 0; i < l_ele; i++)
			{
				fin >> koneksje[i].nr >> koneksje[i].typ >> koneksje[i].wezly[0] >> koneksje[i].wezly[1] >> koneksje[i].wezly[2] >> koneksje[i].obszar;
			}
		if (porownaj("[brzegi]", tmp))
		{
			for (int i = 0; i < l_brz; i++)
			{
				fin >> brzegi[i].nr >> brzegi[i].ilosc_wezlow >> brzegi[i].obszar;
				brzegi[i].utworz_tablice(brzegi[i].ilosc_wezlow);

				for (int j = 0; j < brzegi[i].ilosc_wezlow; j++)
					fin >> brzegi[i].wezly_tworzace[j];
			}
		}
	}
	fin.close();
}

cSiatka::~cSiatka()
{
	delete[] wezly;
	delete[] koneksje;
	delete[] brzegi;
}

std::ostream & operator<<(std::ostream & os, const cSiatka & s)
{
	os << "---------------------------------------------------------------------" << std::endl;
	os << std::endl << "<PLIK *.MSH>" << std::endl << std::endl << "Wymiary: " << std::endl << "Liczba wymiarow: " << s.l_wym << std::endl << "Liczba wezlow: " << s.l_wez << std::endl << "Liczba elementow: " << s.l_ele << std::endl << "Liczba obszarow: " << s.l_obs << std::endl << "Liczba brzegow: " << s.l_brz << std::endl << std::endl;

	os << "[Wspolrzedne wezlow]: " << std::endl;
	for (int i = 0; i < s.l_wez; i++)
		os << "Nr.wezla: " << s.wezly[i].nr << "\tX: " << s.wezly[i].x << "\tY: " << s.wezly[i].y << std::endl;

	os << std::endl << "[Koneksje]: " << std::endl;
	for (int i = 0; i < s.l_ele; i++)
		os << "Nr.wezla: " << s.koneksje[i].nr << "\tTyp koneksji: " << s.koneksje[i].typ << "\tWezly: " << s.koneksje[i].wezly[0] << " " << s.koneksje[i].wezly[1] << " " << s.koneksje[i].wezly[2] << "\tObszar: " << s.koneksje[i].obszar << std::endl;

	os << std::endl << "[Brzegi]: " << std::endl;
	for (int i = 0; i < s.l_brz; i++)
	{
		os << "Nr.brzegu: " << s.brzegi[i].nr << "\tIlosc wezlow: " << s.brzegi[i].ilosc_wezlow << "\tObszar: " << s.brzegi[i].obszar;
		os << "\tWezly tworzace: ";
		for (int j = 0; j < s.brzegi[i].ilosc_wezlow; j++)
		{
			os << " " << s.brzegi[i].wezly_tworzace[j];
		}
		os << std::endl;
	}

	return os;
}