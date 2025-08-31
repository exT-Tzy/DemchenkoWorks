#include "solver.h"

#include <iostream>
#include <string>

int main(
    int argc, 
    char* argv[])
{
    if (argc != 3) 
    {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        std::cerr << "Example: " << argv[0] << " tasks.txt solutions.tex" << std::endl;
       
        return 1;
    }

    std::string input_file = argv[1];
    std::string output_file = argv[2];

    solver problem_solver;

    if (!problem_solver.set_output(output_file)) 
    {
        std::cerr << "Error: Cannot create output file " << output_file << std::endl;
       
        return 1;
    }

    std::cout << "Linear Algebra Problem Solver" << std::endl;
    std::cout << "=============================" << std::endl;
    std::cout << "Input file: " << input_file << std::endl;
    std::cout << "Output file: " << output_file << std::endl;
    std::cout << std::endl;

    if (!problem_solver.process_file(input_file)) 
    {
        std::cerr << "Error: Failed to process input file " << input_file << std::endl;
        problem_solver.close();
       
        return 1;
    }

    problem_solver.close();

    std::cout << "Processing completed successfully!" << std::endl;
    std::cout << "LaTeX file generated: " << output_file << std::endl;
    std::cout << std::endl;
    std::cout << "To generate PDF, run:" << std::endl;
    std::cout << "pdflatex " << output_file << std::endl;

    return 0;
}
