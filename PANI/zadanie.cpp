/*---------------------------------------------------------------------
PANI - projekt
---------------------------------------------------------------------*/

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>

#include "zadanie.h"

// tworzymy obiekty z danymi do zadania
cSiatka siatka;
cBrzegi brzegi;
cMaterialy materialy;
cMatrix b;
cMatrix * lokalna;
cMatrix globalna;
cMatrix * C;
long double f1, f2, f3;
long double E, v;
long double * eps_x;
long double * eps_y;
long double * gamma_xy;
long double * eps_z;
long double * sigma_x;
long double * sigma_y;
long double * sigma_z;
long double * tau;

void czytaj_dane(char * msh, char * bc, char * m)
{
	// tworzymy obiekty z danymi do zadania
	siatka.czytaj(msh);
	brzegi.czytaj(bc);
	materialy.czytaj(m);
}

void wspolczynniki_sprezystosci(char * ps)
{
	E = materialy.materialy[0].wartosc;
	v = materialy.materialy[3].wartosc;

	// dla PSO
	if (porownaj("-pso", ps))
	{
		f1 = (E * (1 - v)) / ((1 + v) * (1 - 2 * v));
		f2 = E*v / ((1 + v) * (1 - 2 * v));
		f3 = E / (2 * (1 + v));
	}

	//dla PSN
	if (porownaj("-psn", ps))
	{
		f1 = E / (1 - v*v);
		f2 = (E*v) / (1 - v*v);
		f3 = E / (2 * (1 + v));
	}
}

