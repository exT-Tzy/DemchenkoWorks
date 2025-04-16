#include <iostream>
#include <stdexcept>

#include "matrix.h"

void matrix::alloc_mem()
{
	_matrix_data = new double* [_rows];
	
	for (size_t i = 0; i < _rows; ++i)
	{
		_matrix_data[i] = new double[_cols];
		
		for (size_t j = 0; j < _cols; ++j)
		{
			_matrix_data[i][j] = 0;
		}
	}
}

void matrix::dealloc_mem()
{
	for (size_t i = 0; i < _rows; ++i)
	{
		delete[] _matrix_data[i];
	}

	delete[] _matrix_data;
}

matrix& matrix::operator+=(
	matrix const& other)
{
	if (_rows != other._rows || _cols != other._cols)
		throw std::invalid_argument(
			"Matrices must have equal sizes!");

	for (size_t i = 0; i < _rows; ++i)
	{
		for (size_t j = 0; j < _cols; ++j)
		{
			_matrix_data[i][j] += other._matrix_data[i][j];
		}
	}

	return *this;
}

matrix matrix::operator+(
	matrix const& other) const
{
	matrix copy = *this;
	
	return copy += other;
}

matrix& matrix::operator-=(
	matrix const& other)
{
	if (_rows != other._rows || _cols != other._cols)
		throw std::invalid_argument(
			"Matrices must have equal sizes!");

	for (size_t i = 0; i < _rows; ++i)
	{
		for (size_t j = 0; j < _cols; ++j)
		{
			_matrix_data[i][j] -= other._matrix_data[i][j];
		}
	}

	return *this;
}

matrix matrix::operator-(
	matrix const& other) const
{
	matrix copy = *this;

	return copy -= other;
}

matrix& matrix::operator*=(
	matrix const& other)
{
	if (_cols != other._rows)
		throw std::invalid_argument(
			"1st matrix columns count must be equal to 2nd matrix rows count!");

	double** new_data = new double* [_rows];

	for (size_t i = 0; i < other._rows; ++i)
	{
		new_data[i] = new double[other._cols];
	}

	double temp;

	for (size_t i = 0; i < _rows; ++i)
	{
		for (size_t j = 0; j < other._cols; ++j)
		{
			temp = 0;

			for (size_t k = 0; k < _cols; ++k)
			{
				temp += _matrix_data[i][k] * other._matrix_data[k][j];
			}
			
			new_data[i][j] = temp;
		}
	}

	dealloc_mem();

	_rows = _cols;
	_cols = other._rows;

	_matrix_data = new_data;

	return *this;
}

matrix matrix::operator*(
	matrix const& other) const
{
	matrix copy = *this;

	return copy *= other;
}

matrix& matrix::operator*=(
	double num)
{
	for (size_t i = 0; i < _rows; ++i)
	{
		for (size_t j = 0; j < _cols; ++j)
		{
			_matrix_data[i][j] *= num;
		}
	}

	return *this;
}

matrix matrix::operator*(
	double num) const
{
	matrix copy = *this;

	return copy *= num;
}

matrix operator*(
	double num,
	matrix const& matr)
{
	return matr * num;
}

matrix matrix::transpose() const
{
	matrix new_matrix(_cols, _rows);

	for (size_t i = 0; i < _rows; ++i)
	{
		for (size_t j = 0; j < _cols; ++j)
		{
			new_matrix._matrix_data[i][j] = _matrix_data[j][i];
		}
	}

	return new_matrix;
}

