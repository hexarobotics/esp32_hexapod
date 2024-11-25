#include "Vectors.h"
#include <iostream> // Para std::cout
#include <cmath>    // Para sqrt

namespace transformations3D
{
    // Constructor por defecto que asigna todo a 0
    Vectors::vector3d::vector3d() : x(0), y(0), z(0)
    {
    }

    // Constructor que inicializa con valores específicos
    Vectors::vector3d::vector3d(double x_val, double y_val, double z_val) : x(x_val), y(y_val), z(z_val)
    {
    }

    // === Implementaciones de vector2d ===

    // Constructor por defecto que asigna todo a 0
    Vectors::vector2d::vector2d() : x(0), y(0)
    {
    }

    // Constructor que inicializa con valores específicos
    Vectors::vector2d::vector2d(double x_val, double y_val) : x(x_val), y(y_val)
    {
    }

    // Suma de vectores
    Vectors::vector3d Vectors::add(const vector3d& v1, const vector3d& v2)
    {
        return vector3d(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
    }

    // Resta de vectores
    Vectors::vector3d Vectors::subtract(const vector3d& v1, const vector3d& v2)
    {
        return vector3d(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
    }

    // Producto punto (dot product)
    double Vectors::dot(const vector3d& v1, const vector3d& v2)
    {
        return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
    }

    // Producto cruz (cross product)
    Vectors::vector3d Vectors::cross(const vector3d& v1, const vector3d& v2)
    {
        return vector3d(
            v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x
        );
    }

    // Magnitud del vector
    double Vectors::magnitude(const vector3d& v)
    {
        return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    }

    // Normalización del vector (vector unitario)
    Vectors::vector3d Vectors::normalize(const vector3d& v)
    {
        double mag = magnitude(v);
        if (mag == 0) return vector3d(0, 0, 0);  // Evitar división por cero
        return vector3d(v.x / mag, v.y / mag, v.z / mag);
    }

    // Escalar un vector por un escalar
    Vectors::vector3d Vectors::scale(const vector3d& v, double scalar)
    {
        return vector3d(v.x * scalar, v.y * scalar, v.z * scalar);
    }

    // Imprimir un vector (para depuración)
    void Vectors::print(const vector3d& v)
    {
        std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
    }
}