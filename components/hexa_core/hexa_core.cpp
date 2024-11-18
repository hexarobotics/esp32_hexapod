

#include "hexa_params.h"
#include "Hexaik.h"
#include "gaits.h"
#include "Servo_controller.h"

// NEWW

// Inicialización estática del array en el .cpp
const transformations3D::Vectors::vector2d coxa_endpoints[6] =
{
    {-X_COXA, Y_COXA},     // PATA 1: LEFT_FRONT
    {-MX_COXA, 0},         // PATA 2: LEFT_MIDDLE
    {-X_COXA, -Y_COXA},    // PATA 3: LEFT_REAR
    {X_COXA, Y_COXA},      // PATA 4: RIGHT_FRONT
    {MX_COXA, 0},          // PATA 5: RIGHT_MIDDLE
    {X_COXA, -Y_COXA}      // PATA 6: RIGHT_REAR
};

const transformations3D::Vectors::vector3d leg_endpoints[6] =
{
    {-115, 100, -100},     // PATA 1: LEFT_FRONT
    {-155, 0, -100},       // PATA 2: LEFT_MIDDLE
    {-115, -100,-100},     // PATA 3: LEFT_REAR
    {115, 100, -100},      // PATA 4: RIGHT_FRONT
    {155, 0, -100},        // PATA 5: RIGHT_MIDDLE
    {115, -100, -100}      // PATA 6: RIGHT_REAR
};

transformations3D::Vectors::vector3d calculate_total_vector( uint8_t leg )
{
    transformations3D::Vectors::vector3d v_out;

    v_out.x = coxa_endpoints[leg].x + leg_endpoints[leg].x;
    v_out.y = coxa_endpoints[leg].y + leg_endpoints[leg].y;
    v_out.z = leg_endpoints[leg].z;
    
    return v_out;
}

transformations3D::Vectors::vector3d recalculate_leg_vector( uint8_t leg, transformations3D::Vectors::vector3d in_vect )
{
    transformations3D::Vectors::vector3d v_out;

    v_out.x = in_vect.x - coxa_endpoints[leg].x;
    v_out.y = in_vect.y - coxa_endpoints[leg].y;
    v_out.z = in_vect.z;
    
    return v_out;
}

void hexa_main_task()
{
	hexapod::Gaits gait(RIPPLE_6);

	Servo::ServoController servo_ctr; 

	// more tasks?

    while(1)
    {
        // 1. get_parameters: leemos de la app los comandos de movimiento
        // 2. is_moving()?
        // 3. calculate_gait_step

        //get_parameters()

        if ( servo_ctr.isInterpolating() && gait.isMoving())
        {
            for( uint8_t leg = LEFT_FRONT; leg < NUM_MAX_LEGS; leg++ )
            {
				// 1. Gait step calculation
                transformations3D::Tmatrix tgait = gait.step(leg);

				// 2. Total Vect
                transformations3D::Vectors::vector3d leg_vect = calculate_total_vector( leg );

				// 3. Apply Transformation matrix to leg_vector
                leg_vect = tgait.apply(leg_vect); // vector modificado

				// 4. Calculate leg vector
                leg_vect = recalculate_leg_vector( leg, leg_vect );

				// 5. Solve IK
                hexapod::Hexaik::ik_angles ik_angles = hexapod::Hexaik::legIK( (leg_id)leg, leg_vect.x, leg_vect.y, leg_vect.z );

				// 6. Save nextpose
				servo_ctr.save_nextpose(leg * 3, 	 ik_angles.coxa );
				servo_ctr.save_nextpose(leg * 3 + 1, ik_angles.femur );
				servo_ctr.save_nextpose(leg * 3 + 2, ik_angles.tibia );

				// 7. Set Next Gait step
                gait.next_step(); // Increment gait step
            }

			servo_ctr.interpolate_setup(gait.get_gait_transition_time());
        }

		// update joints
		servo_ctr.Interpolate_step();
    }


}

