#include "polynomial.h"
#include "horner_methods.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

void task_1()
{
   std::cout << "1) хороший метод вычисления нечетного полинома" << std::endl;

   std::vector<double> odd_coeffs = {0, 2, 0, 3, 0, 5};
   polynomial u(odd_coeffs);

   std::cout << "исходный полином: ";
   horner_methods::print_polynomial(u);

   std::cout << "\nстандартный метод:" << std::endl;
   int degree = u.degree();
   int n_terms = 0;
   for (int i = 1; i <= degree; i += 2)
   {
      if (std::abs(u.get_coeff(i)) > 1e-10)
      {
         n_terms++;
      }
   }
   int standard_mults = degree;
   int standard_adds = n_terms;
   std::cout << "операций: " << standard_mults << " умножений + " << standard_adds << " сложений" << std::endl;

   std::cout << "\nхороший метод (факторизация через x):" << std::endl;

   std::vector<double> even_coeffs;
   for (int i = 1; i <= degree; i += 2)
   {
      even_coeffs.push_back(u.get_coeff(i));
   }
   polynomial even_poly(even_coeffs);

   std::cout << "четный полином по y=x^2: ";
   horner_methods::print_polynomial(even_poly);

   int even_degree = even_poly.degree();
   int good_mults_y = 1;
   int good_mults_horner = even_degree;
   int good_mults_final = 1;
   int good_mults_total = good_mults_y + good_mults_horner + good_mults_final;
   int good_adds = even_degree;

   std::cout << "операций: " << good_mults_y << " (для y=x^2) + " << good_mults_horner << " (горнер) + " << good_mults_final << " (умножить на x) = " << good_mults_total << " умножений" << std::endl;
   std::cout << "         " << good_adds << " сложений" << std::endl;

   double x = 2.0;
   std::cout << "\nпроверка при x = " << x << ":" << std::endl;

   double standard = u(x);
   std::cout << "стандартный метод: " << standard << std::endl;

   double y = x * x;
   double good_method = x * even_poly(y);
   std::cout << "хороший метод: " << good_method << std::endl;
   std::cout << "совпадают: " << (std::abs(standard - good_method) < 1e-10 ? "да" : "нет") << std::endl;
   std::cout << std::endl;
}

void task_2()
{
   std::cout << "2) вычисление u(x + x0) через горнер" << std::endl;

   polynomial u({4, 3, 2, 1});
   double x0 = 1.0;

   std::cout << "исходный полином: ";
   horner_methods::print_polynomial(u);
   std::cout << "x0 = " << x0 << std::endl;

   std::cout << "\nразложение по степеням (x - " << x0 << ") методом горнера:" << std::endl;

   polynomial temp = u;
   std::vector<double> taylor_coeffs;
   int total_ops = 0;

   for (int k = 0; k <= u.degree(); k++)
   {
      auto result = horner_methods::evaluate_with_division(temp, x0);
      taylor_coeffs.push_back(result.remainder);

      std::cout << "шаг " << k << ": c" << k << " = " << result.remainder << std::endl;

      total_ops += result.multiplications + result.additions;

      temp = result.quotient;
      if (temp.degree() < 0)
         break;
   }

   polynomial u_shifted(taylor_coeffs);
   std::cout << "\nрезультат u(x + " << x0 << "): ";
   horner_methods::print_polynomial(u_shifted);

   double x_test = 2.0;
   std::cout << "\nпроверка в точке x = " << x_test << ":" << std::endl;
   double direct = u(x_test + x0);
   double via_shift = u_shifted(x_test);
   std::cout << "u(" << x_test << " + " << x0 << ") напрямую = " << direct << std::endl;
   std::cout << "u_shifted(" << x_test << ") = " << via_shift << std::endl;
   std::cout << "совпадают: " << (std::abs(direct - via_shift) < 1e-10 ? "да" : "нет") << std::endl;

   int n = u.degree();
   int h1_applications = n + 1;
   std::cout << "\nколичество операций:" << std::endl;
   std::cout << "h1 применяется " << h1_applications << " раз" << std::endl;
   std::cout << "фактически использовано: " << total_ops << " операций" << std::endl;
   std::cout << std::endl;
}

