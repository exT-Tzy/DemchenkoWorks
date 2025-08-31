#ifndef VECTOR_H
#define VECTOR_H

#include "bolshaya_drob.h"
#include <vector>
#include <stdexcept>
#include <sstream>

class Vector final
{
private:
    std::vector<bolshaya_drob> _components;
    size_t _dimension;

public:
    Vector() : _dimension(0) {}

    Vector(size_t dimension) : _dimension(dimension), _components(dimension, bolshaya_drob(0)) {}

    Vector(const std::vector<bolshaya_drob>& components)
        : _components(components), _dimension(components.size()) {}

    Vector(std::initializer_list<bolshaya_drob> components)
        : _components(components), _dimension(components.size()) {}

    size_t dimension() const { return _dimension; }

    bolshaya_drob& operator[](size_t index)
    {
        if (index >= _dimension)
            throw std::out_of_range("Vector index out of range");
        
        return _components[index];
    }

    const bolshaya_drob& operator[](size_t index) const
    {
        if (index >= _dimension)
            throw std::out_of_range("Vector index out of range");
      
        return _components[index];
    }

    Vector operator+(const Vector& other) const
    {
        if (_dimension != other._dimension)
            throw std::invalid_argument("Vector dimensions must match for addition");

        Vector result(_dimension);
        for (size_t i = 0; i < _dimension; ++i) 
        {
            result[i] = _components[i] + other._components[i];
        }
      
        return result;
    }

    Vector operator-(
        const Vector& other) const
    {
        if (_dimension != other._dimension)
            throw std::invalid_argument("Vector dimensions must match for subtraction");

        Vector result(_dimension);
        
        for (size_t i = 0; i < _dimension; ++i)
        {
            result[i] = _components[i] - other._components[i];
        }
       
        return result;
    }

    Vector operator*(const bolshaya_drob& scalar) const
    {
        Vector result(_dimension);
       
        for (size_t i = 0; i < _dimension; ++i) 
        {
            result[i] = _components[i] * scalar;
        }
      
        return result;
    }

    Vector operator-() const
    {
        Vector result(_dimension);
        
        for (size_t i = 0; i < _dimension; ++i)
        {
            result[i] = -_components[i];
        }
        
        return result;
    }

    bolshaya_drob dot(
        const Vector& other) const
    {
        if (_dimension != other._dimension)
            throw std::invalid_argument("Vector dimensions must match for dot product");

        bolshaya_drob result(0);
       
        for (size_t i = 0; i < _dimension; ++i)
        {
            result += _components[i] * other._components[i];
        }
      
        return result;
    }

    bolshaya_drob magnitude() const
    {
        bolshaya_drob sum_of_squares(0);
        
        for (size_t i = 0; i < _dimension; ++i)
        {
            sum_of_squares += _components[i] * _components[i];
        }
       
        return sqrt(sum_of_squares, bolshaya_drob::DEFAULT_EPS);
    }

    Vector cross_3d(const Vector& other) const
    {
        if (_dimension != 3 || other._dimension != 3)
            throw std::invalid_argument("Cross product requires 3D vectors");

        Vector result(3);
        result[0] = _components[1] * other._components[2] - _components[2] * other._components[1];
        result[1] = _components[2] * other._components[0] - _components[0] * other._components[2];
        result[2] = _components[0] * other._components[1] - _components[1] * other._components[0];
       
        return result;
    }

