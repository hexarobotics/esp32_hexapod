
#include "tmatrix.h"
#include <cmath>

namespace transformations3D
{

    // Class Tmatrix
    // Implementación de la función apply
    Vectors::vector3d Tmatrix::apply(const Vectors::vector3d& vec) const
    {
        // Si los ángulos son 0, la rotación es la identidad
        if (alpha == 0 && theta == 0 && phi == 0)
        {
            // Solo aplicar la traslación
            Vectors::vector3d result;
            result.x = translationX + vec.x;
            result.y = translationY + vec.y;
            result.z = translationZ + vec.z;
            
            return result;
        } 

        // Precalcular senos y cosenos
        double Cthe = std::cos(theta), Sthe = std::sin(theta);
        double Calf = std::cos(alpha), Salf = std::sin(alpha);
        double Cphi = std::cos(phi), Sphi = std::sin(phi);

        // Matriz de rotación
        Vectors::vector3d rotated;
        rotated.x = Cthe * Cphi * vec.x + (-Calf * Sthe + Cthe * Salf * Sphi) * vec.y + (Salf * Sthe + Calf * Cthe * Sphi) * vec.z;
        rotated.y = Cphi * Sthe * vec.x + (Calf * Cthe + Salf * Sthe * Sphi) * vec.y + (-Cthe * Salf + Calf * Sthe * Sphi) * vec.z;
        rotated.z = -Sphi * vec.x + Cthe * Salf * vec.y + Calf * Cphi * vec.z;

        // Traslación
        Vectors::vector3d result;
        result.x = translationX + rotated.x;
        result.y = translationY + rotated.y;
        result.z = translationZ + rotated.z;

        return result;
    }
}