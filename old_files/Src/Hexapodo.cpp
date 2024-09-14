// 
// 
// 

#include "Hexapodo.h"

//#define Debug_gait



			//	########################################			KINEMATIC CODE			##############################################		\\
		  //		*************************************************************************************************************	      \\


void setupIK()
{

// type error: https://stackoverflow.com/questions/36973928/does-not-name-a-type

//######******  AQUI ESTA EL PROBLEMA::: COMO LOS VALORES DE LOS DEFINES HAN CAMBIADO LES CORRESPONDEN OTROS VALORES MIN MAX Y CERO QUE NO SE CORRESPONDEN
	//########################################################################


	//*****#### EL FRENTE EST� DONDE EL USB***, SE TUVO QUE CAMBIAR EN GAITS PARA QUE LAS PATAS SE MOVIERAN RESPECTO EL NUEVO
	// FRENTE (QUE ERA EL CONECTOR DE LA BATERIA), PERO LOS VALORES DE LOS SERVOS SIGUEN ASIGNADOS AL ANTIGUO ORDEN DE LAS PATAS

// RIGHT_FRONT***
PATAS[RIGHT_FRONT].servo_coxa.min = 60;		PATAS[RIGHT_FRONT].servo_coxa.max = 500;	 PATAS[RIGHT_FRONT].servo_coxa.Cero=115;
PATAS[RIGHT_FRONT].servo_femur.min = 60;	PATAS[RIGHT_FRONT].servo_femur.max = 500;	 PATAS[RIGHT_FRONT].servo_femur.Cero=95;
PATAS[RIGHT_FRONT].servo_tibia.min = 55;	PATAS[RIGHT_FRONT].servo_tibia.max = 495;	 PATAS[RIGHT_FRONT].servo_tibia.Cero=155;
// RIGHT_REAR***
PATAS[RIGHT_REAR].servo_coxa.min = 60;	PATAS[RIGHT_REAR].servo_coxa.max = 500;		PATAS[RIGHT_REAR].servo_coxa.Cero=110;//110
PATAS[RIGHT_REAR].servo_femur.min = 70;	PATAS[RIGHT_REAR].servo_femur.max = 500;	PATAS[RIGHT_REAR].servo_femur.Cero=105;
PATAS[RIGHT_REAR].servo_tibia.min = 60;	PATAS[RIGHT_REAR].servo_tibia.max = 505;PATAS[RIGHT_REAR].servo_tibia.Cero=170;

// LEFT_FRONT***
PATAS[LEFT_FRONT].servo_coxa.min = 60;	PATAS[LEFT_FRONT].servo_coxa.max = 500;	PATAS[LEFT_FRONT].servo_coxa.Cero=90;//90
PATAS[LEFT_FRONT].servo_femur.min = 55;	PATAS[LEFT_FRONT].servo_femur.max = 495;	PATAS[LEFT_FRONT].servo_femur.Cero=75;
PATAS[LEFT_FRONT].servo_tibia.min = 55;	PATAS[LEFT_FRONT].servo_tibia.max = 495; PATAS[LEFT_FRONT].servo_tibia.Cero=170;

// LEFT_REAR***
PATAS[LEFT_REAR].servo_coxa.min = 60;	PATAS[LEFT_REAR].servo_coxa.max = 500;	PATAS[LEFT_REAR].servo_coxa.Cero=80;
PATAS[LEFT_REAR].servo_femur.min = 60;	PATAS[LEFT_REAR].servo_femur.max = 495;	PATAS[LEFT_REAR].servo_femur.Cero=80;
PATAS[LEFT_REAR].servo_tibia.min = 55;	PATAS[LEFT_REAR].servo_tibia.max = 495;	PATAS[LEFT_REAR].servo_tibia.Cero=150;

// RIGHT_MIDDLE***
PATAS[RIGHT_MIDDLE].servo_coxa.min = 55;	PATAS[RIGHT_MIDDLE].servo_coxa.max = 495;	PATAS[RIGHT_MIDDLE].servo_coxa.Cero=110;
PATAS[RIGHT_MIDDLE].servo_femur.min = 60;	PATAS[RIGHT_MIDDLE].servo_femur.max = 500;	PATAS[RIGHT_MIDDLE].servo_femur.Cero=100;
PATAS[RIGHT_MIDDLE].servo_tibia.min = 55;	PATAS[RIGHT_MIDDLE].servo_tibia.max = 490;	PATAS[RIGHT_MIDDLE].servo_tibia.Cero=170;


// LEFT_MIDDLE***

PATAS[LEFT_MIDDLE].servo_coxa.min = 60;		PATAS[LEFT_MIDDLE].servo_coxa.max = 500;	PATAS[LEFT_MIDDLE].servo_coxa.Cero=75;
PATAS[LEFT_MIDDLE].servo_femur.min = 55;	PATAS[LEFT_MIDDLE].servo_femur.max = 495;	PATAS[LEFT_MIDDLE].servo_femur.Cero=105;
PATAS[LEFT_MIDDLE].servo_tibia.min = 60;	PATAS[LEFT_MIDDLE].servo_tibia.max = 500;PATAS[LEFT_MIDDLE].servo_tibia.Cero=155;




//PATA 1: LEFT_FRONT
/*
endpoints[RIGHT_FRONT].x = 153;
endpoints[RIGHT_FRONT].y = 79;
endpoints[RIGHT_FRONT].z = -100;
*/
//DEFAULT
//PATA 4
endpoints[RIGHT_FRONT].x = 115;
endpoints[RIGHT_FRONT].y = 100;
endpoints[RIGHT_FRONT].z = -100;

//PATA 6
endpoints[RIGHT_REAR].x = 115;//
endpoints[RIGHT_REAR].y = -100;
endpoints[RIGHT_REAR].z = -100;    //-90

//PATA 1
endpoints[LEFT_FRONT].x = -115;	//
endpoints[LEFT_FRONT].y = 100;
endpoints[LEFT_FRONT].z = -100;	//-90

//PATA 3
endpoints[LEFT_REAR].x = -115;//
endpoints[LEFT_REAR].y = -100;
endpoints[LEFT_REAR].z = -100;	//-90

//PATA 5
endpoints[RIGHT_MIDDLE].x = 155;
endpoints[RIGHT_MIDDLE].y = 0;
endpoints[RIGHT_MIDDLE].z = -100;	//-90

//PATA 2
endpoints[LEFT_MIDDLE].x = -155;//
endpoints[LEFT_MIDDLE].y = 0;
endpoints[LEFT_MIDDLE].z = -100;	//-90

// ENDPOINT OFICIALMENTE DESDE COXA HASTA LA PUNTA DE LA PATA

// VECTORES DE POSICION COXA

//PATA 1
Posicion_coxa[RIGHT_FRONT].x = X_COXA;
Posicion_coxa[RIGHT_FRONT].y = Y_COXA;

//PATA 2
Posicion_coxa[RIGHT_REAR].x = X_COXA;
Posicion_coxa[RIGHT_REAR].y = -Y_COXA;

//PATA 3
Posicion_coxa[LEFT_FRONT].x = -X_COXA;	//**********************
Posicion_coxa[LEFT_FRONT].y = Y_COXA;

//PATA 4
Posicion_coxa[LEFT_REAR].x = -X_COXA;
Posicion_coxa[LEFT_REAR].y = -Y_COXA;

//PATA 5
Posicion_coxa[RIGHT_MIDDLE].x = MX_COXA;
Posicion_coxa[RIGHT_MIDDLE].y = 0;

//PATA 6
Posicion_coxa[LEFT_MIDDLE].x = -MX_COXA;
Posicion_coxa[LEFT_MIDDLE].y = 0;

TraslacionX=0;
TraslacionY=0;
TraslacionZ=0;

liftHeight=45;

}