void make_global_matrix()
{
	// tworzenie macierzy C
	C = new cMatrix[siatka.GetL_ele()];
	// tworzenie macierzy lokalnych
	lokalna = new cMatrix[siatka.GetL_ele()];
	// tworzenie macierzy dla wspolczynnika A
	cMatrix A(siatka.GetL_ele(), 1);
	// zmienne dla wspolrzednych wezlow
	long double x1, x2, x3, y1, y2, y3;

	// obliczamy A i C oraz zmieniamy rozmiary macierzy lokalnych i wypelniamy je 0
	for (int i = 0; i < siatka.GetL_ele(); ++i)
	{
		lokalna[i].resize(6, 6);
		lokalna[i].wypelnij(0);

		// obliczamy x1, x2, x3, y1, y2, y3 
		// jako wspolrzedne wezlow podajemy kolejne nr. wezlow z koneksji tworzace kolejne elementy
		// odejmujemy 1 poniewaz w pliku *.msh mamy nr wezlow podane od 1 do * a my indeksujemy od 0 do (* - 1)
		x1 = siatka.wezly[siatka.koneksje[i].wezly[0] - 1].x;
		y1 = siatka.wezly[siatka.koneksje[i].wezly[0] - 1].y;
		x2 = siatka.wezly[siatka.koneksje[i].wezly[1] - 1].x;
		y2 = siatka.wezly[siatka.koneksje[i].wezly[1] - 1].y;
		x3 = siatka.wezly[siatka.koneksje[i].wezly[2] - 1].x;
		y3 = siatka.wezly[siatka.koneksje[i].wezly[2] - 1].y;

		// obliczamy wspolczynniki A dla macierzy lokalnych
		A[i][0] = 0.5*(x1*(y2 - y3) + x2*(y3 - y1) + x3*(y1 - y2));

		// zmieniamy rozmiary macierzy C i obliczamy elementy C21, C22, C23, C31, C32, C33
		// macierz zrobiona jest na wyrost zeby latwiej bylo ja indeksowac i nie komplikowac pozniej dostepu do elementow
		C[i].resize(4, 4);
		C[i][2][1] = (y2 - y3) / (2 * A[i][0]);
		C[i][2][2] = (y3 - y1) / (2 * A[i][0]);
		C[i][2][3] = (y1 - y2) / (2 * A[i][0]);
		C[i][3][1] = (x3 - x2) / (2 * A[i][0]);
		C[i][3][2] = (x1 - x3) / (2 * A[i][0]);
		C[i][3][3] = (x2 - x1) / (2 * A[i][0]);
	}

	// tworzymy globalna macierz sztywnosc i wypelniamy ja 0
	globalna.resize(2 * siatka.GetL_wez(), 2 * siatka.GetL_wez());
	globalna.wypelnij(0);

	// wypelniamy macierze lokalne, kazda macierz lokalna ma rozmiar 6x6
	for (int i = 0; i < siatka.GetL_ele(); ++i)
	{
		for (int m = 0; m < 3; ++m)
		{
			for (int n = 0; n < 3; ++n)
			{
				// dodajemy 0 lub 1 do wpolrzednych macierzy lokalnej zeby wybrac 1 z 4 pol np:
				// 		[1] [2] ; 		[2 * m][2 * n] wybiera 1 ; [2 * m][(2 * n) + 1] wybiera 2;
				// 		[3] [4] ; 		[(2 * m) + 1][2 * n] wybiera 3; [(2 * m) + 1][(2 * n) + 1] wybiera 4; 

				// w indexach C[i][2][m + 1]*C[i][2][n + 1] + C[i][3][m + 1]*C[i][3][n + 1]) dodajemy za kazdym razem 1 poniewaz wczesniej 
				// indexowalismy od 1
				lokalna[i][(2 * m)][(2 * n)] = A[i][0] * (f1*C[i][2][m + 1] * C[i][2][n + 1] + f3*C[i][3][m + 1] * C[i][3][n + 1]);
				lokalna[i][(2 * m)][(2 * n) + 1] = A[i][0] * (f2*C[i][2][m + 1] * C[i][3][n + 1] + f3*C[i][3][m + 1] * C[i][2][n + 1]);
				lokalna[i][(2 * m) + 1][(2 * n)] = A[i][0] * (f2*C[i][3][m + 1] * C[i][2][n + 1] + f3*C[i][2][m + 1] * C[i][3][n + 1]);
				lokalna[i][(2 * m) + 1][(2 * n) + 1] = A[i][0] * (f1*C[i][3][m + 1] * C[i][3][n + 1] + f3*C[i][2][m + 1] * C[i][2][n + 1]);
			}
		}
	}

	// wypelniamy macierz globalna macierzami lokalnymi
	for (int i = 0; i < siatka.GetL_ele(); ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				// wybieramy wspolrzedne w macierzy globalnej za pomoca if`ow zeby nie utrudniac indexowania
				// we wspolrzednych macierzy globalnej odejmujemy 1 lub 2 tak np:
				// [a1][a2][a3][a4]....
				// [b1][b2][b3][b4]...
				// [c1][c2][c3][c4]...
				// [d1][d2][d3][d4]...
				// jezeli dodajemy z macierzy lokalnej wpolrzedne [0][2], [0][3], [1][2] i [1][3] to musimy w globalnej wpisac je do [a3], [a4], [b3], [b4]
				// j i k wybiera miejsce w macierzy globalnej do ktorego wpisujemy z macierzy lokalnych
				if (j == 0 && k == 0)
				{
					globalna[2 * siatka.koneksje[i].wezly[j] - 2][2 * siatka.koneksje[i].wezly[k] - 2] += lokalna[i][0][0];
					globalna[2 * siatka.koneksje[i].wezly[j] - 2][2 * siatka.koneksje[i].wezly[k] - 1] += lokalna[i][0][1];
					globalna[2 * siatka.koneksje[i].wezly[j] - 1][2 * siatka.koneksje[i].wezly[k] - 2] += lokalna[i][1][0];
					globalna[2 * siatka.koneksje[i].wezly[j] - 1][2 * siatka.koneksje[i].wezly[k] - 1] += lokalna[i][1][1];
				}

				if (j == 0 && k == 1)
				{
					globalna[2 * siatka.koneksje[i].wezly[j] - 2][2 * siatka.koneksje[i].wezly[k] - 2] += lokalna[i][0][2];
					globalna[2 * siatka.koneksje[i].wezly[j] - 2][2 * siatka.koneksje[i].wezly[k] - 1] += lokalna[i][0][3];
					globalna[2 * siatka.koneksje[i].wezly[j] - 1][2 * siatka.koneksje[i].wezly[k] - 2] += lokalna[i][1][2];
					globalna[2 * siatka.koneksje[i].wezly[j] - 1][2 * siatka.koneksje[i].wezly[k] - 1] += lokalna[i][1][3];
				}

				if (j == 0 && k == 2)
				{
					globalna[2 * siatka.koneksje[i].wezly[j] - 2][2 * siatka.koneksje[i].wezly[k] - 2] += lokalna[i][0][4];
					globalna[2 * siatka.koneksje[i].wezly[j] - 2][2 * siatka.koneksje[i].wezly[k] - 1] += lokalna[i][0][5];
					globalna[2 * siatka.koneksje[i].wezly[j] - 1][2 * siatka.koneksje[i].wezly[k] - 2] += lokalna[i][1][4];
					globalna[2 * siatka.koneksje[i].wezly[j] - 1][2 * siatka.koneksje[i].wezly[k] - 1] += lokalna[i][1][5];
				}

				if (j == 1 && k == 0)
				{
					globalna[2 * siatka.koneksje[i].wezly[j] - 2][2 * siatka.koneksje[i].wezly[k] - 2] += lokalna[i][2][0];
					globalna[2 * siatka.koneksje[i].wezly[j] - 2][2 * siatka.koneksje[i].wezly[k] - 1] += lokalna[i][2][1];
					globalna[2 * siatka.koneksje[i].wezly[j] - 1][2 * siatka.koneksje[i].wezly[k] - 2] += lokalna[i][3][0];
					globalna[2 * siatka.koneksje[i].wezly[j] - 1][2 * siatka.koneksje[i].wezly[k] - 1] += lokalna[i][3][1];
				}

				if (j == 1 && k == 1)
				{
					globalna[2 * siatka.koneksje[i].wezly[j] - 2][2 * siatka.koneksje[i].wezly[k] - 2] += lokalna[i][2][2];
					globalna[2 * siatka.koneksje[i].wezly[j] - 2][2 * siatka.koneksje[i].wezly[k] - 1] += lokalna[i][2][3];
					globalna[2 * siatka.koneksje[i].wezly[j] - 1][2 * siatka.koneksje[i].wezly[k] - 2] += lokalna[i][3][2];
					globalna[2 * siatka.koneksje[i].wezly[j] - 1][2 * siatka.koneksje[i].wezly[k] - 1] += lokalna[i][3][3];
				}


				if (j == 1 && k == 2)
				{
					globalna[2 * siatka.koneksje[i].wezly[j] - 2][2 * siatka.koneksje[i].wezly[k] - 2] += lokalna[i][2][4];
					globalna[2 * siatka.koneksje[i].wezly[j] - 2][2 * siatka.koneksje[i].wezly[k] - 1] += lokalna[i][2][5];
					globalna[2 * siatka.koneksje[i].wezly[j] - 1][2 * siatka.koneksje[i].wezly[k] - 2] += lokalna[i][3][4];
					globalna[2 * siatka.koneksje[i].wezly[j] - 1][2 * siatka.koneksje[i].wezly[k] - 1] += lokalna[i][3][5];
				}


				if (j == 2 && k == 0)
				{
					globalna[2 * siatka.koneksje[i].wezly[j] - 2][2 * siatka.koneksje[i].wezly[k] - 2] += lokalna[i][4][0];
					globalna[2 * siatka.koneksje[i].wezly[j] - 2][2 * siatka.koneksje[i].wezly[k] - 1] += lokalna[i][4][1];
					globalna[2 * siatka.koneksje[i].wezly[j] - 1][2 * siatka.koneksje[i].wezly[k] - 2] += lokalna[i][5][0];
					globalna[2 * siatka.koneksje[i].wezly[j] - 1][2 * siatka.koneksje[i].wezly[k] - 1] += lokalna[i][5][1];
				}

				if (j == 2 && k == 1)
				{
					globalna[2 * siatka.koneksje[i].wezly[j] - 2][2 * siatka.koneksje[i].wezly[k] - 2] += lokalna[i][4][2];
					globalna[2 * siatka.koneksje[i].wezly[j] - 2][2 * siatka.koneksje[i].wezly[k] - 1] += lokalna[i][4][3];
					globalna[2 * siatka.koneksje[i].wezly[j] - 1][2 * siatka.koneksje[i].wezly[k] - 2] += lokalna[i][5][2];
					globalna[2 * siatka.koneksje[i].wezly[j] - 1][2 * siatka.koneksje[i].wezly[k] - 1] += lokalna[i][5][3];
				}

				if (j == 2 && k == 2)
				{
					globalna[2 * siatka.koneksje[i].wezly[j] - 2][2 * siatka.koneksje[i].wezly[k] - 2] += lokalna[i][4][4];
					globalna[2 * siatka.koneksje[i].wezly[j] - 2][2 * siatka.koneksje[i].wezly[k] - 1] += lokalna[i][4][5];
					globalna[2 * siatka.koneksje[i].wezly[j] - 1][2 * siatka.koneksje[i].wezly[k] - 2] += lokalna[i][5][4];
					globalna[2 * siatka.koneksje[i].wezly[j] - 1][2 * siatka.koneksje[i].wezly[k] - 1] += lokalna[i][5][5];
				}
			}
		}
	}
}

