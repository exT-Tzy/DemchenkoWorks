#include "solver.h"
#include "vector.h"  

#include <fstream>
#include <sstream>
#include <iostream>

solver::solver() : writer(latex_writer::instance()) {}

bool solver::set_output(
    const std::string& filename)
{
    return writer.init(filename);
}

bool solver::process_file(
    const std::string& input_file)
{
    std::ifstream file(input_file);
   
    if (!file.is_open()) 
    {
        std::cerr << "Error: Cannot open input file " << input_file << std::endl;
      
        return false;
    }

    std::string line;
   
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#') continue; 

        std::istringstream iss(line);
        std::string task_name;
        iss >> task_name;

        std::string data;
        std::getline(iss, data);

        data.erase(0, data.find_first_not_of(" \t"));

        if (task_name == "SCALAR_DOUBLE_PRODUCT_ND") 
        {
            solve_scalar_double_product_nd(data);
        }
        else if (task_name == "VECTOR_DOUBLE_PRODUCT_3D") 
        {
            solve_vector_double_product_3d(data);
        }
        else if (task_name == "VECTOR_DOUBLE_PRODUCT_7D") 
        {
            solve_vector_double_product_7d(data); 
        }
        else if (task_name == "MIXED_TRIPLE_PRODUCT_3D")
        {
            solve_mixed_triple_product_3d(data);
        }
        else if (task_name == "MIXED_TRIPLE_PRODUCT_7D") 
        {
            solve_mixed_triple_product_7d(data);
        }
        else if (task_name == "VECTOR_ADDITION") 
        {
            solve_vector_addition(data);
        }
        else if (task_name == "VECTOR_SUBTRACTION") 
        {
            solve_vector_subtraction(data);
        }
        else if (task_name == "SCALAR_MULTIPLICATION") 
        {
            solve_vector_multiplication(data);
        }
        else if (task_name == "VECTOR_MODULUS") 
        {
            solve_vector_modulus(data);
        }
        else if (task_name == "ORTHOGONALIZATION_OF_VECTORS_SET") 
        {
            solve_orthogonalization_process(data);
        }
        else 
        {
            std::cerr << "Warning: Unknown task type: " << task_name << std::endl;
        }
    }

    file.close();
   
    return true;
}

void solver::close()
{
    writer.close();
}

Vector parse_vector(
    const std::string& vector_str)
{
    std::istringstream iss(vector_str);
    std::vector<bolshaya_drob> components;
    std::string component_str;

    while (iss >> component_str) 
    {
        size_t slash_pos = component_str.find('/');
       
        if (slash_pos != std::string::npos)
        {
            std::string num_str = component_str.substr(0, slash_pos);
            std::string den_str = component_str.substr(slash_pos + 1);
            components.emplace_back(
                boost::multiprecision::int1024_t(num_str),
                boost::multiprecision::int1024_t(den_str)
            );
        }
        else
        {
            components.emplace_back(boost::multiprecision::int1024_t(component_str));
        }
    }

    return Vector(components);
}