/* Convert radians to servo position offset.
int radToServo(float rads){
  float val = (rads*100)/51 * 100;
  return (int) val;
}
*/

// Tambi�n se puede multiplicar por la constante de proporcionalidad y se obtiene directamente,
// Pero tenemos que crear una variable float

int radToServoV2(float rads) // pasar el offset se va a eliminar
{
  float val = ((Constante_PWM)/HALF_PI)*rads;// el offset es el Delta_90

  return (int) val;
}

int ServoToDEG(int servo_pwm)
{
	//servo PWM TO RAD
	float angulo_rad=servo_pwm*((HALF_PI)/(Constante_PWM));
	//RAD TO DEG
	int angulo_deg=(int)RAD_TO_DEG*angulo_rad;
	return angulo_deg;
}

ik_sol_t  Real_angle(uint8_t leg, ik_sol_t angulo)
{

	int added_angle=0;
	//		######################		 COXA	  ######################		//
	//		ESPECIFICAMOS para cada pata porque solo para coxa es distinto
	if(leg==RIGHT_FRONT){
		angulo.coxa=PATAS[leg].servo_coxa.Cero+(Constante_PWM/2)+angulo.coxa;//parece correcto

	}else if(leg==RIGHT_REAR){
		angulo.coxa=PATAS[leg].servo_coxa.Cero+(3*(Constante_PWM/2))+angulo.coxa;

	}else if(leg==LEFT_FRONT){		//			2 CASOS
					if((angulo.coxa>0)||(angulo.coxa==0))			angulo.coxa=PATAS[leg].servo_coxa.Cero-(Constante_PWM/2)+angulo.coxa;/*hecho*/
					else{ 		angulo.coxa=-angulo.coxa;//consideramos el �ngulo positivo para hacer c�lculos
									 added_angle=2*Constante_PWM-angulo.coxa;//a 180 le restamos el �ngulo coxa calculado y obtenemos el �ngulo a a�adir
									 angulo.coxa=PATAS[leg].servo_coxa.Cero+(3*(Constante_PWM/2))+added_angle;
					}

	}else if(leg==LEFT_REAR){		//			2 CASOS
					if((angulo.coxa>0)||(angulo.coxa==0)){
						added_angle=angulo.coxa-(3*(Constante_PWM/2));
						angulo.coxa=PATAS[leg].servo_coxa.Cero+added_angle;/*hecho*/
					}else{
					angulo.coxa=-angulo.coxa;//consideramos el �ngulo positivo para hacer c�lculos
							added_angle=2*Constante_PWM-angulo.coxa;//a 180 le restamos el �ngulo coxa calculado y obtenemos el �ngulo a a�adir
							angulo.coxa=PATAS[leg].servo_coxa.Cero+Constante_PWM/2+added_angle;
					}

	}else if(leg==RIGHT_MIDDLE){
		angulo.coxa=PATAS[leg].servo_coxa.Cero+Constante_PWM+angulo.coxa;

	}else if(leg==LEFT_MIDDLE){		//			2 CASOS
							if((angulo.coxa>0)||(angulo.coxa==0)){
								added_angle=angulo.coxa-(3*(Constante_PWM/2));
								angulo.coxa=PATAS[leg].servo_coxa.Cero+Constante_PWM/2+added_angle;	/*hecho*/
							}else{ 		angulo.coxa=-angulo.coxa;//consideramos el �ngulo positivo para hacer c�lculos
											 added_angle=2*Constante_PWM-angulo.coxa;//a 180 le restamos el �ngulo coxa calculado y obtenemos el �ngulo a a�adir
											 angulo.coxa=PATAS[leg].servo_coxa.Cero+Constante_PWM+added_angle;
							}
	}
	//		####################		   FEMUR	 	 #####################		//
	// PARA todas las patas igual
angulo.femur=PATAS[leg].servo_femur.Cero+Constante_PWM-angulo.femur; // GOOD
	//angulo.femur=PATAS[leg].servo_femur.Cero+(Constante_PWM/2)+angulo.femur;
	//angulo.femur=PATAS[leg].servo_femur.Cero+(3*(Constante_PWM/2))-angulo.femur;
	//		####################		   TIBIA			  ######################		//
	// PARA todas las patas igual
	angulo.tibia=PATAS[leg].servo_tibia.Cero+angulo.tibia;

	return angulo;
}




