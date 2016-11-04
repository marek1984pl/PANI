/*---------------------------------------------------------------------
PANI - projekt
---------------------------------------------------------------------*/

#ifndef MATRIX_H
#define MATRIX_H

// Szablon klas obslugujacych macierze
// wiersze - ilosc wierszy
// kolumny - ilosc kolumn
// matrix - 2 wymiarowa tablica o rozmiarze wiersze*kolumny

class cMatrix
{
private:
	int wiersze, kolumny;

public:
	long double ** matrix;

	// ctor domyslny  
	cMatrix() : wiersze(0), kolumny(0), matrix(new long double * (0)) {}

	// ctor z parametrami
	cMatrix(int w, int k);

	// cctor
	cMatrix(const cMatrix & m);

	// dtor
	~cMatrix();

	// operator =
	cMatrix & operator=(const cMatrix & m);

	// Funkcja zmieniajaca rozmiar macierzy
	void resize(int wiersze_, int kolumny_);

	// metody zwracajace liczbe wierszy i kolumn     
	int getWiersze() const { return wiersze; }
	int getKolumny() const { return kolumny; }

	// metoda wypelniajaca macierz podana wartoscia wart
	void wypelnij(long double wart);

	// metoda wypelniajaca macierz losowo w zakresie 0 do 100
	void wypelnij_losowo();

	// eliminacja gaussa-jordana
	void gauss_jordan(cMatrix & b);

	//eliminacja gaussa
	void gauss(cMatrix & b);

	// operator []
	long double * operator[](int index) const { return matrix[index]; }

	// operator << 
	friend std::ostream & operator<<(std::ostream & os, const cMatrix & m);
};

#endif //MATRIX_H