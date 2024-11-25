
#include "tmatrix.h"
#include <cmath>

namespace transformations3D
{

    // Class Tmatrix
    // Implementación de la función apply
    Vectors::vector3d Tmatrix::apply(const Vectors::vector3d& vec) const
    {
        // Si los ángulos son 0, la rotación es la identidad
        constexpr double MARGEN = 1e-6;
        if (std::abs(rot_x) < MARGEN && std::abs(rot_y) < MARGEN && std::abs(rot_z) < MARGEN)
        {
            // Solo aplicar la traslación
            Vectors::vector3d result;
            result.x = t_x + vec.x;
            result.y = t_y + vec.y;
            result.z = t_z + vec.z;
            
            return result;
        } 

        // Precalcular senos y cosenos
        double Cthe = std::cos(rot_z), Sthe = std::sin(rot_z); // Yaw (θ)
        double Calf = std::cos(rot_x), Salf = std::sin(rot_x); // Roll (α)
        double Cphi = std::cos(rot_y), Sphi = std::sin(rot_y); // Pitch (φ)

        // Matriz de rotación
        Vectors::vector3d rotated;
        rotated.x = Cthe * Cphi * vec.x + (-Calf * Sthe + Cthe * Salf * Sphi) * vec.y + (Salf * Sthe + Calf * Cthe * Sphi) * vec.z;
        rotated.y = Cphi * Sthe * vec.x + (Calf * Cthe + Salf * Sthe * Sphi) * vec.y + (-Cthe * Salf + Calf * Sthe * Sphi) * vec.z;
        rotated.z = -Sphi * vec.x + Cthe * Salf * vec.y + Calf * Cphi * vec.z;

        // Traslación
        Vectors::vector3d result;
        result.x = t_x + rotated.x;
        result.y = t_y + rotated.y;
        result.z = t_z + rotated.z;

        return result;
    }

    void Tmatrix::reset()
    {
        t_x = t_y = t_z = 0.0f;
        rot_x = rot_y = rot_z = 0.0f;
    }
}