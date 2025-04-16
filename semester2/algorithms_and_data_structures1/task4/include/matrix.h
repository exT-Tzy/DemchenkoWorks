#ifndef MATRIX_H
#define MATRIX_H

class matrix final
{
private:

	size_t _rows, _cols;

	double** _matrix_data;

	void alloc_mem();

	void dealloc_mem();

public:

	matrix& operator+=(
		matrix const&);

	matrix operator+(
		matrix const&) const;

	matrix& operator-=(
		matrix const&);

	matrix operator-(
		matrix const&) const;

	matrix& operator*=(
		matrix const&);

	matrix operator*(
		matrix const&) const;

	matrix& operator*=(
		double);

	matrix operator*(
		double) const;

	friend matrix operator*(
		double,
		matrix const&);

	matrix transpose() const;

	double det() const;

	matrix inverse() const;

	double* operator[](
		size_t) const;

	friend std::ostream& operator<<(
		std::ostream&,
		matrix const&);
	
	matrix(
		size_t = 0, 
		size_t = 0);

	matrix& operator=(
		matrix const&);

	matrix(
		matrix const&);

	~matrix() noexcept;
};

#endif // MATRIX_H