void task_3()
{
   std::cout << "3) вычисление полинома от двух переменных" << std::endl;

   std::vector<std::vector<double>> coeffs = {
       {1, 3, 6},
       {2, 5, 0},
       {4, 0, 0}};

   int n = coeffs.size() - 1;

   std::cout << "p(x,y) = ";
   bool first = true;
   for (size_t i = 0; i < coeffs.size(); i++)
   {
      for (size_t j = 0; j < coeffs[i].size(); j++)
      {
         double c = coeffs[i][j];
         if (std::abs(c) < 1e-10)
            continue;

         if (!first)
            std::cout << " + ";

         std::cout << c;
         if (i > 0)
            std::cout << "x";
         if (i > 1)
            std::cout << "^" << i;
         if (j > 0)
            std::cout << "y";
         if (j > 1)
            std::cout << "^" << j;

         first = false;
      }
   }
   std::cout << std::endl;
   std::cout << "степень n = " << n << std::endl;

   std::cout << "\nматрица коэффициентов:" << std::endl;
   std::cout << "      ";
   for (int j = 0; j <= n; j++)
   {
      std::cout << " y^" << j;
   }
   std::cout << std::endl;

   for (int i = 0; i <= n; i++)
   {
      std::cout << "  x^" << i << ":";
      for (size_t j = 0; j < coeffs[i].size(); j++)
      {
         std::cout << "  " << std::setw(2) << coeffs[i][j];
      }
      std::cout << std::endl;
   }

   std::cout << "\nметод аналогичный горнеру:" << std::endl;
   std::cout << "шаг 1: вычисляем полиномы по y для каждой степени x" << std::endl;

   double x = 2.0, y = 1.5;
   std::cout << "\nвычисление при x = " << x << ", y = " << y << ":" << std::endl;

   std::vector<double> p_vals;
   int mults_y = 0;
   int adds_y = 0;

   for (int i = n; i >= 0; i--)
   {
      polynomial p_i(coeffs[i]);
      double val = p_i(y);
      p_vals.push_back(val);
      std::cout << "p" << i << "(y) = " << val << std::endl;

      int deg_i = p_i.degree();
      mults_y += deg_i;
      adds_y += deg_i;
   }

   std::cout << "\nшаг 2: применяем горнер по x" << std::endl;

   double result = p_vals[0];
   int mults_x = 0;
   int adds_x = 0;

   for (size_t i = 1; i < p_vals.size(); i++)
   {
      result = result * x + p_vals[i];
      mults_x++;
      adds_x++;
   }

   std::cout << "p(" << x << ", " << y << ") = " << result << std::endl;

   double direct = 0;
   for (size_t i = 0; i < coeffs.size(); i++)
   {
      for (size_t j = 0; j < coeffs[i].size(); j++)
      {
         double term = coeffs[i][j];
         for (size_t pi = 0; pi < i; pi++)
            term *= x;
         for (size_t pj = 0; pj < j; pj++)
            term *= y;
         direct += term;
      }
   }

   std::cout << "\nпроверка напрямую: " << direct << std::endl;
   std::cout << "совпадают: " << (std::abs(result - direct) < 1e-10 ? "да" : "нет") << std::endl;

   std::cout << "\nподсчет операций для степени n=" << n << ":" << std::endl;
   std::cout << "умножений: " << mults_y << " (по y) + " << mults_x << " (по x) = " << (mults_y + mults_x) << std::endl;
   std::cout << "сложений: " << adds_y << " (по y) + " << adds_x << " (по x) = " << (adds_y + adds_x) << std::endl;
   std::cout << "итого: " << (mults_y + mults_x + adds_y + adds_x) << " операций" << std::endl;
   std::cout << std::endl;
}

