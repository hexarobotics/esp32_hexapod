#include <Hexaik.h>

uint8_t control_cuerpo;
uint8_t control_robot;

namespace hexapod
{
    // Class Hexaik
    // Constructor Hexaik
    Hexaik::Hexaik()
    {
	    // Inicializa todos los elementos del arreglo a {0.0, 0.0, 0.0}
	    for (int i = 0; i < num_legs; ++i)
	    {
	        leg_endpoints[i] = Vectors::vector3d(); // Usa el constructor por defecto que inicializa a 0
	    }
    }

    /* Simple 3dof leg solver. X,Y,Z are the length from the Coxa rotate to the endpoint. */
    Hexaik::ik_sol_t Hexaik::legIK(int X, int Y, int Z, int leg)
    {
        ik_sol_t ans; // angulo en servo pwm.

        // primero, resolver el angulo coxa para conseguir que nuestro problema se reduzca a un problema 2D
        ans.coxa = radians_to_degrees(atan2(Y,X)); //pata ya es una direccion de memoria

        long trueX = sqrt(sq((long)X)+sq((long)Y)) - L_COXA;
        long im = sqrt(sq((long)trueX)+sq((long)Z));    // length of imaginary leg
        // get femur angle above horizon...
        float q1  = atan2(Z,trueX);// Mientras que Z sea negativo, el angulo saldra negativo, cuando la pata sobrepase el eje horizontal, el angulo sera positivo
        long  n1  = sq(L_TIBIA)-sq(L_FEMUR)-sq(im);//numerador
        long  d2  = -2*L_FEMUR*im;//denominador
        float q2  = acos((float)n1/(float)d2);

        ans.femur = radians_to_degrees((q2+q1)); // lo pongo positivo pero ahora la z es negativa ya que el centro esta en coxa y endpoint esta por debajo
        //float intermedio=q2-q1;

        // and tibia angle from femur...
        n1 = sq(im)-sq(L_FEMUR)-sq(L_TIBIA);
        d2 = -2*L_FEMUR*L_TIBIA;
        //alfa=n1/d2 //teta3=alfa-90º
        ans.tibia = radians_to_degrees((acos((float)n1/(float)d2))-1.57);

        //  ##### adaptar los angulos reales ##### 	//
        ans = real_angle( leg, ans);//		pasamos la soluci�n de los angulos y los adaptamos a cada servomotor

        return ans;
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
