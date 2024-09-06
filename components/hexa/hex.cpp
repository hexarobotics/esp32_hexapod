#include <hex.h>
#include <cmath>


uint8_t control_cuerpo;
uint8_t control_robot;

namespace hexapod
{
    // Class HexIk
    // Constructor HexIk
    HexIk::HexIk()
    {
        initializeLegEndpoints();
    }

    void HexIk::initializeLegEndpoints()
    {
        for(int i = 0; i < NUM_LEGS; ++i)
        {
            leg_endpoints[i] = {0.0, 0.0, 0.0};  // Inicializa con {x, y, z}
        }
    }

    // Class Tmatrix
    // Implementación de la función apply
    Tmatrix::Vector3 Tmatrix::apply(const Vector3& vec) const
    {
        // Si los ángulos son 0, la rotación es la identidad
        if (alpha == 0 && theta == 0 && phi == 0)
        {
            // Solo aplicar la traslación
            Vector3 result;
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
        Vector3 rotated;
        rotated.x = Cthe * Cphi * vec.x + (-Calf * Sthe + Cthe * Salf * Sphi) * vec.y + (Salf * Sthe + Calf * Cthe * Sphi) * vec.z;
        rotated.y = Cphi * Sthe * vec.x + (Calf * Cthe + Salf * Sthe * Sphi) * vec.y + (-Cthe * Salf + Calf * Sthe * Sphi) * vec.z;
        rotated.z = -Sphi * vec.x + Cthe * Salf * vec.y + Calf * Cphi * vec.z;

        // Traslación
        Vector3 result;
        result.x = translationX + rotated.x;
        result.y = translationY + rotated.y;
        result.z = translationZ + rotated.z;

        return result;
    }
} // namespace hexapod


/// que he hecho
/// TODO: separar las clases en archivos distintos





void get_ik_initial_parameters()
{
    if ( control_cuerpo == 1 )
    {

    }
    else if ( control_robot == 1 )
    {

    }
}