void obciazenia()
{
	// zmieniamy rozmiar wektora b i wypelniamy go 0
	b.resize(2 * siatka.GetL_wez(), 1);
	b.wypelnij(0);

	//cout << brzegi << std::endl;

	// zmienne przechowujace wspolrzedne wezlow
	long double x1, x2, y1, y2;

	// zmienna przechowujaca odleglosc miedzy wezlami tworzacymi brzeg ktory jest obciazony
	long double odl;

	// wypelniamy wektor b
	for (int i = 0; i < siatka.GetL_brz(); ++i)
	{
		// sprawdzamy czy istnieje warunek brzegowy
		if (brzegi.brzegi[i].l_warunkow_brz == 1)
			// nastepnie sprawdzamy czy warunek jest obciazeniem
			if (brzegi.brzegi[i].id == 1008)
			{
				for (int j = 0; j < siatka.brzegi[i].ilosc_wezlow - 1; ++j)
				{
					// pobieramy wspolrzedne wezlow
					x1 = siatka.wezly[siatka.brzegi[i].wezly_tworzace[j] - 1].x;
					x2 = siatka.wezly[siatka.brzegi[i].wezly_tworzace[j + 1] - 1].x;
					y1 = siatka.wezly[siatka.brzegi[i].wezly_tworzace[j] - 1].y;
					y2 = siatka.wezly[siatka.brzegi[i].wezly_tworzace[j + 1] - 1].y;

					// obliczamy odleglosc miedzy 2 wezlami
					odl = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));

					// dla obciazenia po x
					if (brzegi.brzegi[i].id2[0] == 1001)
					{
						b[2 * siatka.brzegi[i].wezly_tworzace[j] - 2][0] += (odl * brzegi.brzegi[i].wart[0]) / 2;
						b[2 * siatka.brzegi[i].wezly_tworzace[j + 1] - 2][0] += (odl * brzegi.brzegi[i].wart[0]) / 2;
					}
					//dla obciazenia po y
					if (brzegi.brzegi[i].id2[1] == 1002)
					{
						b[2 * siatka.brzegi[i].wezly_tworzace[j] - 1][0] += (odl * brzegi.brzegi[i].wart[1]) / 2;
						b[2 * siatka.brzegi[i].wezly_tworzace[j + 1] - 1][0] += (odl * brzegi.brzegi[i].wart[1]) / 2;
					}
				}
			}
	}
}

