#ifndef __HEX_IK_H__
  #define __HEX_IK_H__

#include <stdint.h>

//	###			KINEMATIC VARIABLES		###		//
//	********************************		//

/* Body
* We assume 4 legs are on the corners of a box defined by X_COXA x Y_COXA
* Middle legs for a hexapod can be different Y, but should be halfway in X
*/
#define X_COXA  60  // MM between front and back legs /2
#define Y_COXA  60  // MM between front/back legs /2
#define MX_COXA  100  // MM between two middle legs /2

/* Legs */
#define L_COXA      48  // MM distance from coxa servo to femur servo
#define L_FEMUR     75 // MM distance from femur servo to tibia servo
#define L_TIBIA     135 // MM distance from tibia servo to foot

/**
 * @brief Hexapod kinematics library
 * 
 */
namespace hexapod
{
    class Vector
    {
        public:
            struct Vector3
            {
                double x, y, z;
            };
    };

    class HexIk : public Vector
    {
        public:
            static const uint8_t NUM_LEGS = 6;

            HexIk();

        private:
            Vector3 leg_endpoints[NUM_LEGS];
            void initializeLegEndpoints();
    };

    class Tmatrix : public Vector
    {
    	public:
            // Constructor por defecto
            Tmatrix() : translationX(0), translationY(0), translationZ(0),
                        alpha(0), theta(0), phi(0) {}

            // Aplica la transformación al vector dado
            Vector3 apply(const Vector3& vec) const;

            // Setters para las traslaciones
            void setTranslationX(int16_t tx) { translationX = tx; }
            void setTranslationY(int16_t ty) { translationY = ty; }
            void setTranslationZ(int16_t tz) { translationZ = tz; }

            // Setters para los ángulos de rotación
            void setAlpha(double a) { alpha = a; }
            void setTheta(double t) { theta = t; }
            void setPhi(double p) { phi = p; }

            // Getters para las traslaciones
            double getTranslationX() const { return translationX; }
            double getTranslationY() const { return translationY; }
            double getTranslationZ() const { return translationZ; }

            // Getters para los ángulos de rotación
            double getAlpha() const { return alpha; }
            double getTheta() const { return theta; }
            double getPhi() const { return phi; }

        private:
            // Traslaciones
            int16_t translationX; // mm
            int16_t translationY;
            int16_t translationZ;

            // Ángulos de rotación en radianes
            double alpha; // Rotación alrededor del eje X - body roll  (rad)
            double theta; // Rotación alrededor del eje Y - body pitch (rad)
            double phi;   // Rotación alrededor del eje Z - body yaw   (rad)
    };
};


#endif
