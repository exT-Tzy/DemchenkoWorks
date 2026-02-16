#include "polynomial.h"
#include "polynomial_operations.h"
#include <iostream>
#include <iomanip>

void test_task_1()
{
   std::cout << "1) проверка принадлежности линейной оболочке" << std::endl;

   polynomial f({1, 2, 1});

   std::vector<polynomial> generators = {
       polynomial({1, 1}),
       polynomial({1, 0, 1})};

   std::cout << "многочлен f(x): ";
   polynomial_operations::print_polynomial(f);

   std::cout << "\nпорождающие многочлены:" << std::endl;
   for (size_t i = 0; i < generators.size(); i++)
   {
      std::cout << "g" << (i + 1) << "(x): ";
      polynomial_operations::print_polynomial(generators[i]);
   }

   bool belongs = polynomial_operations::check_linear_span(f, generators);

   std::cout << "\nрезультат: ";
   if (belongs)
   {
      std::cout << "f(x) принадлежит линейной оболочке" << std::endl;

      std::vector<double> coeffs = polynomial_operations::find_linear_combination(f, generators);

      std::cout << "\nпредставление: f(x) = ";
      for (size_t i = 0; i < coeffs.size(); i++)
      {
         if (i > 0 && coeffs[i] >= 0)
            std::cout << " + ";
         std::cout << coeffs[i] << "*g" << (i + 1) << "(x)";
      }
      std::cout << std::endl;

      polynomial check(0.0);
      for (size_t i = 0; i < coeffs.size(); i++)
      {
         check = check + generators[i] * coeffs[i];
      }
      std::cout << "\nпроверка: ";
      polynomial_operations::print_polynomial(check);
   }
   else
   {
      std::cout << "f(x) НЕ принадлежит линейной оболочке" << std::endl;
   }

   polynomial f2({4, 2, 2});

   std::cout << "\n\nмногочлен f(x): ";
   polynomial_operations::print_polynomial(f2);

   std::cout << "\nпорождающие многочлены:" << std::endl;
   for (size_t i = 0; i < generators.size(); i++)
   {
      std::cout << "g" << (i + 1) << "(x): ";
      polynomial_operations::print_polynomial(generators[i]);
   }

   bool belongs2 = polynomial_operations::check_linear_span(f2, generators);

   std::cout << "\nрезультат: ";
   if (belongs2)
   {
      std::cout << "f(x) принадлежит линейной оболочке" << std::endl;

      std::vector<double> coeffs = polynomial_operations::find_linear_combination(f2, generators);

      std::cout << "\nпредставление: f(x) = ";
      for (size_t i = 0; i < coeffs.size(); i++)
      {
         if (i > 0 && coeffs[i] >= 0)
            std::cout << " + ";
         std::cout << coeffs[i] << "*g" << (i + 1) << "(x)";
      }
      std::cout << std::endl;

      polynomial check(0.0);
      for (size_t i = 0; i < coeffs.size(); i++)
      {
         check = check + generators[i] * coeffs[i];
      }
      std::cout << "\nпроверка: ";
      polynomial_operations::print_polynomial(check);
   }
   else
   {
      std::cout << "f(x) НЕ принадлежит линейной оболочке" << std::endl;
   }

   std::cout << std::endl;
}

void test_task_2()
{
   std::cout << "2) представление в виде степеней (x-a)^k" << std::endl;

   polynomial f({6, 11, 6, 1});
   double a = 2.0;

   std::cout << "исходный многочлен f(x): ";
   polynomial_operations::print_polynomial(f);
   std::cout << "точка a = " << a << std::endl;

   polynomial result = polynomial_operations::represent_as_powers(f, a);

   std::cout << "\nпредставление в виде f(x) = c0 + c1*(x-" << a << ") + c2*(x-" << a << ")^2 + ..." << std::endl;
   std::cout << "коэффициенты: ";
   for (int i = 0; i <= result.degree(); i++)
   {
      if (i > 0)
         std::cout << ", ";
      std::cout << "c" << i << " = " << result.get_coeff(i);
   }
   std::cout << std::endl;

   std::cout << "\nпроверка в нескольких точках:" << std::endl;
   for (double x = 0; x <= 4; x += 1.0)
   {
      double original = f(x);
      double from_powers = result(x - a);
      std::cout << "x = " << x << ": f(x) = " << original << ", через степени = " << from_powers << std::endl;
   }
   std::cout << std::endl;
}