void solver::solve_scalar_double_product_nd(
    const std::string& data)
{
    std::istringstream iss(data);
    std::string vector1_str, vector2_str;

    std::getline(iss, vector1_str, ';');
    std::getline(iss, vector2_str);

    Vector v1 = parse_vector(vector1_str);
    Vector v2 = parse_vector(vector2_str);

    writer.write_line("\\section*{Scalar Product of Vectors in n-dimensional Space}");
    writer.write_line("\\textbf{Given:}");
    writer.write_line("$\\vec{a} = " + v1.to_latex() + "$");
    writer.write_line("$\\vec{b} = " + v2.to_latex() + "$");
    writer.write_line("");

    writer.write_line("\\textbf{Find:} $\\vec{a} \\cdot \\vec{b}$");
    writer.write_line("");

    writer.write_line("\\textbf{Solution:}");
    writer.write_line("The scalar product of vectors in n-dimensional space is calculated using the formula:");
    writer.begin_math();
    writer.write_line("\\vec{a} \\cdot \\vec{b} = \\sum_{i=1}^{n} a_i \\cdot b_i");
    writer.end_math();

    writer.write_line("Substituting the component values:");
    writer.begin_align();

    bolshaya_drob result(0);
   
    for (size_t i = 0; i < v1.dimension(); ++i)
    {
        bolshaya_drob product = v1[i] * v2[i];
        result += product;

        writer.write_line("a_{" + std::to_string(i + 1) + "} \\cdot b_{" + std::to_string(i + 1) + "} &= " +
            v1[i].to_decimal(6) + " \\cdot " + v2[i].to_decimal(6) + " = " + product.to_decimal(6) + " \\\\");
    }

    writer.end_align();

    writer.write_line("Summing all products:");
    writer.begin_math();
    writer.write_line("\\vec{a} \\cdot \\vec{b} = " + result.to_decimal(10));
    writer.end_math();

    writer.write_line("\\textbf{Answer:} $\\vec{a} \\cdot \\vec{b} = " + result.to_decimal(10) + "$");
    writer.write_line("\\newpage");
}

void solver::solve_vector_addition(
    const std::string& data)
{
    std::istringstream iss(data);
    std::string vector1_str, vector2_str;

    std::getline(iss, vector1_str, ';');
    std::getline(iss, vector2_str);

    Vector v1 = parse_vector(vector1_str);
    Vector v2 = parse_vector(vector2_str);

    writer.write_line("\\section*{Vector Addition}");
    writer.write_line("\\textbf{Given:}");
    writer.write_line("$\\vec{a} = " + v1.to_latex() + "$");
    writer.write_line("$\\vec{b} = " + v2.to_latex() + "$");
    writer.write_line("");

    writer.write_line("\\textbf{Find:} $\\vec{a} + \\vec{b}$");
    writer.write_line("");

    writer.write_line("\\textbf{Solution:}");
    writer.write_line("Vector addition is performed component-wise:");
    writer.begin_math();
    writer.write_line("\\vec{a} + \\vec{b} = (a_1 + b_1, a_2 + b_2, \\ldots, a_n + b_n)");
    writer.end_math();

    Vector result = v1 + v2;

    writer.write_line("Computing each component:");
    writer.begin_align();
   
    for (size_t i = 0; i < v1.dimension(); ++i) 
    {
        writer.write_line("c_{" + std::to_string(i + 1) + "} &= a_{" + std::to_string(i + 1) + "} + b_{" +
            std::to_string(i + 1) + "} = " + v1[i].to_decimal(6) + " + " + v2[i].to_decimal(6) +
            " = " + result[i].to_decimal(6) + " \\\\");
    }
    writer.end_align();

    writer.write_line("\\textbf{Answer:} $\\vec{a} + \\vec{b} = " + result.to_latex() + "$");
    writer.write_line("\\newpage");
}

void solver::solve_vector_subtraction(
    const std::string& data)
{
    std::istringstream iss(data);
    std::string vector1_str, vector2_str;

    std::getline(iss, vector1_str, ';');
    std::getline(iss, vector2_str);

    Vector v1 = parse_vector(vector1_str);
    Vector v2 = parse_vector(vector2_str);

    writer.write_line("\\section*{Vector Subtraction}");
    writer.write_line("\\textbf{Given:}");
    writer.write_line("$\\vec{a} = " + v1.to_latex() + "$");
    writer.write_line("$\\vec{b} = " + v2.to_latex() + "$");
    writer.write_line("");

    writer.write_line("\\textbf{Find:} $\\vec{a} - \\vec{b}$");
    writer.write_line("");

    writer.write_line("\\textbf{Solution:}");
    writer.write_line("Vector subtraction is performed component-wise:");
    writer.begin_math();
    writer.write_line("\\vec{a} - \\vec{b} = (a_1 - b_1, a_2 - b_2, \\ldots, a_n - b_n)");
    writer.end_math();

    Vector result = v1 - v2;

    writer.write_line("Computing each component:");
    writer.begin_align();
   
    for (size_t i = 0; i < v1.dimension(); ++i) 
    {
        writer.write_line("c_{" + std::to_string(i + 1) + "} &= a_{" + std::to_string(i + 1) + "} - b_{" +
            std::to_string(i + 1) + "} = " + v1[i].to_decimal(6) + " - " + v2[i].to_decimal(6) +
            " = " + result[i].to_decimal(6) + " \\\\");
    }
    writer.end_align();

    writer.write_line("\\textbf{Answer:} $\\vec{a} - \\vec{b} = " + result.to_latex() + "$");
    writer.write_line("\\newpage");
}

