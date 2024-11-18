

#include "esp_timer.h"
#include "servo_driver.h"


#define FRAME_TIME_MS      20  // 33 HA SIDO SIEMPRE
#define FRAME_TIME_US      FRAME_TIME_MS * 1000  // 33 HA SIDO SIEMPRE


uint8_t poseSize=18;
unsigned char interpolating=0;
unsigned int  pose_[18];                       // the current pose, updated by Step(), set out by Sync()
unsigned int  nextpose_[18];                   // the destination pose, where we put on load
uint8_t  id_[18];                              // servo id for this index
unsigned long lastframe_;                      // time last frame was sent out
int speed_[18];



// 1. 



void Setup_Servo() // Solo en la inicialización ( seteaba posiciones iniciales )
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
    lastframe_ = esp_timer_get_time();// / 1000; // Inicializa con el tiempo actual en milisegundos

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

void Interpolate_Setup( int time )
{

    uint8_t frames = (time/FRAME_TIME_MS) + 1;
    lastframe_ = esp_timer_get_time();
    // set speed each servo...
    for(uint8_t i=0;i<poseSize;i++)
    {
        if(nextpose_[i] > pose_[i])
        {
            speed_[i] = ( nextpose_[i] - pose_[i] ) / frames + 1;// siempre mas uno porque se desprecia la parte decimal y se quiere redondear al alza
        }
        else
        {
            speed_[i] = ( pose_[i] - nextpose_[i] ) / frames + 1;
        }
    }

    interpolating = 1;
}

void Interpolate_Step()
{
    if(interpolating == 0)
    {
        return;
    }

    int complete = poseSize;

    if(FRAME_TIME_US > esp_timer_get_time() - lastframe_ )
    {
    	return;// si todavia no se ha cumplido el tiempo del frame salimos de la funci�n
    }
    else
    {
        lastframe_ = esp_timer_get_time();
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
                    {
                        pose_[i] += speed_[i];
                    }
                }
				else // diff negative
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
void Guardar_posicion(int id, int pos) // id_ e id son numeros naturales (positivos y enteros) de manera que cuando se pasa el id
{
    for(int i=0; i<poseSize; i++) // este se busca en el vector de ordenaci�n de los servos y cuando coinciden, el valor pos se pasa a la siguiente posicion
    {
        if( id_[i] == id )
        {
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