/*



void doIK(){
#ifdef DEBUG_DOIK
	tiempo_ant_doIK=millis();
#endif

    //	Volvemos a poner el flag de fallo a falso //
	servo_fail=false;
    //	************************	/

 for(uint8_t n_pata=0; n_pata<6;n_pata++){
	    ik_req_t req;
	    ik_sol_t sol_ik;
	    uint8_t servo_actual;
 				gait_step(n_pata);

 					 Serial.print("Gait X: ");
 					 Serial.print(gaits[n_pata].x );
 					 Serial.print("Gait Y: ");
 					 Serial.print(gaits[n_pata].y );
 					 Serial.print("Gait Z: ");
 					 Serial.println(gaits[n_pata].z );

 				req= bodyIK(n_pata);	  // Hay que generar un vector de puntos coxa asociados a cada pata como en el endpoint, que guarde los puntos pat solo tener
 				sol_ik = legIK(req.x,req.y,req.z,n_pata);//hemos a�adido el siguiente parametro
 				 for(uint8_t n_servo=1; n_servo<4;n_servo++){

							servo_actual=n_pata*3+n_servo;// DESDE 1 hasta 18 PORQUE ES LA ID

// 							Serial.print("N_PATA:actual: ");
// 							Serial.print(n_pata);
//							Serial.print("	N_SERVO:actual: ");
//							Serial.print(n_servo);
//					     	Serial.print(" servo:actual:  ");
//							Serial.print(servo_actual);
//							Serial.print(" Valor pwm:	");
//			   			    Serial.println(servo);

									if(n_servo==1){	// servo ser�a sol_ik.coxa###############
											if(sol_ik.tibia < PATAS[n_pata].servo_tibia.max && sol_ik.tibia > PATAS[n_pata].servo_tibia.min){
												Guardar_posicion(servo_actual, sol_ik.tibia);
											}else{

												Serial.print(F("	fallo Tibia; servo: "));
												Serial.print((servo_actual));
												Serial.print(F("	valor: "));
												Serial.println((sol_ik.tibia));
												//Serial.println("	fallo-S1");
											#ifdef DEBUG_IK
												servo_fail=true;
												Serial.print("	Servo_actual:		");
												Serial.print(servo_actual);
												Serial.print(String_Fails[servo_actual-1]);
												Serial.println(sol_ik.coxa);
											#endif
										}
									}else if(n_servo==2){
											if(sol_ik.femur < PATAS[n_pata].servo_femur.max && sol_ik.femur > PATAS[n_pata].servo_femur.min){
												Guardar_posicion(servo_actual, sol_ik.femur);
											}else{
												Serial.print(F("	fallo Femur; servo: "));
												Serial.print((servo_actual));
												Serial.print(F("	valor: "));
												Serial.println((sol_ik.femur));

												//Serial.println("	fallo-S2");
											#ifdef DEBUG_IK
												servo_fail=true;
												Serial.print("	Servo_actual:		");
												Serial.print(servo_actual);
												Serial.print(String_Fails[servo_actual-1]);
												Serial.println(sol_ik.femur);
											#endif
										}
									}else if(n_servo==3){
											if(sol_ik.coxa < PATAS[n_pata].servo_coxa.max && sol_ik.coxa  > PATAS[n_pata].servo_coxa.min){
												Guardar_posicion(servo_actual, sol_ik.coxa);
											}else{
												Serial.print(F("	fallo Coxa; servo: "));
												Serial.print((servo_actual));
												Serial.print(F("	valor: "));
												Serial.println((sol_ik.coxa));

											//	Serial.print("	fallo-S3: ");
											//	Serial.println(servo_actual);
											//	Serial.print("valor servo: ");
										//		Serial.println(sol_ik.tibia);
											#ifdef DEBUG_IK
												servo_fail=true;
												Serial.print("	Servo_actual:		");
												Serial.print(servo_actual);
												Serial.print(String_Fails[servo_actual-1]);
												Serial.println(sol_ik.tibia);
											#endif
										}
									}
 					}
 			}





step++;
if (step>stepsInCycle)step=1;

#ifdef Muestra_step
    Serial.print("Step Actual: ");
    Serial.println(step);
#endif

#ifdef DEBUG_DOIK
    tiempo_sig_doIK = millis();
    tiempo_pasado_doIK = tiempo_sig_doIK- tiempo_ant_doIK;
    tiempo_ant_doIK=tiempo_sig_doIK;
    Serial.print(F("Tiempo doIK():  "));
    Serial.println(tiempo_pasado_doIK);
#endif



}

*/