/* Body IK solver: compute where legs should be. */
// X_pata, Y_pata, Z_pata : son los endpoint m�s lo que se tenga que a�adir por el gait
ik_req_t bodyIK(uint8_t leg ){//, float Zrot){
	//endpoint(x,y,z) X_COXA Y_COXA
	//yo quiero que se pase como par�metro solo la distancia COXA

    ik_req_t new_endpoint;
    ik_req_t resultado;
																																/*Calculamos el total en X Y e Z
																																    DZ_COXA = 0;	la DZ_COXA ES * 0 * SIEMPRE*/
    float Calf = cos (bodyRotX);  				 int TraslacionX_;				int totalX =   Posicion_coxa[leg].x + endpoints[leg].x; 	//distancia X total inicial
    float Salf = sin (bodyRotX);	 			 int TraslacionY_;			    int totalY =   Posicion_coxa[leg].y + endpoints[leg].y; 	//distancia Y total inicial
    float Cphi = cos (bodyRotY);	     		 int TraslacionZ_;			    int totalZ =   endpoints[leg].z;			//distancia Z total inicial
    float Sphi = sin (bodyRotY);   				 int VectorX_;						uint8_t  DZ_COXA=0;
    float Cthe = cos (bodyRotZ); 	   			 int VectorZ_;
    float Sthe = sin (bodyRotZ);				 int VectorY_;
    //por DEFECTO bodyrotZ es
    //la rotacion del CUERPO

    if(modo_control==1){	//movimiento robot
    	 Cthe = cos (gaits[leg].r);	// Rotacion sobre s� mismo (GAITS)
    	 Sthe = sin (gaits[leg].r);
    	TraslacionX_ = gaits[leg].x;
    	TraslacionY_ = gaits[leg].y;
    	TraslacionZ_ = gaits[leg].z;
    	VectorX_ = totalX;
    	VectorY_ = totalY;
    	VectorZ_ = totalZ;

    }else if(modo_control==0){		//movimiento cuerpo
    	TraslacionX_ = TraslacionX;
    	TraslacionY_ = TraslacionY;
    	TraslacionZ_ = TraslacionZ;
    	VectorX_ =  Posicion_coxa[leg].x;
    	VectorY_ =  Posicion_coxa[leg].y;
    	VectorZ_ = 0;
    }

    //	MATRIZ DE TRANSFORMACION HOMOGÉNEA ****	(ROTACION Y TRASLACION)		****
    resultado.x = TraslacionX_+int( Cthe*Cphi*VectorX_ + ( -Calf*Sthe+Cthe*Salf*Sphi )*VectorY_ + (Salf*Sthe+Calf*Cthe*Sphi )*VectorZ_);
    resultado.y = TraslacionY_+int( Cphi*Sthe*VectorX_ + (Calf*Cthe+Salf*Sthe*Sphi )*VectorY_ + (-Cthe*Salf+Calf*Sthe*Sphi )*VectorZ_) ;
    resultado.z = TraslacionZ_+int( -Sphi*VectorX_ + Cthe*Salf*VectorY_ + Calf*Cphi*VectorZ_) ;


    if(modo_control==1){	//movimiento robot
    	new_endpoint.x =resultado.x  -  Posicion_coxa[leg].x;
    	new_endpoint.y = resultado.y -  Posicion_coxa[leg].y;
    	new_endpoint.z = resultado.z -DZ_COXA;  	 // DZ_COXA es cero
        }else if(modo_control==0){		//movimiento cuerpo
        	new_endpoint.x = totalX - resultado.x;
		 	new_endpoint.y = totalY -  resultado.y;
			new_endpoint.z = totalZ -  resultado.z;
        }
	//Nuevo vector endpoint resultante
    return new_endpoint;
}

