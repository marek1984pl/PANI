/*---------------------------------------------------------------------
	PANI - projekt
---------------------------------------------------------------------*/

#ifndef SIATKA_H
#define SIATKA_H

#include <fstream>

#include "porownaj.h"

// typ wyliczeniowy uzywany do obslugi wyjatkow
//enum BLAD {ZLY_PLIK};

// struktura opisujaca wezel
// nr x y 
// gdzie nr jest nr wezla, x wspolrzedna pozioma, y wspolrzedna pionowa
struct sWezel
{
	int nr;
	long double x, y;
};

// struktura opisujaca koneksje
// nr typ wezely[0] wezely[1] wezel[2] obszar
// gdzie nr jest nr elementu, typ jest rodzajem koneksi, wezely[0] - wezel tworzacy nr 1, wezly[1] - wezel tworzacy nr 2, wezly[2] -  // wezel tworzacy nr 3, obszar - nr obszaru do ktorego nalezy
struct sKoneksja
{
	int nr, obszar;
	std::string typ;
	int wezly[3];
};

// struktura opisujaca brzeg
// nr ilosc_wezlow obszar
// wezly_tworzace...
// nr - nr brzegu, ilosc_wezlow - liczba wezlow tworzaca brzeg (na jej podstawie tworzona jest tab wezly_tworzace), obszar - obszar do // ktorego nalezy, wezly_tworzace - tab dynamiczna przchowujaca nr wezlow tworzacych brzeg
struct sBrzeg
{
	int nr, ilosc_wezlow, obszar;
	int * wezly_tworzace;
	
	// Konstruktor domyslny	
	sBrzeg() : wezly_tworzace(new int(0)) {}
	// Konstruktor kopiujacy
	sBrzeg(const sBrzeg & b) : nr(b.nr), ilosc_wezlow(b.ilosc_wezlow), obszar(b.obszar), wezly_tworzace(new int(*b.wezly_tworzace)) {}
	
	// Przeciazony operator =
	sBrzeg & operator=(const sBrzeg & b);
	
	// Metoda ustalajaca rozmiar tablicy wezly_tworzace
	void utworz_tablice(int ilosc_wezlow) { wezly_tworzace = new int[ilosc_wezlow];	}
		
	// Destruktor
	~sBrzeg() { delete [] wezly_tworzace; }
};

// clasa przechowujaca siatke
// l_wym - liczba wymiarow
// l_wez - liczba wezlow
// l_ele - liczba elementow
// l_obs - liczba obszarow
// l_brz - liczba brzegow
// opis - opis pliku *.msh
// wezly, koneksje, brzegi - tablice struktur zawierajace wczytane dane
// tmp - zmienna pomocnicza uzywana do wczytywania danych
class cSiatka
{
private:
    int l_wym, l_wez, l_ele, l_obs, l_brz;
    std::string opis;
	std:: ifstream fin;
    std::string tmp;
	
public:
    sWezel * wezly;
    sKoneksja * koneksje;
    sBrzeg * brzegi;
    
	// metody zwracajace dane
	// GetL_wym() - zwraca liczbe wymiarow
	// GetL_wez() - zwraca liczbe wezlow
	// GetL_ele() - zwraca liczbe elementow
	// GetL_brz() - zwraca liczbe brzegow
    int GetL_wym() const { return l_wym; }
    int GetL_wez() const { return l_wez; }
    int GetL_ele() const { return l_ele; }
    int GetL_brz() const { return l_brz; }
    
	//Konstruktor domyslny
	cSiatka() : wezly(new sWezel), koneksje(new sKoneksja), brzegi(new sBrzeg) {}
	
	// Konstruktor kopiujacy
	cSiatka(const cSiatka & s);
	
	// Przeciazony operator =
	cSiatka & operator=(const cSiatka & s);
 	
   	// wczytanie danych do tablic struktur
	void czytaj(char * nazwa_pliku);
		
	// destruktor usuwajacy tablice struktur
	~cSiatka();
  
   // przeciazony operator strumienia wyjscia
	friend std::ostream & operator<<(std::ostream & os, const cSiatka & s);
};

#endif //SIATKA_H