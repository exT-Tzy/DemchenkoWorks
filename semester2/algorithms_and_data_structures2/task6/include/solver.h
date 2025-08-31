#ifndef SOLVER_H
#define SOLVER_H 

#include "latex_writer.h"

class solver final
{
private:

	latex_writer& writer;

	enum task_type
	{
		SCALAR_DOUBLE_PRODUCT_ND,
		VECTOR_DOUBLE_PRODUCT_3D,
		VECTOR_DOUBLE_PRODUCT_7D,
		MIXED_TRIPLE_PRODUCT_3D,
		MIXED_TRIPLE_PRODUCT_7D,
		VECTOR_ADDITION,
		VECTOR_SUBTRACTION,
		SCALAR_MULTIPLICATION,
		VECTOR_MODULUS,
		ORTHOGONALIZATION_OF_VECTORS_SET
	};

private:

	void solve_scalar_double_product_nd(
		std::string const& data);

	void solve_vector_double_product_3d(
		std::string const& data);

	void solve_vector_double_product_7d(
		std::string const& data);

	void solve_mixed_triple_product_3d(
		std::string const& data);

	void solve_mixed_triple_product_7d(
		std::string const& data);

	void solve_vector_addition(
		std::string const& data);

	void solve_vector_subtraction(
		std::string const& data);

	void solve_vector_multiplication(
		std::string const& data);

	void solve_vector_modulus(
		std::string const& data);

	void solve_orthogonalization_process(
		std::string const& data);

public:

	solver();

	bool set_output(
		std::string const& filename);

	bool process_file(
		std::string const& input_file);

	void close();
};

#endif //SOLVER_H