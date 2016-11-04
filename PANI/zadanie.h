/*---------------------------------------------------------------------
	PANI - projekt
---------------------------------------------------------------------*/

#ifndef ZADANIE_H
#define ZADANIE_H

#include <iostream>
#include <cmath>
#include <fstream>

#include "porownaj.h"
#include "siatka.h"
#include "brzegi.h"
#include "materialy.h"
#include "matrix.h"

// wczytywanie danych
void czytaj_dane(char * msh, char * bc, char * m);
	
// obliczanie wspolczynnikow sprezystosci f1, f2, f3
// E - modul Younga
// v - liczba Poissona
void wspolczynniki_sprezystosci(char * ps);

// funkcja budujaca macierz globalna i zwracajaca ja a pobierajaca sciezki dostepu do plikow z zadaniem	
void make_global_matrix();

// funkcja tworzaca wektor b z obciazeniami 
void obciazenia();

// funkcja dodajace utwierdzenia do macierzy globalnej i wektora b
void utwierdzenia();

// funkcja obliczajaca utwierdzenia (pobiera jako parametr rodzaj zadania - PSO lub PSN)
void odksztalcenia(char * ps);

// funkcja obliczajaca naprezenie (jako parametr przyjmuje rodzaj zadania - PSO lub PSN)
void naprezenia(char * ps);

// zapisanie wynikow do pliku
void zapisz_wyniki(char * nazwa, char * ps);

// zakonczenie programu i zwolnienie pamieci
void koniec();

#endif //ZADANIE_H