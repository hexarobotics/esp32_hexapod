#ifndef __VECTORS_H__
	#define __VECTORS_H__

namespace transformations3D
{
    class Vectors
    {
    public:
        struct vector3d
        {
            double x, y, z;

            // Constructor por defecto que asigna todo a 0
            vector3d();
            // Constructor que inicializa con valores específicos
            vector3d(double x_val, double y_val, double z_val);
        };

        // Declaraciones de funciones
        static vector3d add(const vector3d& v1, const vector3d& v2);
        static vector3d subtract(const vector3d& v1, const vector3d& v2);
        static double dot(const vector3d& v1, const vector3d& v2);
        static vector3d cross(const vector3d& v1, const vector3d& v2);
        static double magnitude(const vector3d& v);
        static vector3d normalize(const vector3d& v);
        static vector3d scale(const vector3d& v, double scalar);
        static void print(const vector3d& v);
    };
}

#endif // __VECTORS_H__