void solver::solve_vector_multiplication(
    const std::string& data)
{
    std::istringstream iss(data);
    std::string scalar_str, vector_str;

    std::getline(iss, scalar_str, ';');
    std::getline(iss, vector_str);

    bolshaya_drob scalar;
    size_t slash_pos = scalar_str.find('/');
   
    if (slash_pos != std::string::npos)
    {
        std::string num_str = scalar_str.substr(0, slash_pos);
        std::string den_str = scalar_str.substr(slash_pos + 1);
        scalar = bolshaya_drob(boost::multiprecision::int1024_t(num_str),
            boost::multiprecision::int1024_t(den_str));
    }
    else 
    {
        scalar = bolshaya_drob(boost::multiprecision::int1024_t(scalar_str));
    }

    Vector v = parse_vector(vector_str);

    writer.write_line("\\section*{Vector Scalar Multiplication}");
    writer.write_line("\\textbf{Given:}");
    writer.write_line("$k = " + scalar.to_decimal(6) + "$");
    writer.write_line("$\\vec{a} = " + v.to_latex() + "$");
    writer.write_line("");

    writer.write_line("\\textbf{Find:} $k \\cdot \\vec{a}$");
    writer.write_line("");

    writer.write_line("\\textbf{Solution:}");
    writer.write_line("Vector scalar multiplication is performed component-wise:");
    writer.begin_math();
    writer.write_line("k \\cdot \\vec{a} = (k \\cdot a_1, k \\cdot a_2, \\ldots, k \\cdot a_n)");
    writer.end_math();

    Vector result = v * scalar;

    writer.write_line("Computing each component:");
    writer.begin_align();
   
    for (size_t i = 0; i < v.dimension(); ++i) 
    {
        writer.write_line("c_{" + std::to_string(i + 1) + "} &= k \\cdot a_{" + std::to_string(i + 1) + "} = " +
            scalar.to_decimal(6) + " \\cdot " + v[i].to_decimal(6) + " = " + result[i].to_decimal(6) + " \\\\");
    }
    writer.end_align();

    writer.write_line("\\textbf{Answer:} $k \\cdot \\vec{a} = " + result.to_latex() + "$");
    writer.write_line("\\newpage");
}

void solver::solve_vector_modulus(
    const std::string& data)
{
    Vector v = parse_vector(data);

    writer.write_line("\\section*{Vector Magnitude}");
    writer.write_line("\\textbf{Given:}");
    writer.write_line("$\\vec{a} = " + v.to_latex() + "$");
    writer.write_line("");

    writer.write_line("\\textbf{Find:} $|\\vec{a}|$");
    writer.write_line("");

    writer.write_line("\\textbf{Solution:}");
    writer.write_line("Vector magnitude is calculated using the formula:");
    writer.begin_math();
    writer.write_line("|\\vec{a}| = \\sqrt{a_1^2 + a_2^2 + \\ldots + a_n^2}");
    writer.end_math();

    writer.write_line("Computing the sum of squares of components:");
    writer.begin_align();

    bolshaya_drob sum_of_squares(0);
   
    for (size_t i = 0; i < v.dimension(); ++i) 
    {
        bolshaya_drob square = v[i] * v[i];
        sum_of_squares += square;
        writer.write_line("a_{" + std::to_string(i + 1) + "}^2 &= (" + v[i].to_decimal(6) + ")^2 = " + square.to_decimal(6) + " \\\\");
    }

    writer.write_line("\\sum &= " + sum_of_squares.to_decimal(10) + " \\\\");
    writer.end_align();

    bolshaya_drob result = v.magnitude();

    writer.write_line("Taking the square root:");
    writer.begin_math();
    writer.write_line("|\\vec{a}| = \\sqrt{" + sum_of_squares.to_decimal(10) + "} = " + result.to_decimal(10));
    writer.end_math();

    writer.write_line("\\textbf{Answer:} $|\\vec{a}| = " + result.to_decimal(10) + "$");
    writer.write_line("\\newpage");
}

