#ifndef __T_MATRIX_H__
  #define __T_MATRIX_H__

#include "vectors.h"
#include <cstdint>

namespace transformations3D
{
  class Tmatrix
  {
    public:
          // Constructor por defecto
          Tmatrix() : t_x(0), t_y(0), t_z(0),
                      alpha_x(0), theta_y(0), phi_z(0) {}

          // Aplica la transformación al vector dado
          transformations3D::Vectors::vector3d apply(const transformations3D::Vectors::vector3d& vec) const;

          // Setters para las traslaciones
          void setTranslationX(int16_t tx) { t_x = tx; }
          void setTranslationY(int16_t ty) { t_y = ty; }
          void setTranslationZ(int16_t tz) { t_z = tz; }

          // Setters para los ángulos de rotación
          void setAlpha(double a) { alpha_x = a; }
          void setTheta(double t) { theta_y = t; }
          void setPhi(double p) { phi_z = p; }

          // Getters para las traslaciones
          double getTranslationX() const { return t_x; }
          double getTranslationY() const { return t_y; }
          double getTranslationZ() const { return t_z; }

          // Getters para los ángulos de rotación
          double getAlpha() const { return alpha_x; }
          double getTheta() const { return theta_y; }
          double getPhi() const { return phi_z; }

          // Traslaciones
          int16_t t_x; // mm
          int16_t t_y;
          int16_t t_z;

          // Ángulos de rotación en radianes
          double alpha_x; // Rotación alrededor del eje X - body roll  (rad)
          double theta_y; // Rotación alrededor del eje Y - body pitch (rad)
          double phi_z;   // Rotación alrededor del eje Z - body yaw   (rad)

          private:
  };
};

#endif