double matrix::det() const
{
	if (_rows != _cols) throw std::invalid_argument(
		"Determinant is defined for square matrices only!");
	
	matrix temp(*this);
	double det = 1.0;
	
	for (size_t i = 0; i < _rows; ++i) 
	{
		size_t pivot = i;
		
		for (size_t j = i + 1; j < _rows; ++j) 
		{
			if (std::abs(temp._matrix_data[j][i]) > std::abs(temp._matrix_data[pivot][i])) 
			{
				pivot = j;
			}
		}
		
		if (pivot != i) 
		{
			std::swap(temp._matrix_data[i], temp._matrix_data[pivot]);
			
			det *= -1;
		}
		
		if (temp._matrix_data[i][i] == 0) 
		{
			return 0;
		}
		
		det *= temp._matrix_data[i][i];
		
		for (size_t j = i + 1; j < _rows; ++j) 
		{
			double factor = temp._matrix_data[j][i] / temp._matrix_data[i][i];
			
			for (size_t k = i + 1; k < _cols; ++k) 
			{
				temp._matrix_data[j][k] -= factor * temp._matrix_data[i][k];
			}
		}
	}
	
	return det;
}

matrix matrix::inverse() const
{
	if (_rows != _cols) throw std::invalid_argument(
		"Inverse is defined for square matrices only!");

	double det = this->det();
	
	if (det == 0.0) throw std::invalid_argument(
		"Matrix is singular and cannot be inverted!");

	matrix temp(_rows, _cols * 2);
	
	for (size_t i = 0; i < _rows; ++i) 
	{
		for (size_t j = 0; j < _cols; ++j) 
		{
			temp._matrix_data[i][j] = _matrix_data[i][j];
		}
		
		temp._matrix_data[i][_cols + i] = 1;
	}
	
	for (size_t i = 0; i < _rows; ++i) 
	{
		size_t pivot = i;
		
		for (size_t j = i + 1; j < _rows; ++j) 
		{
			if (std::abs(temp._matrix_data[j][i]) > std::abs(temp._matrix_data[pivot][i])) 
			{
				pivot = j;
			}
		}
		
		if (pivot != i) 
		{
			std::swap(temp._matrix_data[i], temp._matrix_data[pivot]);
		}
		
		double factor = temp._matrix_data[i][i];
		
		for (size_t j = 0; j < _cols * 2; ++j) 
		{
			temp._matrix_data[i][j] /= factor;
		}
		
		for (size_t j = 0; j < _rows; ++j) 
		{
			if (j != i) 
			{
				factor = temp._matrix_data[j][i];
				
				for (size_t k = 0; k < _cols * 2; ++k) 
				{
					temp._matrix_data[j][k] -= factor * temp._matrix_data[i][k];
				}
			}
		}
	}
	
	matrix result(_rows, _cols);

	for (size_t i = 0; i < _rows; ++i) 
	{
		for (size_t j = 0; j < _cols; ++j) 
		{
			result._matrix_data[i][j] = temp._matrix_data[i][_cols + j];
		}
	}
	
	return result;
}

double* matrix::operator[](
	size_t pos) const
{
	if (pos >= _rows) throw std::invalid_argument(
		"The index is greater than the rows count of matrix!");

	return _matrix_data[pos];
}

std::ostream& operator<<(
	std::ostream& os,
	matrix const& src)
{
	for (size_t i = 0; i < src._rows; ++i)
	{
		for (size_t j = 0; j < src._cols; ++j)
		{
			os << src._matrix_data[i][j] << ' ';
		}

		os << std::endl;
	}

	os << std::endl;

	return os;
}

matrix::matrix(
	size_t rows, 
	size_t cols) : _rows(rows), 
				   _cols(cols)
{
	alloc_mem();
}

matrix& matrix::operator=(
	matrix const& src)
{
	if (&src == this) return *this;

	if (_rows != src._rows || _cols != src._cols)
	{
		dealloc_mem();

		_rows = src._rows;
		_cols = src._cols;

		alloc_mem();
	}

	for (size_t i = 0; i < _rows; ++i)
	{
		for (size_t j = 0; j < _cols; ++j)
		{
			_matrix_data[i][j] = src._matrix_data[i][j];
		}
	}

	return *this;
}

matrix::matrix(
	matrix const& src)
{
	_rows = src._rows;
	_cols = src._cols;

	alloc_mem();

	*this = src;
}

matrix::~matrix() noexcept
{
	dealloc_mem();
}