void solver::solve_vector_double_product_3d(
    const std::string& data)
{
    std::istringstream iss(data);
    std::string vector1_str, vector2_str;

    std::getline(iss, vector1_str, ';');
    std::getline(iss, vector2_str);

    Vector v1 = parse_vector(vector1_str);
    Vector v2 = parse_vector(vector2_str);

    writer.write_line("\\section*{Cross Product in 3D Space}");
    writer.write_line("\\textbf{Given:}");
    writer.write_line("$\\vec{a} = " + v1.to_latex() + "$");
    writer.write_line("$\\vec{b} = " + v2.to_latex() + "$");
    writer.write_line("");

    writer.write_line("\\textbf{Find:} $\\vec{a} \\times \\vec{b}$");
    writer.write_line("");

    writer.write_line("\\textbf{Solution:}");
    writer.write_line("Cross product in 3D space is calculated using the determinant:");
    writer.begin_math();
    writer.write_line("\\vec{a} \\times \\vec{b} = \\begin{vmatrix} \\vec{i} & \\vec{j} & \\vec{k} \\\\ a_1 & a_2 & a_3 \\\\ b_1 & b_2 & b_3 \\end{vmatrix}");
    writer.end_math();

    Vector result = v1.cross_3d(v2);

    writer.write_line("Substituting values:");
    writer.begin_math();
    writer.write_line("\\vec{a} \\times \\vec{b} = \\begin{vmatrix} \\vec{i} & \\vec{j} & \\vec{k} \\\\ " +
        v1[0].to_decimal(6) + " & " + v1[1].to_decimal(6) + " & " + v1[2].to_decimal(6) + " \\\\ " +
        v2[0].to_decimal(6) + " & " + v2[1].to_decimal(6) + " & " + v2[2].to_decimal(6) + " \\end{vmatrix}");
    writer.end_math();

    writer.write_line("Computing components:");
    writer.begin_align();
    writer.write_line("(\\vec{a} \\times \\vec{b})_1 &= a_2 \\cdot b_3 - a_3 \\cdot b_2 = " +
        v1[1].to_decimal(6) + " \\cdot " + v2[2].to_decimal(6) + " - " +
        v1[2].to_decimal(6) + " \\cdot " + v2[1].to_decimal(6) + " = " + result[0].to_decimal(6) + " \\\\");
    writer.write_line("(\\vec{a} \\times \\vec{b})_2 &= a_3 \\cdot b_1 - a_1 \\cdot b_3 = " +
        v1[2].to_decimal(6) + " \\cdot " + v2[0].to_decimal(6) + " - " +
        v1[0].to_decimal(6) + " \\cdot " + v2[2].to_decimal(6) + " = " + result[1].to_decimal(6) + " \\\\");
    writer.write_line("(\\vec{a} \\times \\vec{b})_3 &= a_1 \\cdot b_2 - a_2 \\cdot b_1 = " +
        v1[0].to_decimal(6) + " \\cdot " + v2[1].to_decimal(6) + " - " +
        v1[1].to_decimal(6) + " \\cdot " + v2[0].to_decimal(6) + " = " + result[2].to_decimal(6) + " \\\\");
    writer.end_align();

    writer.write_line("\\textbf{Answer:} $\\vec{a} \\times \\vec{b} = " + result.to_latex() + "$");
    writer.write_line("\\newpage");
}