void utwierdzenia()
{
	// wspolrzedne przeciec
	int przeciecie1, przeciecie2;

	for (int i = 0; i < siatka.GetL_brz(); ++i)
	{
		if (brzegi.brzegi[i].l_warunkow_brz == 1)
		{
			// utwierdzenie po x
			if (brzegi.brzegi[i].id == 1002)
			{
				for (int j = 0; j < siatka.brzegi[i].ilosc_wezlow; ++j)
				{
					// glowna przekatna
					przeciecie1 = 2 * siatka.brzegi[i].wezly_tworzace[j] - 2;
					for (int m = 0; m < 2 * siatka.GetL_wez(); ++m)
					{
						// zerujemy macierz globalna i wektor b dla utwierdzen po x
						if (przeciecie1 != m)
						{
							globalna[przeciecie1][m] = 0;
							globalna[m][przeciecie1] = 0;
							b[przeciecie1][0] = 0;
						}

						// na glownej przekatnej macierzy globalnej dla utwierdzen wstawiamy 1
						if (przeciecie1 == m)
						{
							globalna[przeciecie1][m] = 1;
							globalna[m][przeciecie1] = 1;
						}
					}
				}
			}

			// utwierdzenia po y
			if (brzegi.brzegi[i].id == 1004)
			{
				for (int j = 0; j < siatka.brzegi[i].ilosc_wezlow; ++j)
				{
					// glowana przekatna
					przeciecie2 = 2 * siatka.brzegi[i].wezly_tworzace[j] - 1;
					for (int m = 0; m < 2 * siatka.GetL_wez(); ++m)
					{
						// zerujemy macierz globalna i wektor b dla utwierdzen po y
						if (przeciecie2 != m)
						{
							globalna[przeciecie2][m] = 0;
							globalna[m][przeciecie2] = 0;
							b[przeciecie2][0] = 0;
						}

						// na glownej przekatnej macierzy globalnej dla utwierdzen wstawiamy 1
						if (przeciecie2 == m)
						{
							globalna[przeciecie2][m] = 1;
							globalna[m][przeciecie2] = 1;
						}
					}
				}
			}

			// utwierdzenia po x i y
			if (brzegi.brzegi[i].id == 1006)
			{
				for (int j = 0; j < siatka.brzegi[i].ilosc_wezlow; ++j)
				{
					// glowna przekatna
					przeciecie1 = 2 * siatka.brzegi[i].wezly_tworzace[j] - 2;
					przeciecie2 = 2 * siatka.brzegi[i].wezly_tworzace[j] - 1;

					for (int m = 0; m < 2 * siatka.GetL_wez(); ++m)
					{
						// zerujemy macierz globalna i wektor b dla utwierdzen po x						
						if (przeciecie1 != m)
						{
							globalna[przeciecie1][m] = 0;
							globalna[m][przeciecie1] = 0;
							b[przeciecie2][0] = 0;
						}

						// na glownej przekatnej macierzy globalnej dla utwierdzen wstawiamy 1
						if (przeciecie1 == m)
						{
							globalna[przeciecie1][m] = 1;
							globalna[m][przeciecie1] = 1;
						}

						// zerujemy macierz globalna i wektor b dla utwierdzen po y											
						if (przeciecie2 != m)
						{
							globalna[przeciecie2][m] = 0;
							globalna[m][przeciecie2] = 0;
							b[przeciecie1][0] = 0;
						}

						// na glownej przekatnej macierzy globalnej dla utwierdzen wstawiamy 1												
						if (przeciecie2 == m)
						{
							globalna[przeciecie2][m] = 1;
							globalna[m][przeciecie2] = 1;
						}
					}
				}
			}

		}
	}
}