void test_task_3()
{
   std::cout << "3) замена базиса с (x-a) на (x-B)" << std::endl;

   std::vector<double> coeffs_in_xa_basis = {1, 2, 3, 1};
   double a = 1.0;
   double b = 2.0;

   std::cout << "коэффициенты в базисе (x-" << a << "): [";
   for (size_t i = 0; i < coeffs_in_xa_basis.size(); i++)
   {
      if (i > 0)
         std::cout << ", ";
      std::cout << coeffs_in_xa_basis[i];
   }
   std::cout << "]" << std::endl;
   std::cout << "т.е. f(x) = 1 + 2*(x-" << a << ") + 3*(x-" << a << ")^2 + 1*(x-" << a << ")^3" << std::endl;

   polynomial f_in_xa(coeffs_in_xa_basis);

   polynomial f_standard(0.0);
   polynomial xa_power(1.0);
   polynomial x_minus_a({-a, 1.0});

   for (size_t i = 0; i < coeffs_in_xa_basis.size(); i++)
   {
      f_standard = f_standard + xa_power * coeffs_in_xa_basis[i];
      xa_power = xa_power * x_minus_a;
   }

   std::cout << "\nв стандартном виде: ";
   polynomial_operations::print_polynomial(f_standard);

   polynomial result = polynomial_operations::represent_as_powers(f_standard, b);

   std::cout << "\nкоэффициенты в базисе (x-" << b << "): [";
   for (int i = 0; i <= result.degree(); i++)
   {
      if (i > 0)
         std::cout << ", ";
      std::cout << result.get_coeff(i);
   }
   std::cout << "]" << std::endl;

   std::cout << "\nпроверка - значения многочлена в точках:" << std::endl;
   for (double x = 0; x <= 4; x += 1.0)
   {
      double val_standard = f_standard(x);

      double val_from_xa = 0;
      double xa_pow = 1.0;
      for (size_t i = 0; i < coeffs_in_xa_basis.size(); i++)
      {
         val_from_xa += coeffs_in_xa_basis[i] * xa_pow;
         xa_pow *= (x - a);
      }

      double val_from_xb = 0;
      double xb_pow = 1.0;
      for (int i = 0; i <= result.degree(); i++)
      {
         val_from_xb += result.get_coeff(i) * xb_pow;
         xb_pow *= (x - b);
      }

      std::cout << "x = " << x << ": стандарт = " << val_standard << ", из (x-a) = " << val_from_xa << ", из (x-b) = " << val_from_xb << std::endl;
   }
   std::cout << std::endl;
}

void test_task_4()
{
   std::cout << "4) пределы рациональной функции" << std::endl;

   polynomial num({-6, 1, 1});
   polynomial den({-4, 0, 1});

   std::cout << "числитель: ";
   polynomial_operations::print_polynomial(num, "f(x)");
   std::cout << "знаменатель: ";
   polynomial_operations::print_polynomial(den, "g(x)");

   double a = 2.0;
   double limit_a = polynomial_operations::limit_at_point(num, den, a);
   std::cout << "\nlim(x→" << a << ") f(x)/g(x) = " << limit_a << std::endl;

   double limit_inf = polynomial_operations::limit_at_infinity(num, den);
   std::cout << "lim(x→∞) f(x)/g(x) = " << limit_inf << std::endl;

   std::cout << "\nпримеры вычислений в точках:" << std::endl;
   for (double x = -3; x <= 5; x += 1.0)
   {
      if (std::abs(den(x)) > 1e-6)
      {
         std::cout << "x = " << x << ": R(x) = " << (num(x) / den(x)) << std::endl;
      }
   }
   std::cout << std::endl;
}

void test_task_5()
{
   std::cout << "5) пределы композиции рациональных функций" << std::endl;

   polynomial f1({1, 1});
   polynomial s1({1, 0, 1});
   polynomial f2({1, 1});
   polynomial s2({1, 1});

   std::cout << "числитель внешней функции: ";
   polynomial_operations::print_polynomial(f1, "f1(x)");
   std::cout << "знаменатель внешней функции: ";
   polynomial_operations::print_polynomial(s1, "s1(x)");
   std::cout << "числитель внутренней функции: ";
   polynomial_operations::print_polynomial(f2, "f2(x)");
   std::cout << "знаменатель внутренней функции: ";
   polynomial_operations::print_polynomial(s2, "s2(x)");

   int k = 1;
   int l = 1;
   double a = 0.0;

   std::cout << "\nпараметры: k = " << k << ", l = " << l << std::endl;

   double limit_a = polynomial_operations::composite_limit_at_point(f1, s1, f2, s2, a, k, l);
   std::cout << "\nlim(x→" << a << ") T(x) = " << limit_a << std::endl;

   double limit_inf = polynomial_operations::composite_limit_at_infinity(f1, s1, f2, s2, k, l);
   std::cout << "lim(x→∞) T(x) = " << limit_inf << std::endl;

   std::cout << std::endl;
}

int main()
{
   std::cout << std::fixed << std::setprecision(4);

   std::cout << "\n";
   std::cout << "многочлены лол гыгыгыг" << std::endl;
   std::cout << "\n";

   test_task_1();
   test_task_2();
   test_task_3();
   test_task_4();
   test_task_5();

   return 0;
}