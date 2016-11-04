/*---------------------------------------------------------------------
PANI - projekt
---------------------------------------------------------------------*/

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <cmath>

#include "siatka.h"
#include "materialy.h"
#include "brzegi.h"
#include "matrix.h"
#include "zadanie.h"

int main(int argc, char * argv[])
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

	return 0;
}