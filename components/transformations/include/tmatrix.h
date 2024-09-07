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
          Tmatrix() : translationX(0), translationY(0), translationZ(0),
                      alpha_x(0), theta_y(0), phi_z(0) {}

          // Aplica la transformación al vector dado
          transformations3D::Vectors::vector3d apply(const transformations3D::Vectors::vector3d& vec) const;

          // Setters para las traslaciones
          void setTranslationX(int16_t tx) { translationX = tx; }
          void setTranslationY(int16_t ty) { translationY = ty; }
          void setTranslationZ(int16_t tz) { translationZ = tz; }

          // Setters para los ángulos de rotación
          void setAlpha(double a) { alpha_x = a; }
          void setTheta(double t) { theta_y = t; }
          void setPhi(double p) { phi_z = p; }

          // Getters para las traslaciones
          double getTranslationX() const { return translationX; }
          double getTranslationY() const { return translationY; }
          double getTranslationZ() const { return translationZ; }

          // Getters para los ángulos de rotación
          double getAlpha() const { return alpha_x; }
          double getTheta() const { return theta_y; }
          double getPhi() const { return phi_z; }

      private:
          // Traslaciones
          int16_t translationX; // mm
          int16_t translationY;
          int16_t translationZ;

          // Ángulos de rotación en radianes
          double alpha_x; // Rotación alrededor del eje X - body roll  (rad)
          double theta_y; // Rotación alrededor del eje Y - body pitch (rad)
          double phi_z;   // Rotación alrededor del eje Z - body yaw   (rad)
  };
};

#endif