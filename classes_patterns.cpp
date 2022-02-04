#include <iostream>
#include <vector>

using namespace std;

template <typename T>
struct Matrix
{
	int row;
	int column;
	vector<vector<T>> elements;

	Matrix<T>& operator() (int start_row, int finish_row, int start_column, int finish_column)
	{
		row = finish_row - start_row + 1;
		column = finish_column - start_column + 1;

		elements.erase(elements.begin(), elements.begin() + start_row);	
		int end_row = row - finish_row - 1;
		int end_column = column - finish_column - 1;
		for (int i = 0; i < end_row; i++)
			elements.pop_back();
		for (int i = 0; i < row; i++)
		{
			elements[i].erase(elements[i].begin(), elements[i].begin() + start_column);
			for (int j = 0; j < end_row; j++)
				elements[i].pop_back();
		}
		return *this;
	}

	Matrix<T>& operator= (Matrix<T>& M)
	{
		row = M.row;
		column = M.column;
		elements = M.elements;
		return *this;
	}


};

template <typename T>
istream& operator >> (istream& in, Matrix<T>& M)
{
	M.elements.resize(M.row);
	for (int i = 0; i < M.row; i++)
		M.elements[i].resize(M.column);
	for (int i = 0; i < M.row; i++)
	{
		for (int j = 0; j < M.column; j++)
		{
			in >> M.elements[i][j];
		}
	}
	return in;
}

template <typename T>
ostream& operator << (std::ostream& os, Matrix<T>& M)
{
	for (int i = 0; i < M.row; i++)
	{
		for (int j = 0; j < M.column; j++)
		{
			os << M.elements[i][j] << ' ';
		}
		cout << endl;
	}
	return os;
}

template <typename T>
Matrix<T> operator+ (Matrix<T>& M, Matrix<T>& N)
{
	Matrix<T> Sum;
	Sum.row = M.row; Sum.column = M.column;
	Sum.elements.resize(M.row);
	for (int i = 0; i < M.row; i++)
		Sum.elements[i].resize(M.column);
	for (int i = 0; i < M.row; i++)
		for (int j = 0; j < M.column; j++)
		{
			Sum.elements[i][j] = M.elements[i][j] + N.elements[i][j];
		}
	return Sum;
}

template <typename T>
Matrix<T> operator* (Matrix<T>& M, Matrix<T>& N)
{
	Matrix<T> Multiply;
	Multiply.row = M.row; Multiply.column = N.column;
	Multiply.elements.resize(Multiply.row);
	for (int i = 0; i < M.row; i++)
		Multiply.elements[i].resize(Multiply.column);
	for (int i = 0; i < M.row; i++)
	{
		for (int j = 0; j < N.column; j++)
		{
			for (int k = 0; k < M.column; k++)
				Multiply.elements[i][j] += M.elements[i][k] * N.elements[k][j];
		}
	}
	return Multiply;
}

template <typename T>
Matrix<T> operator| (Matrix<T>& M, Matrix<T>& N)
{
	Matrix<T> Conc;
	Conc.row = M.row; Conc.column = M.column + N.column;
	Conc.elements.resize(Conc.row);
	for (int i = 0; i < Conc.row; i++)
		Conc.elements[i].resize(Conc.column);
	for (int i = 0; i < M.row; i++)
	{
		for (int j = 0; j < M.column; j++)
			Conc.elements[i][j] = M.elements[i][j];
		for (int j = M.column; j < M.column + N.column; j++)
			Conc.elements[i][j] = N.elements[i][j-M.column];
	}
	return Conc;
}

template <typename T>
Matrix<T> operator& (Matrix<T>& M, Matrix<T>& N)
{
	Matrix<T> Conc;
	Conc.row = M.row + N.row; Conc.column = M.column;
	Conc.elements.resize(Conc.row);
	for (int i = 0; i < Conc.row; i++)
		Conc.elements[i].resize(Conc.column);
	for (int j = 0; j < Conc.column; j++)
	{
		for (int i = 0; i < M.row; i++)
			Conc.elements[i][j] = M.elements[i][j];
		for (int i = M.row; i < Conc.row; i++)
			Conc.elements[i][j] = N.elements[i-M.row][j];
	}
	return Conc;
}

int main() {
	Matrix <int> mat1, mat2;
	mat1.row = 3; mat1.column = 3;
	cin >> mat1;
	cout << mat1(0, 1, 0, 1);
	return 0;
}