void solver::solve_vector_double_product_7d(
    const std::string& data)
{
    std::istringstream iss(data);
    std::string vector1_str, vector2_str;

    std::getline(iss, vector1_str, ';');
    std::getline(iss, vector2_str);

    Vector v1 = parse_vector(vector1_str);
    Vector v2 = parse_vector(vector2_str);

    writer.write_line("\\section*{Cross Product in 7D Space}");
    writer.write_line("\\textbf{Given:}");
    writer.write_line("$\\vec{a} = " + v1.to_latex() + "$");
    writer.write_line("$\\vec{b} = " + v2.to_latex() + "$");
    writer.write_line("");

    writer.write_line("\\textbf{Find:} $\\vec{a} \\times \\vec{b}$");
    writer.write_line("");

    writer.write_line("\\textbf{Solution:}");
    writer.write_line("Cross product in 7D space is based on octonion algebra.");
    writer.write_line("Using the octonion multiplication table for basis vectors:");

    Vector result = v1.cross_7d(v2);

    writer.write_line("After applying the formulas, we get:");
    writer.write_line("\\textbf{Answer:} $\\vec{a} \\times \\vec{b} = " + result.to_latex() + "$");
    writer.write_line("\\newpage");
}

void solver::solve_mixed_triple_product_3d(
    const std::string& data)
{
    std::istringstream iss(data);
    std::string vector1_str, vector2_str, vector3_str;

    std::getline(iss, vector1_str, ';');
    std::getline(iss, vector2_str, ';');
    std::getline(iss, vector3_str);

    Vector v1 = parse_vector(vector1_str);
    Vector v2 = parse_vector(vector2_str);
    Vector v3 = parse_vector(vector3_str);

    writer.write_line("\\section*{Mixed Triple Product in 3D}");
    writer.write_line("\\textbf{Given:}");
    writer.write_line("$\\vec{a} = " + v1.to_latex() + "$");
    writer.write_line("$\\vec{b} = " + v2.to_latex() + "$");
    writer.write_line("$\\vec{c} = " + v3.to_latex() + "$");
    writer.write_line("");

    writer.write_line("\\textbf{Find:} $\\vec{a} \\cdot (\\vec{b} \\times \\vec{c})$");
    writer.write_line("");

    writer.write_line("\\textbf{Solution:}");
    writer.write_line("Mixed triple product equals the determinant of the matrix formed by vector components:");
    writer.begin_math();
    writer.write_line("\\vec{a} \\cdot (\\vec{b} \\times \\vec{c}) = \\begin{vmatrix} a_1 & a_2 & a_3 \\\\ b_1 & b_2 & b_3 \\\\ c_1 & c_2 & c_3 \\end{vmatrix}");
    writer.end_math();

    Vector cross_bc = v2.cross_3d(v3);
    writer.write_line("First compute $\\vec{b} \\times \\vec{c}$:");
    writer.write_line("$\\vec{b} \\times \\vec{c} = " + cross_bc.to_latex() + "$");
    writer.write_line("");

    bolshaya_drob result = v1.mixed_triple_3d(v2, v3);

    writer.write_line("Then compute the scalar product:");
    writer.begin_math();
    writer.write_line("\\vec{a} \\cdot (\\vec{b} \\times \\vec{c}) = " + result.to_decimal(10));
    writer.end_math();

    writer.write_line("\\textbf{Answer:} $\\vec{a} \\cdot (\\vec{b} \\times \\vec{c}) = " + result.to_decimal(10) + "$");
    writer.write_line("\\newpage");
}