/* Simple 3dof leg solver. X,Y,Z are the length from the Coxa rotate to the endpoint. */
ik_sol_t legIK(int X, int Y, int Z, int leg){
    ik_sol_t ans; // angulo en servo pwm


    // primero, resolver el angulo coxa para conseguir que nuestro problema se reduzca a un problema 2D
		ans.coxa = radToServoV2(atan2(Y,X));//pata ya es una direccion de memoria


    long trueX = sqrt(sq((long)X)+sq((long)Y)) - L_COXA;
    long im = sqrt(sq((long)trueX)+sq((long)Z));    // length of imaginary leg
    // get femur angle above horizon...
    float q1 = atan2(Z,trueX);// Mientras que Z sea negativo, el angulo saldr� negativo, cuando la pata sobrepase el eje horizontal, el angulo sera positivo
	long n1 = sq(L_TIBIA)-sq(L_FEMUR)-sq(im);//numerador
    long d2 = -2*L_FEMUR*im;//denominador
    float q2 = acos((float)n1/(float)d2);
    ans.femur = radToServoV2((q2+q1)); // lo pongo positivo pero ahora la z es negativa ya que el centro esta en coxa y endpoint esta por debajo
	//float intermedio=q2-q1;

    // and tibia angle from femur...
    n1 = sq(im)-sq(L_FEMUR)-sq(L_TIBIA);
    d2 = -2*L_FEMUR*L_TIBIA;
    //alfa=n1/d2 //teta3=alfa-90�
    ans.tibia = radToServoV2((acos((float)n1/(float)d2))-1.57);

		//  ##### adaptar los �ngulos reales ##### 	//
		  ans=Real_angle( leg, ans);//		pasamos la soluci�n de los angulos y los adaptamos a cada servomotor


    return ans;

}
// #define DEBUG_DOIK

#ifdef DEBUG_DOIK
unsigned long tiempo_ant_doIK=0;
unsigned long tiempo_sig_doIK=0;
unsigned long tiempo_pasado_doIK=0;
#endif



