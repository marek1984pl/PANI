/*---------------------------------------------------------------------
PANI - projekt
---------------------------------------------------------------------*/

#include "stdafx.h"

#include <iostream>

#include "matrix.h"

cMatrix::cMatrix(int w, int k) : wiersze(w), kolumny(k)
{
	matrix = new long double *[wiersze];
	for (int i = 0; i < wiersze; ++i)
		matrix[i] = new long double[kolumny];
}

cMatrix::cMatrix(const cMatrix & m)
{
	wiersze = m.wiersze;
	kolumny = m.kolumny;

	matrix = new long double *[wiersze];
	for (int i = 0; i < wiersze; ++i)
		matrix[i] = new long double[kolumny];

	for (int i = 0; i < wiersze; ++i)
		for (int j = 0; j < kolumny; ++j)
			matrix[i][j] = m.matrix[i][j];
}

cMatrix::~cMatrix()
{
	for (int i = 0; i < wiersze; ++i)
		delete[] matrix[i];

	delete[] matrix;
}

cMatrix & cMatrix::operator=(const cMatrix & m)
{
	if (this == &m) return *this;
	for (int i = 0; i < wiersze; ++i)
		delete[] matrix[i];

	delete[] matrix;

	wiersze = m.wiersze;
	kolumny = m.kolumny;

	matrix = new long double *[wiersze];
	for (int i = 0; i < wiersze; ++i)
		matrix[i] = new long double[kolumny];

	for (int i = 0; i < wiersze; ++i)
		for (int j = 0; j < kolumny; ++j)
			matrix[i][j] = m.matrix[i][j];

	return *this;
}

void cMatrix::resize(int wiersze_, int kolumny_)
{
	for (int i = 0; i < wiersze; ++i)
		delete[] matrix[i];

	delete[] matrix;

	wiersze = wiersze_;
	kolumny = kolumny_;

	matrix = new long double *[wiersze_];
	for (int i = 0; i < wiersze_; ++i)
		matrix[i] = new long double[kolumny_];
}

void cMatrix::wypelnij(long double wart)
{
	for (int i = 0; i < wiersze; ++i)
		for (int j = 0; j < kolumny; ++j)
			matrix[i][j] = wart;
}

void cMatrix::wypelnij_losowo()
{
	for (int i = 0; i < wiersze; ++i)
		for (int j = 0; j < kolumny; ++j)
			matrix[i][j] = rand() % 100;
}

void cMatrix::gauss_jordan(cMatrix & b)
{
	long double dzielnik;
	long double mnoznik;

	for (int i = 0; i < wiersze; ++i)
	{
		dzielnik = matrix[i][i];

		for (int j = i; j < wiersze; ++j)
			matrix[i][j] /= dzielnik;

		b[i][0] /= dzielnik;

		for (int i2 = 0; i2 < wiersze; ++i2)
		{
			if (i2 != i)
			{
				mnoznik = matrix[i2][i];

				for (int j2 = 0; j2 < wiersze; ++j2)
					matrix[i2][j2] -= mnoznik * matrix[i][j2];

				b[i2][0] -= mnoznik * b[i][0];
			}
		}
	}
}

void cMatrix::gauss(cMatrix & b)
{
	for (int i = 1; i <= wiersze - 1; ++i)
	{
		for (int j = i + 1; j <= wiersze; ++j)
		{
			for (int k = i + 1; k <= wiersze; ++k)
				matrix[j - 1][k - 1] -= matrix[i - 1][k - 1] * matrix[j - 1][i - 1] / matrix[i - 1][i - 1];

			b[j - 1][0] -= b[i - 1][0] * matrix[j - 1][i - 1] / matrix[i - 1][i - 1];
		}
	}

	b[wiersze - 1][0] /= matrix[wiersze - 1][wiersze - 1];

	for (int i = 1; i <= wiersze - 1; ++i)
	{
		long double s = 0;

		for (int j = wiersze + 1 - i; j <= wiersze; ++j)
			s += matrix[wiersze - i - 1][j - 1] * b[j - 1][0];

		b[wiersze - i - 1][0] = (b[wiersze - i - 1][0] - s) / matrix[wiersze - i - 1][wiersze - i - 1];
	}
}

std::ostream & operator<<(std::ostream & os, const cMatrix & m)
{
	os.setf(std::ios::scientific, std::ios::floatfield);
	for (int i = 0; i < m.wiersze; ++i)
	{
		for (int j = 0; j < m.kolumny; ++j)
		{
			os << m.matrix[i][j] << "\t";
		}
		os << std::endl;
	}
	return os;
}