/*-----------------------------------------------------------------------
	PANI - projekt
-----------------------------------------------------------------------*/

#ifndef MATERIALY_H
#define MATERIALY_H

#include <iostream>
#include <fstream>

#include "porownaj.h"

// struktura opisujaca material
// typ - rodzaj materialu, nr - ????, wartosc - wartosc ???
struct sMaterial
{
	int typ;
	int nr;
	long double wartosc;
};

// klasa przechowujaca opisy materialow
// tmp - zmienna pomocnicza
// materialy - tab struktur
// ile - ilosc pozycji do wczytania
class cMaterialy
{
	std::ifstream fin;
	std::string tmp;
	int ile;
public:
	sMaterial * materialy;

	// Domyslny konstruktor
	cMaterialy() : materialy(new sMaterial) {}
	// Konstruktor kopiujacy
	cMaterialy(const cMaterialy & m) : ile(m.ile), materialy(new sMaterial(*m.materialy)) {}
	//Operator =
	cMaterialy & operator=(const cMaterialy & m);
	
	// Funkcja czytajaca plik z materialami
	void czytaj(char * nazwa_pliku);
	
	// Destruktor
	~cMaterialy() { delete [] materialy; }
	
	// Przeciazony operator <<
	friend std::ostream & operator<<(std::ostream & os, const cMaterialy & m);
};

#endif // MATERIALY_H