void doIK(){
#ifdef DEBUG_DOIK
	tiempo_ant_doIK=millis();
#endif

    //	Volvemos a poner el flag de fallo a falso	//
	servo_fail=false;
    //	************************	/

 for(uint8_t n_pata=0; n_pata<6;n_pata++){
	    ik_req_t req;
	    ik_sol_t sol_ik;
	    uint8_t servo_actual;
 				Gait_generator(n_pata);
/*
 					 Serial.print("Gait X: ");
 					 Serial.print(gaits[n_pata].x );
 					 Serial.print("Gait Y: ");
 					 Serial.print(gaits[n_pata].y );
 					 Serial.print("Gait Z: ");
 					 Serial.println(gaits[n_pata].z );
*/
 				sol_ik = legIK(req.x,req.y,req.z,n_pata);//hemos a�adido el siguiente parametro
 				req= bodyIK(n_pata);	  // Hay que generar un vector de puntos coxa asociados a cada pata como en el endpoint, que guarde los puntos pat solo tener
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

	//###TEST###




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



}//doik()



















//	########################################			GAIT GENERATOR CODE			##############################################		\\
//		*************************************************************************************************************	      \\





void Gait_generator(uint8_t leg){




		if(!MOVING ||modo_control==0){		// NOT MOVING***
			if(parado==false)parado=true;
			gaits[leg].x = 0;
			gaits[leg].y = 0;
			gaits[leg].z = 0;
			gaits[leg].r = 0;
			//step=1;
		}else{ 		//  MOVING***

			if(parado==true) {	parado=false;		step=1;}



		//step = (step+1)%stepsInCycle;
		// cambiar (stepsInCycle-pushSteps) por DESFASE: 1, 2, 3, 4, o 6 seg�n: (6, 12, 24 steps), wave 24 steps o tripod 24 steps
		leg_step = step-(gaitLegNo[leg]-1)*(desfase);		 // vale gial, se supone que est� bien
			if (leg_step<0)	leg_step=stepsInCycle + leg_step;  // parece que funciona para ciclos de 6, 12 y 24 steps en RIPPLE
			if(leg_step==0) leg_step=stepsInCycle;


		if((Current_Gait==ripple_6)||(Current_Gait==tripod_6)||(Current_Gait==wave_12)){	// ### 3 ETAPAS ###

			if(leg_step == 1){	  // UP
				gaits[leg].x = 0;
				gaits[leg].y = 0;
				gaits[leg].z = liftHeight;
				gaits[leg].r = 0;
			}
			else if(leg_step == 2){	// DOWN
				gaits[leg].x = (Xspeed*cycleTime*pushSteps)/(2*stepsInCycle);
				gaits[leg].y = (Yspeed*cycleTime*pushSteps)/(2*stepsInCycle);
				gaits[leg].z = 0;
				gaits[leg].r = (Rspeed*cycleTime*pushSteps)/(2*stepsInCycle);
			}
				else{	  // MOVE BODY FORWARD
					gaits[leg].x = gaits[leg].x - (Xspeed*cycleTime)/stepsInCycle;
					gaits[leg].y = gaits[leg].y - (Yspeed*cycleTime)/stepsInCycle;
					gaits[leg].z = 0;
					gaits[leg].r = gaits[leg].r - (Rspeed*cycleTime)/stepsInCycle;
			}
}//6

		if((Current_Gait==ripple_12)||(Current_Gait==wave_24)||(Current_Gait==tripod_12)){ //	### 5 ETAPAS ###

			if(leg_step == stepsInCycle){	// UP/2 :::: 12 (stepsInCycle)
				gaits[leg].x = gaits[leg].x/2;
				gaits[leg].y = gaits[leg].y/2;
				gaits[leg].z = liftHeight/2;
				gaits[leg].r = gaits[leg].r/2;
			}
			else if(leg_step == 1){	// UP
				gaits[leg].x = 0;
				gaits[leg].y = 0;
				gaits[leg].z = liftHeight;
				gaits[leg].r = 0;
			}
			else if(leg_step == 2){	// DOWN/2
				gaits[leg].x = (Xspeed*cycleTime*pushSteps)/(4*stepsInCycle);
				gaits[leg].y = (Yspeed*cycleTime*pushSteps)/(4*stepsInCycle);
				gaits[leg].z = int(liftHeight/2);
				gaits[leg].r = (Rspeed*cycleTime*pushSteps)/(4*stepsInCycle);
			}
			else if(leg_step == 3){	// DOWN
				gaits[leg].x = (Xspeed*cycleTime*pushSteps)/(2*stepsInCycle);
				gaits[leg].y = (Yspeed*cycleTime*pushSteps)/(2*stepsInCycle);
				gaits[leg].z = 0;
				gaits[leg].r = (Rspeed*cycleTime*pushSteps)/(2*stepsInCycle);
			}
				else {	// MOVE BODY FORWARD
					gaits[leg].x = gaits[leg].x - (Xspeed*cycleTime)/stepsInCycle;
					gaits[leg].y = gaits[leg].y - (Yspeed*cycleTime)/stepsInCycle;
					gaits[leg].z = 0;
					gaits[leg].r = gaits[leg].r - (Rspeed*cycleTime)/stepsInCycle;
			}
}//12

		if((Current_Gait==ripple_24)||(Current_Gait==tripod_24)){//	### 7 ETAPAS ###

					if(leg_step == stepsInCycle-1){	// UP 1/3
						gaits[leg].x = gaits[leg].x/3;
						gaits[leg].y = gaits[leg].y/3;
						gaits[leg].z = liftHeight/3;
						gaits[leg].r = gaits[leg].r/3;
					}
					else if(leg_step == stepsInCycle){	// UP 2/3
						gaits[leg].x = gaits[leg].x*2/3;
						gaits[leg].y = gaits[leg].y*2/3;
						gaits[leg].z = liftHeight*2/3;
						gaits[leg].r = gaits[leg].r*2/3;
					}
					else if(leg_step == 1){	// UP
						gaits[leg].x = 0;
						gaits[leg].y = 0;
						gaits[leg].z = liftHeight;
						gaits[leg].r = 0;
					}
					else if(leg_step == 2){	// DOWN 1/3
						gaits[leg].x = (Xspeed*cycleTime*pushSteps)/(3*stepsInCycle);
						gaits[leg].y = (Yspeed*cycleTime*pushSteps)/(3*stepsInCycle);
						gaits[leg].z = liftHeight*2/3;
						gaits[leg].r = (Rspeed*cycleTime*pushSteps)/(3*stepsInCycle);
					}
					else if(leg_step == 3){	// DOWN 2/3
						gaits[leg].x = (Xspeed*cycleTime*pushSteps)*2/(3*stepsInCycle);
						gaits[leg].y = (Yspeed*cycleTime*pushSteps)*2/(3*stepsInCycle);
						gaits[leg].z = liftHeight/3;
						gaits[leg].r = (Rspeed*cycleTime*pushSteps)*2/(3*stepsInCycle);
					}
					else if(leg_step == 4){	// DOWN
						gaits[leg].x = (Xspeed*cycleTime*pushSteps)/(2*stepsInCycle);// same as Xspeed*trantime*pushsteps/2
						gaits[leg].y = (Yspeed*cycleTime*pushSteps)/(2*stepsInCycle);// same as Yspeed*trantime*pushsteps/2
						gaits[leg].z = 0;
						gaits[leg].r = (Rspeed*cycleTime*pushSteps)/(2*stepsInCycle);// same as Rspeed*trantime*pushsteps/2
					}else{	// MOVE BODY FORWARD
						gaits[leg].x = gaits[leg].x - (Xspeed*cycleTime)/stepsInCycle;// same as Xspeed*trantime
						gaits[leg].y = gaits[leg].y - (Yspeed*cycleTime)/stepsInCycle;// same as Yspeed*trantime
						gaits[leg].z = 0;
						gaits[leg].r = gaits[leg].r - (Rspeed*cycleTime)/stepsInCycle;// same as Rspeed*trantime
					}
}//24

#ifdef Debug_gait
		Serial.print("Step referencia: ");
		Serial.print(step);
		Serial.print("  pata n�: ");
		Serial.print(leg);
		Serial.print(" leg_step: ");
		Serial.print(leg_step);
		Serial.print("  Gait.r:  ");
		Serial.print(gaits[leg].r);
		Serial.print("  Gait.X:  ");
		Serial.print(gaits[leg].x);
		Serial.print("  Gait.Y:  ");
		Serial.print(gaits[leg].y);
		Serial.print("  Gait.Z:  ");
		Serial.println(gaits[leg].z);
#endif
		}

	}

void Gait_body_config(){
	tranTime = 140;
}

void gaitSelect(gait GaitType)  //ripple_6, ripple_12,	ripple_24,	wave_12,	wave_24,	tripod_6,	tripod_12,	tripod_24
{
	if(GaitType == Current_Gait)
    {
	    return;
    }

	Current_Gait = GaitType;
	liftHeight=45;
	// RIPPLE
	if((Current_Gait == ripple_6)||(Current_Gait == ripple_12)||(Current_Gait == ripple_24))
    {
		//ORDEN PATAS
        gaitLegNo[RIGHT_FRONT] = 4;//5;// actualizado
        gaitLegNo[RIGHT_REAR] =2; //1;// actualizado
        gaitLegNo[LEFT_FRONT] =1; //2;// actualizado
        gaitLegNo[LEFT_REAR] = 5;//4;// actualizado
        gaitLegNo[RIGHT_MIDDLE] =6;// 3;// actualizado
        gaitLegNo[LEFT_MIDDLE] =3; //6;// actualizado

		switch(Current_Gait)
        {
            case ripple_6 :
                pushSteps=4;
                stepsInCycle=6;
                desfase=1;
                tranTime=140;
                break;
            case ripple_12 :
                pushSteps=8;
                stepsInCycle=12;
                desfase=2;
                tranTime=120;
                break;
            case ripple_24 :
                pushSteps=12;
                stepsInCycle=18;
                desfase=3;
                tranTime=120;
                break;
            default:
                break;
        }
	}
    else if((Current_Gait == tripod_6)||(Current_Gait == tripod_12)||(Current_Gait == tripod_24))
    {

		gaitLegNo[RIGHT_FRONT] = 2;	// actualizado
		gaitLegNo[RIGHT_REAR] = 2;	// actualizado
		gaitLegNo[LEFT_FRONT] = 1;	// actualizado
		gaitLegNo[LEFT_MIDDLE] = 2;	// actualizado
		gaitLegNo[LEFT_REAR] = 1;	// actualizado
		gaitLegNo[RIGHT_MIDDLE] = 1;	// actualizado

		switch(Current_Gait) 
        {
            case tripod_6 : 	//	parece que hay que agregarle algun step entre elevaciones alternas de las 3 patas
                pushSteps=2;	//6 	ya que justo cuando una toca el suelo, la otra se levanta y se tambalea un poco
                stepsInCycle=4;//7
                desfase=2;//3
                tranTime=140;
                break;
            case tripod_12 :
                pushSteps=4;
                stepsInCycle=8;
                desfase=4;	// antes era 4 (para hacer que transicione mejor)
                tranTime=120;
                break;
            case tripod_24 :
                pushSteps=6;//19
                stepsInCycle=12;//24
                desfase=6;
                tranTime=120;
                break;
            default:
                break;
		}
//		tranTime = 65;???????????????????????????
		}else if((Current_Gait == wave_12)||(Current_Gait == wave_24)) //wave_12,	wave_24
        {		
			gaitLegNo[RIGHT_REAR] = 6;// actualizado
			gaitLegNo[RIGHT_MIDDLE] = 5;// actualizado
			gaitLegNo[RIGHT_FRONT] = 4;// actualizado
			gaitLegNo[LEFT_REAR] = 3;// actualizado
			gaitLegNo[LEFT_MIDDLE] = 2;// actualizado
			gaitLegNo[LEFT_FRONT] = 1;// actualizado

		switch(Current_Gait)
        {
            case wave_12 :
                pushSteps=10;
                stepsInCycle=12;
                desfase=2;
                tranTime=80;
                break;
            case wave_24 :
                pushSteps=20;
                stepsInCycle=24;
                desfase=4;
                tranTime=80;
                break;
            default:
            break;
        }
}
	// 	RECALCULAMOS VARIABLES		******************************************

	cycleTime = (stepsInCycle*tranTime)/1000.0;
	step = 1;	//	pero hay que poner la step por la que empezamos
	calculo_recorrido ();

	//	Serial.print("Step actual: ");
	//	Serial.println(step);
}

/**
 * @brief 
 * @details
 *                            Distancia recorrida (mm)
 *	 Veloc_max =   ----------------------------------------------
 *                      (trantime(s) / conv_to__ms) * pushsteps
 */
void calculo_recorrido ()
{
	Veloc_X_max=(70.0)/((tranTime/1000.0)*pushSteps);
	Veloc_Y_max=(70.0)/((tranTime/1000.0)*pushSteps);
	Veloc_Rot_max=(PI/7)/((tranTime/1000.0)*pushSteps);
}

//	########################################			SERVO CONTROL CODE			##############################################		\\
//		*************************************************************************************************************	      \\


void Setup_Servo()
{
    //REPOSO NO ES INICIO
    // initialize

    for(uint8_t i=0;i<poseSize;i++)
    {
        id_[i] = i+1;
        pose_[i] = agachados_ [i];
        nextpose_[i] = agachados_ [i];
    }

    interpolating = 0;
    //playing = 0;
    lastframe_ = millis();
}


/* write pose out to servos using sync write. */
void Escribir_posicion()
{
	//Serial.print("primer servo: ");
  	// Serial.print("{");

    for(uint8_t i=0; i<poseSize; i++)
    {
  	    //Serial.print(pose_[i]);
	    // Serial.print(",");
        if(i<9) pwm2.setPWM(i, 0, pose_[i]);    //	EL PRIMER VALOR ES EL N� DEL CANAL
        if(i>=9) pwm.setPWM(24-i, 0, pose_[i]);	//	EL SEGUNDO VALOR ES EL TICK PARA EL CUAL LA SE�AL PASA DE OFF A ON (0 SIGNIFICA QUE EMPIEZA JUSTO AL PRINCIPIO)
                                                //  EL TERCER VALOR INDICA EL TICK PARA EL CUAL LA SE�AL PASA DE ON A OFF(AL PASAR LA SE�AL A OFF LA HEMOS DEJADO EN ON UN TIEMPO TON)
    }

	//Serial.println("}");

    /* set up for an interpolation from pose to nextpose over TIME milliseconds by setting servo speeds. */
}

void Interpolate_Setup(int time){

    uint8_t frames = (time/BIOLOID_FRAME_LENGTH) + 1;
    lastframe_ = millis();
    // set speed each servo...
    for(uint8_t i=0;i<poseSize;i++){

        if(nextpose_[i] > pose_[i]){
            speed_[i] = (nextpose_[i] - pose_[i])/frames + 1;// siempre mas uno porque se desprecia la parte decimal y se quiere redondear al alza
        }else{
            speed_[i] = (pose_[i]-nextpose_[i])/frames + 1;
        }
    }

    interpolating = 1;
}

unsigned long tiempo_ant_doIK = 0;
unsigned long tiempo_sig_doIK = 0;
unsigned long tiempo_pasado_doIK = 0;

//    while(millis() - lastframe_ < BIOLOID_FRAME_LENGTH);	********************************
//	    lastframe_ = millis();																		********************************
/* interpolate our pose, this should be called at about 30Hz. */
void Interpolate_Step()
{
    if(interpolating == 0)
    {
        return;
    }

    int complete = poseSize;
    if(BIOLOID_FRAME_LENGTH > millis() - lastframe_ )
    {
    	return;// si todavia no se ha cumplido el tiempo del frame salimos de la funci�n
    }
    else
    {
        lastframe_ = millis();
        // update each servo
        for(uint8_t i=0;i<poseSize;i++)
        {
            int diff = nextpose_[i] - pose_[i];
            if(diff == 0)
			{
                complete--;
            }
			else
			{
                if(diff > 0)
				{
                    if(diff < speed_[i])
					{
                        pose_[i] = nextpose_[i];
                        complete--;
                    }
					else
                        pose_[i] += speed_[i];
                }
				else
				{
                    if((-diff) < speed_[i]){

                        pose_[i] = nextpose_[i];
                        complete--;
                    }
					else
					{
                        pose_[i] -= speed_[i];
					}
                }
            }
        }
    }

    if(complete <= 0)
    {
    	interpolating = 0;
/*
    	tiempo_sig = millis();
    		 tiempo_pasado = tiempo_sig- tiempo_ant;
    			 Serial.print("Tiempo pasado:  ");
    			 Serial.println(tiempo_pasado);
*/
    }
	//ESTE METODO HAY QUE REDEFINIRLO***********
    Escribir_posicion();
    /*
    tiempo_sig_doIK = millis();
    tiempo_pasado_doIK = tiempo_sig_doIK- tiempo_ant_doIK;
    tiempo_ant_doIK=tiempo_sig_doIK;
    Serial.print(F("Tiempo doIK():  "));
    Serial.println(tiempo_pasado_doIK);
    */
}


/* set a servo value in the next pose */
void Guardar_posicion(int id, int pos){// id_ e id son numeros naturales (positivos y enteros) de manera que cuando se pasa el id
    for(int i=0; i<poseSize; i++){// este se busca en el vector de ordenaci�n de los servos y cuando coinciden, el valor pos se pasa a la siguiente posicion
        if( id_[i] == id ){
            nextpose_[i] = pos;// << BIOLOID_SHIFT);
            return;
        }
    }
}



void Inicializacion_servos ()
{
	 Escribir_posicion();		// ESCRIBIMOS POSICION DE REPOSO QUE VIENEN COMO DEFAULT EN BIOLOID CONTROLLER
	 delay(1000);//esperamos a que se ubiquen los servos


	for(int i=1;i<19;i++) Guardar_posicion(i,Servos_inicio[i-1]);

	Interpolate_Setup(1000);

	while(interpolating > 0)
	{
		Interpolate_Step();
	}

	for(int i=1;i<19;i++) Guardar_posicion(i,Servos_grupo1_up[i-1]);

	Interpolate_Setup(500);

	while(interpolating > 0)
	{
		Interpolate_Step();
	}

	for(int i=1;i<19;i++) Guardar_posicion(i,Servos_grupo2_up[i-1]);

	Interpolate_Setup(500);

	while(interpolating > 0)
	{
		Interpolate_Step();
	}


	for(int i=1;i<19;i++) Guardar_posicion(i,Servos_inicio[i-1]);

	Interpolate_Setup(500);

	while(interpolating > 0)
	{
		Interpolate_Step();
	}
}