void odksztalcenia(char * ps)
{
	// obliczenie ukladu rownac
	globalna.gauss_jordan(b);

	// utworzenie tablic dla wynikow
	eps_x = new long double[siatka.GetL_ele()];
	eps_y = new long double[siatka.GetL_ele()];
	gamma_xy = new long double[siatka.GetL_ele()];
	eps_z = new long double[siatka.GetL_ele()];

	// dla PSO
	if (porownaj("-pso", ps))
	{
		for (int i = 0; i < siatka.GetL_ele(); ++i)
		{
			// obliczenie eps_x, eps_y i gammy
			eps_x[i] = C[i][2][1] * b[0][0] + C[i][2][2] * b[2][0] + C[i][2][3] * b[4][0];
			eps_y[i] = C[i][3][1] * b[1][0] + C[i][3][2] * b[3][0] + C[i][3][3] * b[5][0];
			gamma_xy[i] = C[i][3][1] * b[0][0] + C[i][3][2] * b[2][0] + C[i][3][3] * b[4][0] + C[i][2][1] * b[1][0] + C[i][2][2] * b[3][0] + C[i][2][3] * b[5][0];
		}
	}

	// dla PSN
	if (porownaj("-psn", ps))
	{
		for (int i = 0; i < siatka.GetL_ele(); ++i)
		{
			// oblicznie eps_x, eps_y, eps_z
			eps_x[i] = C[i][2][1] * b[0][0] + C[i][2][2] * b[2][0] + C[i][2][3] * b[4][0];
			eps_y[i] = C[i][3][1] * b[1][0] + C[i][3][2] * b[3][0] + C[i][3][3] * b[5][0];
			eps_z[i] = (-v / (1 - v))*(eps_x[i] + eps_y[i]);
		}
	}
}