void task_4()
{
   std::cout << "4) сравнение схемы 3 с правилом горнера" << std::endl;

   std::vector<double> real_coeffs = {4, 3, 2, 1};
   polynomial p(real_coeffs);

   std::cout << "полином: ";
   horner_methods::print_polynomial(p);

   double z_real = 1.0;
   double z_imag = 2.0;
   std::cout << "z = " << z_real << " + " << z_imag << "i (комплексное число)" << std::endl;

   std::cout << "\nвычисление горнером с комплексными числами:" << std::endl;
   std::cout << "умножение: (a+bi)*(c+di) = (ac-bd) + (ad+bc)i" << std::endl;

   int real_mults_per_complex_mult = 4;
   int real_adds_per_complex_mult = 2;

   std::cout << "  " << real_mults_per_complex_mult << " вещественных умножения" << std::endl;
   std::cout << "  " << real_adds_per_complex_mult << " вещественных сложения/вычитания" << std::endl;
   std::cout << "сложение: (a+bi)+(c+di) = (a+c) + (b+d)i" << std::endl;

   int real_adds_per_complex_add = 2;
   std::cout << "  " << real_adds_per_complex_add << " вещественных сложения" << std::endl;

   int n = p.degree();
   int complex_mults = n;
   int complex_adds = n;
   int real_mults = complex_mults * real_mults_per_complex_mult;
   int real_adds = complex_mults * real_adds_per_complex_mult + complex_adds * real_adds_per_complex_add;

   std::cout << "\nдля полинома степени " << n << ":" << std::endl;
   std::cout << "комплексных умножений: " << complex_mults << std::endl;
   std::cout << "комплексных сложений: " << complex_adds << std::endl;
   std::cout << "вещественных операций: " << real_mults << " умн. + " << real_adds << " слож. = " << (real_mults + real_adds) << std::endl;
   std::cout << std::endl;
}

void task_8()
{
   std::cout << "8) вычисление u_n*x^n + ... + u_1*x + u_0" << std::endl;

   std::vector<double> u_coeffs = {1, 5, 2, 3};
   int n = u_coeffs.size() - 1;
   double x_val = 7.0;

   polynomial p(u_coeffs);
   std::cout << "полином: ";
   horner_methods::print_polynomial(p);

   std::cout << "\nметод 1: стандартный горнер" << std::endl;

   double result_horner = u_coeffs[n];
   int horner_mults = 0;
   int horner_adds = 0;

   for (int i = n - 1; i >= 0; i--)
   {
      result_horner = result_horner * x_val + u_coeffs[i];
      horner_mults++;
      horner_adds++;
   }

   std::cout << "операций: " << horner_mults << " умножений + " << horner_adds << " сложений" << std::endl;
   std::cout << "p(" << x_val << ") = " << result_horner << std::endl;

   std::cout << "\nметод 2: через факториальные степени" << std::endl;
   std::cout << "k!(x/k) = x(x-1)...(x-k+1)" << std::endl;

   for (int k = 0; k <= n; k++)
   {
      std::cout << "  " << k << "!(x/" << k << ") = ";
      if (k == 0)
      {
         std::cout << "1";
      }
      else
      {
         std::cout << "x";
         for (int j = 1; j < k; j++)
         {
            std::cout << "(x-" << j << ")";
         }
      }
      std::cout << std::endl;
   }

   int factorial_mults = n;
   int factorial_subtracts = (n - 1);
   int factorial_products = n;
   int factorial_adds = n;

   int total_mults = factorial_mults + factorial_products;
   int total_adds = factorial_subtracts + factorial_adds;

   std::cout << "\nдля коэффициентов в факториальном базисе:" << std::endl;
   std::cout << "вычисление факториальных членов: " << factorial_mults << " умножений + " << factorial_subtracts << " вычитаний" << std::endl;
   std::cout << "умножение на коэффициенты: " << factorial_products << " умножений" << std::endl;
   std::cout << "суммирование: " << factorial_adds << " сложений" << std::endl;
   std::cout << "итого: " << total_mults << " умножений + " << total_adds << " сложений" << std::endl;

   std::cout << "\nпример последовательных вычислений p(x), p(x+1), ...:" << std::endl;
   std::cout << "   x     p(x)" << std::endl;

   int num_points = 7;
   for (int k = 0; k < num_points; k++)
   {
      double x = x_val + k;
      double val = u_coeffs[n];
      for (int i = n - 1; i >= 0; i--)
      {
         val = val * x + u_coeffs[i];
      }
      std::cout << std::setw(4) << (int)x << "  " << std::setw(8) << val << std::endl;
   }

   std::cout << "\nобычный горнер эффективнее (" << horner_mults << " умн. vs " << total_mults << " умн.)" << std::endl;
   std::cout << std::endl;
}

int main()
{
   std::cout << std::fixed << std::setprecision(4);

   std::cout << "ne budu eto kommentirovat\n"
             << std::endl;

   task_1();
   task_2();
   task_3();
   task_4();
   task_8();

   return 0;
}