    Vector cross_7d(const Vector& other) const
    {
        if (_dimension != 7 || other._dimension != 7)
            throw std::invalid_argument("7D cross product requires 7D vectors");

        Vector result(7);

        result[0] = _components[1] * other._components[2] - _components[2] * other._components[1] +
            _components[3] * other._components[4] - _components[4] * other._components[3] +
            _components[5] * other._components[6] - _components[6] * other._components[5];

        result[1] = _components[2] * other._components[0] - _components[0] * other._components[2] +
            _components[4] * other._components[5] - _components[5] * other._components[4] +
            _components[6] * other._components[3] - _components[3] * other._components[6];

        result[2] = _components[0] * other._components[1] - _components[1] * other._components[0] +
            _components[5] * other._components[3] - _components[3] * other._components[5] +
            _components[4] * other._components[6] - _components[6] * other._components[4];

        result[3] = _components[1] * other._components[5] - _components[5] * other._components[1] +
            _components[6] * other._components[0] - _components[0] * other._components[6] +
            _components[2] * other._components[4] - _components[4] * other._components[2];

        result[4] = _components[2] * other._components[3] - _components[3] * other._components[2] +
            _components[0] * other._components[5] - _components[5] * other._components[0] +
            _components[6] * other._components[1] - _components[1] * other._components[6];

        result[5] = _components[3] * other._components[1] - _components[1] * other._components[3] +
            _components[6] * other._components[2] - _components[2] * other._components[6] +
            _components[0] * other._components[4] - _components[4] * other._components[0];

        result[6] = _components[4] * other._components[0] - _components[0] * other._components[4] +
            _components[1] * other._components[3] - _components[3] * other._components[1] +
            _components[5] * other._components[2] - _components[2] * other._components[5];

        return result;
    }

    bolshaya_drob mixed_triple_3d(
        const Vector& b, 
        const Vector& c) const
    {
        if (_dimension != 3 || b._dimension != 3 || c._dimension != 3)
            throw std::invalid_argument("Mixed triple product requires 3D vectors");

        return this->dot(b.cross_3d(c));
    }

    bolshaya_drob mixed_triple_7d(const Vector& b, const Vector& c) const
    {
        if (_dimension != 7 || b._dimension != 7 || c._dimension != 7)
            throw std::invalid_argument("7D mixed triple product requires 7D vectors");

        return this->dot(b.cross_7d(c));
    }

    Vector normalize() const
    {
        bolshaya_drob mag = magnitude();
        if (mag == bolshaya_drob(0))
            throw std::invalid_argument("Cannot normalize zero vector");

        Vector result(_dimension);
        for (size_t i = 0; i < _dimension; ++i) 
        {
            result[i] = _components[i] / mag;
        }
       
        return result;
    }

    Vector project_onto(
        const Vector& other) const
    {
        if (_dimension != other._dimension)
            throw std::invalid_argument("Vector dimensions must match for projection");

        bolshaya_drob dot_product = this->dot(other);
        bolshaya_drob other_magnitude_squared = other.dot(other);

        if (other_magnitude_squared == bolshaya_drob(0))
            throw std::invalid_argument("Cannot project onto zero vector");

        return other * (dot_product / other_magnitude_squared);
    }

    bool is_zero() const
    {
        for (size_t i = 0; i < _dimension; ++i) 
        {
            if (_components[i] != bolshaya_drob(0))
                return false;
        }
      
        return true;
    }

    std::string to_latex() const
    {
        std::string result = "\\begin{pmatrix}";
        
        for (size_t i = 0; i < _dimension; ++i)
        {
            if (i > 0) result += " \\\\ ";
            result += _components[i].to_decimal(6);
        }
        result += "\\end{pmatrix}";
       
        return result;
    }

    std::string to_latex_row() const
    {
        std::string result = "(";
        
        for (size_t i = 0; i < _dimension; ++i) 
        {
            if (i > 0) result += ", ";
            result += _components[i].to_decimal(6);
        }
        result += ")";
       
        return result;
    }
};

std::vector<Vector> gram_schmidt_orthogonalization(
    const std::vector<Vector>& vectors)
{
    if (vectors.empty())
        return {};

    std::vector<Vector> orthogonal_vectors;

    for (size_t i = 0; i < vectors.size(); ++i) 
    {
        Vector current = vectors[i];

        for (size_t j = 0; j < orthogonal_vectors.size(); ++j) 
        {
            Vector projection = current.project_onto(orthogonal_vectors[j]);
            current = current - projection;
        }

        if (!current.is_zero())
        {
            orthogonal_vectors.push_back(current);
        }
    }

    return orthogonal_vectors;
}

Vector operator*(
    const bolshaya_drob& scalar,
    const Vector& vector)
{
    return vector * scalar;
}

#endif // VECTOR_H