
#include "tmatrix.h"
#include <cmath>

namespace transformations3D
{

    // Class Tmatrix
    // Implementación de la función apply
    Vectors::vector3d Tmatrix::apply(const Vectors::vector3d& vec) const
    {
        // Si los ángulos son 0, la rotación es la identidad
        if (alpha_x == 0 && theta_y == 0 && phi_z == 0)
        {
            // Solo aplicar la traslación
            Vectors::vector3d result;
            result.x = t_x + vec.x;
            result.y = t_y + vec.y;
            result.z = t_z + vec.z;
            
            return result;
        } 

        // Precalcular senos y cosenos
        double Cthe = std::cos(theta_y), Sthe = std::sin(theta_y);
        double Calf = std::cos(alpha_x), Salf = std::sin(alpha_x);
        double Cphi = std::cos(phi_z), Sphi = std::sin(phi_z);

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
}