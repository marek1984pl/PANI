/*---------------------------------------------------------------------
PANI - projekt
---------------------------------------------------------------------*/

#include "stdafx.h"
#include <iostream>
#include <string>

#include "siatka.h"
#include "materialy.h"
#include "brzegi.h"
#include "matrix.h"
#include "zadanie.h"

int main(int argc, char * argv[])
{
	setlocale(LC_ALL, "");

	if (argc < 6)
	{

		std::cout << "----------****   PANI projekt   ****----------" << std::endl << std::endl << std::endl;
		std::cout << "Podaj argumenty wymagane do uruchomiania programu:" << std::endl << std::endl;
		std::cout << "1. Po�o�enie pliku *.msh z danymi siatki" << std::endl;
		std::cout << "2. Po�o�enie pliku *.bc z warunkami brzegowymi" << std::endl;
		std::cout << "3. Po�o�enie pliku *.m z danymi materia��w" << std::endl;
		std::cout << "4. Rodzaj oblicz�: -pso lub -psn" << std::endl;
		std::cout << "5. Plik do kt�rego zapisa� wyniki" << std::endl << std::endl;

		std::cout << "Przyk�adowe uruchomienie programu:" << std::endl;
		std::cout << "pani.exe k.msh k.bc k.m -pso wyniki.txt" << std::endl;
	}
	else
	{
		czytaj_dane(argv[1], argv[2], argv[3]);
		wspolczynniki_sprezystosci(argv[4]);
		make_global_matrix();
		obciazenia();
		utwierdzenia();
		odksztalcenia(argv[4]);
		naprezenia(argv[4]);
		zapisz_wyniki(argv[5], argv[4]);
		koniec();
		std::cout << std::endl << "Obliczenia zako�czone! Wyniki zapisane do pliku " << argv[5] << std::endl;
	}

	system("pause");

	return 0;
}