void naprezenia(char * ps)
{
	// utworzenie tablic dla wynikow
	sigma_x = new long double[siatka.GetL_ele()];
	sigma_y = new long double[siatka.GetL_ele()];
	sigma_z = new long double[siatka.GetL_ele()];
	tau = new long double[siatka.GetL_ele()];

	// obliczenie sigma_x i sigma_y
	for (int i = 0; i < siatka.GetL_ele(); ++i)
	{
		sigma_x[i] = f1 * eps_x[i] + f2 * eps_y[i];
		sigma_y[i] = f2 * eps_x[i] + f1 * eps_y[i];
	}

	// dla PSO obliczamy sigma_z
	if (porownaj("-pso", ps))
		for (int i = 0; i < siatka.GetL_ele(); ++i)
			sigma_z[i] = v*(sigma_x[i] + sigma_y[i]);

	// dla PSN obliczamy tau
	if (porownaj("-psn", ps))
	{
		for (int i = 0; i < siatka.GetL_ele(); i++)
			tau[i] = f3 * gamma_xy[i];
	}
}

void zapisz_wyniki(char * nazwa, char * ps)
{
	std::ofstream fout(nazwa);
	fout.setf(std::ios::scientific, std::ios::floatfield);

	// PSO
	if (porownaj("-pso", ps))
	{
		fout << "[ PSO ]" << std::endl;
		fout << std::endl << "EPS_X\tEPS_Y\tGAMMA" << std::endl;

		for (int i = 0; i < siatka.GetL_ele(); ++i)
			fout << "eps_x[" << i << "] = " << eps_x[i] << "\t eps_y[" << i << "] = " << eps_y[i] << "\t gama[" << i << "] = " << gamma_xy[i] << std::endl;

		fout << std::endl << "SIGMA_Z" << std::endl;

		for (int i = 0; i < siatka.GetL_ele(); ++i)
			fout << "sigma_z[" << i << "] = " << sigma_z[i] << std::endl;
	}

	// PSN
	if (porownaj("-psn", ps))
	{
		fout << "[ PSN ]" << std::endl;
		fout << std::endl << "tEPS_Z" << std::endl;

		for (int i = 0; i < siatka.GetL_ele(); ++i)
			fout << "eps_z[" << i << "] = " << eps_z[i] << std::endl;

		fout << std::endl << "SIGMA_X\tSIGMA_y\tTAU" << std::endl;

		for (int i = 0; i < siatka.GetL_ele(); ++i)
			fout << "sigma_x[" << i << "] = " << sigma_x[i] << "\t\tsigma_y[" << i << "] = " << sigma_y[i] << "\t\ttau[" << i << "] = " << tau[i] << std::endl;
	}

	fout.close();
}

void koniec()
{
	delete[] lokalna;
	delete[] C;
	delete[] sigma_x;
	delete[] sigma_y;
	delete[] sigma_z;
	delete[] tau;
	delete[] eps_x;
	delete[] eps_y;
	delete[] gamma_xy;
	delete[] eps_z;
}