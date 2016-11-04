/*---------------------------------------------------------------------
	PANI - projekt
---------------------------------------------------------------------*/

#ifndef BRZEGI_H
#define BRZEGI_H

#include <fstream>

#include "porownaj.h"

// Struktura przechowujaca wymiary
// l_wym - liczba wymiarow
// l_wez - liczba wezlow
// l_ele - liczba elementow
// l_obs - liczba obszarow
// l_brz - liczba brzegow
struct sWymiary
{
	int l_wym, l_wez, l_ele, l_obs, l_brz;
};
   
// struktura opisujaca brzeg
// nr - nr brzegu
// obszar - obszar do ktorego nalezy
// l_wez - liczba wezlow nalezaca do brzegu
// l_warunkow_brz - liczba warunkow opisujacych brzeg
// id - identyfikator brzegu
// def - ??
// l_param_def - liczba parametrow definiujacych brzeg
// id2[2] - 2 identyfikator (czy 1001 czy 1002)
// wart[2] - wartosc dla 1001 i 1002
struct Brzeg
{
	int nr, obszar, l_wez, l_warunkow_brz;
	int id, def;
	int l_param_def;
	int id2[2];
	long double wart[2];
};


// Klasa opisujaca wszystkie brzegi
// fin - strumien wejscia
// tmp - zmienna pomocnicza
// wymiary - obiekt sWymiary przechowujacy wymiary
// brzegi - obiekt  sBrzeg przechowujacy opis brzegow
class cBrzegi
{
	std::ifstream fin;
	std::string tmp;
	sWymiary wymiary;
public:
    Brzeg * brzegi;
	
    // Domyslny konstruktor
	cBrzegi() : tmp(""), brzegi(new Brzeg) {}
	
	// Konstruktor kopiujacy
	cBrzegi(const cBrzegi & b) : wymiary(b.wymiary), brzegi(new Brzeg(*b.brzegi)) {}
	
	// Operator =
	cBrzegi & operator=(const cBrzegi & b);
	
	// Dekstruktor
	~cBrzegi() { delete [] brzegi; }
	
	// Funkcja czytajaca plik *.bc
	void czytaj(char * nazwa_pliku);
	
	// operator <<
	friend std::ostream & operator<<(std::ostream & os, const cBrzegi & b);
};

#endif //BRZEGI_H