void solver::solve_mixed_triple_product_7d(
    const std::string& data)
{
    std::istringstream iss(data);
    std::string vector1_str, vector2_str, vector3_str;

    std::getline(iss, vector1_str, ';');
    std::getline(iss, vector2_str, ';');
    std::getline(iss, vector3_str);

    Vector v1 = parse_vector(vector1_str);
    Vector v2 = parse_vector(vector2_str);
    Vector v3 = parse_vector(vector3_str);

    writer.write_line("\\section*{Mixed Triple Product in 7D}");
    writer.write_line("\\textbf{Given:}");
    writer.write_line("$\\vec{a} = " + v1.to_latex() + "$");
    writer.write_line("$\\vec{b} = " + v2.to_latex() + "$");
    writer.write_line("$\\vec{c} = " + v3.to_latex() + "$");
    writer.write_line("");

    writer.write_line("\\textbf{Find:} $\\vec{a} \\cdot (\\vec{b} \\times \\vec{c})$");
    writer.write_line("");

    writer.write_line("\\textbf{Solution:}");
    writer.write_line("For 7D space, we use cross product based on octonion algebra.");

    Vector cross_bc = v2.cross_7d(v3);
    writer.write_line("First compute $\\vec{b} \\times \\vec{c}$:");
    writer.write_line("$\\vec{b} \\times \\vec{c} = " + cross_bc.to_latex() + "$");
    writer.write_line("");

    bolshaya_drob result = v1.mixed_triple_7d(v2, v3);

    writer.write_line("Then compute the scalar product:");
    writer.begin_math();
    writer.write_line("\\vec{a} \\cdot (\\vec{b} \\times \\vec{c}) = " + result.to_decimal(10));
    writer.end_math();

    writer.write_line("\\textbf{Answer:} $\\vec{a} \\cdot (\\vec{b} \\times \\vec{c}) = " + result.to_decimal(10) + "$");
    writer.write_line("\\newpage");
}

void solver::solve_orthogonalization_process(
    const std::string& data)
{
    std::istringstream iss(data);
    std::vector<Vector> vectors;
    std::string vector_str;

    while (std::getline(iss, vector_str, ';')) 
    {
        if (!vector_str.empty()) 
        {
            vectors.push_back(parse_vector(vector_str));
        }
    }

    writer.write_line("\\section*{Gram-Schmidt Orthogonalization Process}");
    writer.write_line("\\textbf{Given set of vectors:}");

    for (size_t i = 0; i < vectors.size(); ++i)
    {
        writer.write_line("$\\vec{v}_{" + std::to_string(i + 1) + "} = " + vectors[i].to_latex() + "$");
    }
    writer.write_line("");

    writer.write_line("\\textbf{Find:} orthogonal system of vectors");
    writer.write_line("");

    writer.write_line("\\textbf{Solution:}");
    writer.write_line("Apply the Gram-Schmidt orthogonalization process:");
    writer.write_line("");

    std::vector<Vector> orthogonal = gram_schmidt_orthogonalization(vectors);

    writer.write_line("\\textbf{Step 1:} First vector remains unchanged:");
    writer.write_line("$\\vec{u}_1 = \\vec{v}_1 = " + vectors[0].to_latex() + "$");
    writer.write_line("");

    for (size_t i = 1; i < vectors.size() && i < orthogonal.size(); ++i)
    {
        writer.write_line("\\textbf{Step " + std::to_string(i + 1) + ":}");
        writer.write_line("Compute projections of $\\vec{v}_{" + std::to_string(i + 1) + "}$ onto previous orthogonal vectors:");

        Vector current = vectors[i];
        
        for (size_t j = 0; j < i; ++j)
        {
            Vector projection = current.project_onto(orthogonal[j]);
            writer.write_line("Projection onto $\\vec{u}_{" + std::to_string(j + 1) + "}$: " + projection.to_latex_row());
            current = current - projection;
        }

        writer.write_line("$\\vec{u}_{" + std::to_string(i + 1) + "} = " + orthogonal[i].to_latex() + "$");
        writer.write_line("");
    }

    writer.write_line("\\textbf{Orthogonalization result:}");
    
    for (size_t i = 0; i < orthogonal.size(); ++i) 
    {
        writer.write_line("$\\vec{u}_{" + std::to_string(i + 1) + "} = " + orthogonal[i].to_latex() + "$");
    }

    writer.write_line("\\newpage");
}