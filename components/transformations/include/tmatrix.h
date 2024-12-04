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
          Tmatrix() : t_x(0.0f), t_y(0.0f), t_z(0.0f),
                      rot_x(0.0f), rot_y(0.0f), rot_z(0.0f)
          {
          }

          // Aplica la transformación al vector dado
          transformations3D::Vectors::vector3d apply(const transformations3D::Vectors::vector3d& vec) const;

          // Método para resetear todos los valores a 0
          void reset();
          
          // Setters para las traslaciones
          void setTranslationX(float tx) { t_x = tx; }
          void setTranslationY(float ty) { t_y = ty; }
          void setTranslationZ(float tz) { t_z = tz; }

          // Setters para los ángulos de rotación
          void setAlpha(double a) { rot_x = a; }
          void setTheta(double t) { rot_y = t; }
          void setPhi(double p) { rot_z = p; }

          // Getters para las traslaciones
          double getTranslationX() const { return t_x; }
          double getTranslationY() const { return t_y; }
          double getTranslationZ() const { return t_z; }

          // Getters para los ángulos de rotación
          double getAlpha() const { return rot_x; }
          double getTheta() const { return rot_y; }
          double getPhi() const { return rot_z; }

          // Traslaciones
          double t_x; // mm
          double t_y;
          double t_z;

          // Ángulos de rotación en radianes
          double rot_x; // Rotación alrededor del eje X - body roll  (rad)
          double rot_y; // Rotación alrededor del eje Y - body pitch (rad)
          double rot_z;   // Rotación alrededor del eje Z - body yaw   (rad)

          private:
  };
};

#endif