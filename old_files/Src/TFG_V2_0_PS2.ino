//	###			DEBUG OPTIONS		###	//

	//#define Debug_JoyStick
	//#define DEBUG

//#include <EEPROM.h>
//#include "arduino.h"


#include "\HEXAPODO_V2_0\Hexapodo.h"
#include "PS2X_lib.h"
#include "Adafruit_PWMServoDriver.h"
#include"Wire.h"

	Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
	Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x44);



	//USB Usb; //USBHub Hub1(&Usb); // Some dongles have a hub inside
	//BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
	/* You can create the instance of the class in two ways */
	//PS3BT PS3(&Btd); // This will just create the instance
	//PS3BT PS3(&Btd, 0x00, 0x15, 0x83, 0xEA, 0x4D, 0xDF); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch
//#include "Controlador_Servos.h"
//#include "Gaits.h"


	//	###			BATERY CONTROL VARIABLES		###		//
			//	********************************		//

				#define red_Pin 7
				#define green_Pin 9
				#define blue_Pin 10
				#define Mosfet  6
				//importante
				bool encendido=true;// si esta en true el robot se enciende

				  // El voltaje mínimo en una celda debe ser de 3.0V
				   //El voltaje máximo en una celda de 4.2V
				int m=8740;
				//int n=17777;
				long n=0;//-200000;//17777;
				 //long n=156184;
				unsigned long voltaje=0;
				unsigned long voltaje_real=0;
				long porcentaje=0;
				unsigned long sumatorio=0;


				//	********************************		//



//CINEMATICA INVERSA ##################################################################
	LEG PATAS  [6];
	ik_req_t endpoints  [6];
	vect_xy  Posicion_coxa[6];
	uint8_t bodyPosX = 0;               // body offset (mm)
	uint8_t bodyPosY = 0;               // body offset (mm)
	int TraslacionX=0;
	int TraslacionY=0;
	int TraslacionZ=0;
	float bodyRotX = 0;             // body roll (rad)
	float bodyRotY = 0;             // body pitch (rad)
	float bodyRotZ = 0;             // body rotation (rad)
	bool servo_fail=false;


	//VARIABLES DE CONTROL DE MANDO  ##################################################################
	#define zumbador  11
	float nivel=(PI/6)/128; //2^7 niveles de comapración		 	//60º--> pi/3		//  Variable que indica el minimo valor en radianes que se puede girar
	PS2X ps2x; 	// Objeto ps2x controller
	byte error=0;
	byte vibrate=0;
	int  cont_gait=2;		// contador para seleccionar el tipo de gait// POR DEFECTO WAVE (0,1,2)
	int  cont_gait_submodo=1;		// contador para seleccionar el tipo de gait
	int  modo_gait=1;		// contador para seleccionar el tipo de gait
	bool modo_control=0;	// 0 movimiento cuerpo; 1 movimiento robot

	//GAIT MANIPULATION VARIABLES   ##################################################################
#define MOVING   ((Xspeed > 15 || Xspeed < -15) || (Yspeed > 15 || Yspeed < -15) || (Rspeed > 0.05 || Rspeed < -0.05))
#define STD_TRANSITION          32
	ik_req_t gaits  [6];
	 int Xspeed;                     // forward speed (mm/s)
	 int Yspeed;                     // sideward speed (mm/s)
	 float Rspeed;                   // rotation speed (rad/s)
	 bool parado=0;
	 float  multiplicador;

	 enum gait Current_Gait;

			 uint8_t desfase=0;
			 uint8_t pushSteps=0;                  // how much of the cycle we are on the ground
			 uint8_t stepsInCycle=0;               // how many steps in this cycle
			 uint8_t step=0;                       // current step
			 int8_t leg_step=0;				//step de la pata actual
			 uint16_t tranTime=0;
			 uint8_t liftHeight=0;
			 float cycleTime=0;                // cycle time in seconds (adjustment from speed to step-size)
			 uint8_t gaitLegNo[6];
			 float Veloc_X_max=0;
			 float Veloc_Y_max=0;
			 float  Veloc_Rot_max=0;

				//SERVO CONTROL VARIABLES   ##################################################################

			   uint8_t poseSize=18;
			 int agachados_ []={109,127,294,89,166,271,79,132,266,84,137,300,114,156,300,99,157,313};
			 int reposo_ []= {174,234,294,153,271,271,144,239,266,149,244,300,178,261,300,164,264,313};//REPOSO
			 int Servos_grupo1_up[]= {161,219,294,112,214,271,131,224,266,109,188,300,165,245,300,124,208,313};
			 int Servos_grupo2_up[]= {134,178,294,140,255,271,104,183,266,136,229,300,137,204,300,151,249,313};
			 int Servos_inicio[]={161,219,294,140,255,271,131,224,266,136,229,300,165,245,300,151,249,313};//en el suelo	(INICIO)
			 unsigned char interpolating=0;
			 unsigned int  pose_[18];                       // the current pose, updated by Step(), set out by Sync()
			 unsigned int  nextpose_[18];                   // the destination pose, where we put on load
			 uint8_t  id_[18];                        // servo id for this index
			 unsigned long lastframe_;                   // time last frame was sent out
			 int speed_[18];



#ifdef Debug_JoyStick
// joystick
int ly;
int lx;
int ry;
int rx;
#endif





#ifdef DEBUG_IK
int time_test_IK;
#endif

void Select_Gait();	// borrar funcion??
int Ajuste_velocidad(uint8_t joystick);
float mapf(float val, float in_min, float in_max, float out_min, float out_max);
// NEW

//unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 1000;

unsigned long tiempo_sig = 0;
unsigned long tiempo_ant = 0;
unsigned long tiempo_pasado = 0;

unsigned long previousMillis = 0;        // will store last time LED was updated
int ledState = LOW;
// constants won't change:
const long duracion = 500;           // interval at which to blink (milliseconds)


bool mando_conectado=0;

//RESET***
void(* resetFunc) (void) = 0;
void setup(){
	// # SETUP GPIO # //
	pinMode(red_Pin,OUTPUT);// LED ROJO
	pinMode(green_Pin,OUTPUT);// LED  VERDE
	pinMode(blue_Pin,OUTPUT);// LED AZUL
	pinMode(zumbador, OUTPUT);// Zumbador
	pinMode(Mosfet, OUTPUT);// Mosfet
	pinMode(4, OUTPUT);// LED PARA MANDO

	// # SETUP COMMUNICACION PROTOCOL # //
	Serial.begin(115200);//serial(baudios)
	Wire.setClock(400000);//velocidad i2c
	// # SETUP PWM CONTROLLERS # //
	pwm.begin();
	pwm.setPWMFreq(50);
	pwm2.begin();
	pwm2.setPWMFreq(50);


	// # SETUP MANDO PS2 # //

	error = ps2x.config_gamepad(8,12,A0,13, false, false);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error

	   if(error == 0)
	      Serial.println("Found Controller, configured successful");
	   else if(error == 1)
	   Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
	   else if(error == 2)
	   Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
	   else if(error == 3)
	   Serial.println("Controller refusing to enter Pressures mode, may not support it. ");


// ******************************************************************************************************//


delay(200);

	   control_bateria();


 // ******************************************************************************************************//
//CONFIGURAMOS LOS VALORES INICIALES DE LA CINEMATICA Y LOS VALORES DE LOS SERVOS
		setupIK();
		Setup_Servo();
/*
//MANDAMOS LA POSICIÓN DE REPOSO AL ROBOT
 	 Escribir_posicion();		// ESCRIBIMOS POSICION DE REPOSO QUE VIENEN COMO DEFAULT EN BIOLOID CONTROLLER
 	 delay(2000);//esperamos a que se ubiquen los servos


//Se calcula la cinemática inversa para poner a los servos en la posición inicial para el movimiento
	doIK();
	Interpolate_Setup(1500);//  esta primera interpolación la hacemos lentamente: 1.5 segundos
	while(interpolating > 0){ // INTERPOLAMOS HASTA LA POSICION DE INICIO, DESPACIO
		Interpolate_Step();
	}
*/
		Inicializacion_servos ();

	/*	SELECCIÓN DEL MODO	*/
	gaitSelect(wave_12);
	modo_control=1; // MOVIMIENTO ROBOT

	// velocidad



	//float t3=millis();
	//Serial.print("Tiempo de ejecucion doIK: ");
	//Serial.println((t2-t1));
	//Serial.println("Tiempo de ejecucion interpolate setup: ");//46ms
	//Serial.println((t3-t2));
		//tiempo_pasado = millis();
	//	Serial.print("freeMemory()=");        Serial.println(freeMemory());
		tiempo_ant=millis();

}
void loop(){


	if(1000<(millis()-tiempo_ant)){ // Revisión periódica del estado de la batería (cada segundo)
		tiempo_ant=millis();
	control_bateria();
	//Serial.print("Altura: ");
	//Serial.println(TraslacionZ-  endpoints[0].z);
	//Serial.print("Altura Paso: ");
	//Serial.println(liftHeight);
/*
	Serial.print("Traslacion X: ");
	Serial.println(TraslacionX);
	Serial.print("Traslacion Y: ");
	Serial.println(TraslacionY);
	Serial.print("Traslacion Z: ");
	Serial.println(TraslacionZ);
	*/
	}

/*
	 tiempo_sig = millis();
	 tiempo_pasado = tiempo_sig- tiempo_ant;
	 tiempo_ant=tiempo_sig;
	 Serial.print("Tiempo pasado:  ");
     Serial.println(tiempo_pasado);
     */

ps2x.read_gamepad(false, vibrate); 	// LEE LOS COMANDOS RECIBIDOS


	if (ps2x.NewButtonState()) { // SOLO PARA PULSACIÓN DE BOTONES
		//will be TRUE if any button changes state (on to off, or off to on)

		if(ps2x.Button(PSB_START)){
				if(modo_control==1){ modo_control=0;  pi_pi_();	setupIK(); Gait_body_config(); Serial.println("Control del cuerpo");}
				else{				 modo_control=1;	 pi_pi_();		 setupIK(); Serial.println("Movimiento del robot");}

		}


		//************** BOTONES MODO DESPLAZAMIENTO ROBOT***************
		if (modo_control==1){

			 if(ps2x.Button(PSB_SELECT)){ //seleccion de gait
				 setupIK();
				cont_gait++;
				if(cont_gait>2)cont_gait=0;
				 cont_gait_submodo=1;// cada vez que se pulsa select se empieza por el primer submodo
				 Select_Gait();
				 pi_pi_();
			}
			else if(ps2x.Button(PSB_PAD_RIGHT)){ // aumentamos el submodo
				Serial.println("pad derecha");
				setupIK();
				cont_gait_submodo++;
				if((cont_gait_submodo>3)||(modo_gait==9)) cont_gait_submodo=1;
				Select_Gait();
				 pi_pi_();
			}
			else if(ps2x.Button(PSB_PAD_LEFT)){ // decrementamos el submodo
				Serial.println("pad izquierda");
				setupIK();
				cont_gait_submodo--;
				if(cont_gait_submodo<=0)cont_gait_submodo=1;
				Select_Gait();
				 pi_pi_();
			}
			else if(ps2x.Button(PSB_L1)){liftHeight+=5;}// AUMENTA ALTURA PASO

			else if(ps2x.Button(PSB_L2)){liftHeight-=5;} 	//	DECREMENTAMOS ALTURA PASO

			else  if(ps2x.Button(PSB_R1)){ 	//SUBE R1
					Serial.println("SUBE ALTURA");
					  for(int i=0;i<6;i++){
						  endpoints[i].z += -5;
							if(endpoints[i].z<=-140) endpoints[i].z=-140;// H max 140 mm
					  }
			}
			else if(ps2x.Button(PSB_R2)){ 	//BAJA R2
				Serial.println("BAJA ALTURA");
				  for(int i=0;i<6;i++){
					  endpoints[i].z += 5;
						if(endpoints[i].z>=-90) endpoints[i].z=-90;// H min 90 mm
				  }
			}
		}

		//********************BOTONES MODO MOVIMIENTO CUERPO********************
		if (modo_control==0){

				//desplazamientos del cuerpo del robot
			 if(ps2x.Button(PSB_TRIANGLE)){		//TRIANGULO
				Serial.println("Triangulo pulsado: y -- (Y Decrementa)");
				TraslacionY=TraslacionY-5;
			}
			else if(ps2x.Button(PSB_CROSS)){	//EQUIS
				Serial.println("equis pulsada: z ++ (H Baja)");
				TraslacionY = TraslacionY+5;
			}
			else if(ps2x.Button(PSB_CIRCLE)){	//CIRCULO
				Serial.println("Circulo pulsado: y ++ (Y Incrementa)");
				TraslacionX=TraslacionX-5;
			}
			else if(ps2x.Button(PSB_SQUARE)){ 	//CUADRADO
				Serial.println("cuadrado pulsado: z -- (H Sube)");
				TraslacionX = TraslacionX+5;
			}
			 if(ps2x.Button(PSB_R1)){ 	//SUBE R1
				Serial.println("SUBE ALTURA");
				TraslacionZ = TraslacionZ+5;
				if(TraslacionZ>=20) TraslacionZ=20;// H max 140 mm
					}
			else if(ps2x.Button(PSB_R2)){ 	//BAJA R2
				Serial.println("BAJA ALTURA");
				TraslacionZ = TraslacionZ-5;
				if(TraslacionZ<=-10) TraslacionZ=-10;// H min 90 mm
					}
		}
	}// NEW BUTTONSTATE

	if (modo_control==0){ //CONTROL CUERPO

		//	JOYSTICK IZQUIERDO
		if((ps2x.Read_Joystick(PSS_LX)) > 15 || ((ps2x.Read_Joystick(PSS_LX)) < -15) ){		//	EJE X
			bodyRotY =-nivel*ps2x.Read_Joystick(PSS_LX);
			//Serial.print("rotación sobre eje Y: ");
		//	Serial.println(bodyRotY*RAD_TO_DEG);
		}else{	bodyRotY = 0;}

		//	JOYSTICK IZQUIERDO
		if((ps2x.Read_Joystick(PSS_LY)) > 15 || ((ps2x.Read_Joystick(PSS_LY)) < -15) ){		//	EJE Y
			bodyRotX = -nivel*ps2x.Read_Joystick(PSS_LY);
			//Serial.print("rotación sobre eje X: ");
			//Serial.println(bodyRotX);
		}else{	bodyRotX = 0;}
		//	JOYSTICK DERECHO
		//	GIRO Z
		if((ps2x.Read_Joystick(PSS_RX)) > 15 || ((ps2x.Read_Joystick(PSS_RX)) < -15) ){
			bodyRotZ =- nivel*ps2x.Read_Joystick(PSS_RX);
		//	Serial.print("rotación sobre eje Z: ");
		//	Serial.println(bodyRotZ);
		}else{	bodyRotZ = 0;}
	}

	if (modo_control==1){ // MOVIMIENTO ROBOT
		//	JOYSTICK IZQUIERDO
		//	EJE X
		if((ps2x.Read_Joystick(PSS_LX)) > 15 || ((ps2x.Read_Joystick(PSS_LX)) < -15) ){
			Ajuste_velocidad(PSS_LX);
		//	Serial.print("Xspeed: ");
		//	Serial.println(Xspeed);
		}else Xspeed=0;

		//	JOYSTICK IZQUIERDO
		//	EJE Y
		if((ps2x.Read_Joystick(PSS_LY)) > 15 || ((ps2x.Read_Joystick(PSS_LY)) < -15) ){
			Ajuste_velocidad(PSS_LY);
		//	Yspeed = multiplicador*ps2x.Read_Joystick(PSS_LY);
			//Serial.print("Yspeed: ");
		//	Serial.println(Yspeed);
		}else Yspeed=0;
		//	JOYSTICK DERECHO
		//	GIRO Z
		if((ps2x.Read_Joystick(PSS_RX)) > 15 || ((ps2x.Read_Joystick(PSS_RX)) < -15) ){
			Ajuste_velocidad(PSS_RX);
			//Rspeed = -nivel*ps2x.Read_Joystick(PSS_RX);
		//	Serial.print("Rspeed: ");
		//	Serial.println(Rspeed);
		}else Rspeed=0;

	}


#ifdef Debug_JoyStick
	ry=ps2x.Read_Joystick(PSS_RY);
	rx=ps2x.Read_Joystick(PSS_RX);
	ly=ps2x.Read_Joystick(PSS_LY);
	lx=ps2x.Read_Joystick(PSS_LX);

	Serial.print("LY= ");
	Serial.print(ly);
	Serial.print("	LX= ");
	Serial.print(lx);

	Serial.print("RY= ");
	Serial.print(ry);
	Serial.print("	RX= ");
	Serial.println(rx);
#endif

// final del loop

	if(interpolating== 0){
			//	tiempo_ant=millis();
				doIK();
				Interpolate_Setup(tranTime);
			}
			// update joints
			Interpolate_Step();
}


int Ajuste_velocidad(uint8_t joystick){

	if (joystick==7){//RIGHT
		Xspeed=-map(ps2x.Read_Joystick(PSS_LX),-128,127,-Veloc_X_max,Veloc_X_max);
		return Xspeed;

	}else if (joystick==8){// LEFT
		Yspeed=map(ps2x.Read_Joystick(PSS_LY),-128,127,-Veloc_Y_max,Veloc_Y_max);
		return Yspeed;

	}else if (joystick==5){// LEFT
		Rspeed=-mapf(ps2x.Read_Joystick(PSS_RX),-128,127,-Veloc_Rot_max,Veloc_Rot_max);
		//Serial.print("Rspeed: ");
		//Serial.println(Rspeed,4);
		return Rspeed;

	}else {
		return 0;
	}

}


void Select_Gait(){



				 					modo_gait=cont_gait*3+cont_gait_submodo;
				 				Serial.println("Seleccion de gait: ");
				 				Serial.println(modo_gait);
				 				switch(modo_gait) {
				 						      case 1 :
				 						    	  gaitSelect(ripple_6);
				 						    	  Serial.println("ripple_6");
				 						         break;
				 						      case 2 :
				 						    	  gaitSelect(ripple_12);
				 						    	  Serial.println("ripple_12");
				 						    	  break;
				 						      case 3 :
				 						    	  gaitSelect(ripple_24);
				 						    	  Serial.println("ripple_24");
				 						         break;
				 						      case 4 :
				 						    	  gaitSelect(tripod_6);
				 						    	  Serial.println("tripod_6");
				 								 break;
				 						      case 5 :
				 						    	  gaitSelect(tripod_12);
				 						    	  Serial.println("tripod_12");
				 								 break;
				 						      case 6 :
				 						    	  gaitSelect(tripod_24);
				 						    	  Serial.println("tripod_24");
				 								 break;
				 						      case 7 :
				 								  gaitSelect(wave_12);
				 								  Serial.println("wave_12");
				 								 break;
				 						      case 8 :
				 								  gaitSelect(wave_24);
				 								  Serial.println("wave_24");
				 								 break;
				 						      case 9 :
				 								  gaitSelect(wave_24);
				 								  Serial.println("wave_24");
				 								 break;

				 						      default:
				 						    	  Serial.println("default mode: nothing");
				 						    	 break;
				 			}
}


void control_bateria(){

	for (int i=0;i<50;i++){

	voltaje=analogRead(A1);
		sumatorio+=voltaje;
	}
	voltaje=sumatorio/50;
	sumatorio=0;

	voltaje_real=m*voltaje+n;

    porcentaje=map(voltaje_real,62e5,84e5,0,100)+5;	//por el pico de consumo de corriente se le suma 4% mas

#ifdef DEBUG
  Serial.print("voltaje analogico:  ");
  Serial.print(voltaje);
  Serial.print("   voltaje real:  ");
  Serial.print(voltaje_real);
  Serial.print("  porcentaje:  ");
   Serial.println(porcentaje);
#endif

  // El voltaje mínimo en una celda debe ser de 3.0V
   //El voltaje máximo en una celda de 4.2V

		if (porcentaje>=25)	set_color(0,255,0);	 // VERDE
		//digitalWrite(Mosfet, HIGH);
		if ((porcentaje<25)&&(porcentaje>15)) set_color(255,100,0); //AMARILLO
		//digitalWrite(Mosfet, HIGH);
		if ((porcentaje<15)&&(porcentaje>=5)) set_color(255,0,0); //ROJO
		//digitalWrite(Mosfet, HIGH);
//	AVISOS SONOROS CUANDO ESTA EN ROJO?
		if (porcentaje<5) { //EL ROBOT SE TIENE QUE APAGAR
			// este color era indicativo para las pruebas, EN LA VERSIÖN FINAL DESACTIVA EL MOSFET PARA APAGAR EL ROBOT
			set_color(0,0,255);
			//si llegase a este if se
			// aunque por defecto está a LOW
			//digitalWrite(Mosfet, LOW);
			encendido=false;// el robot se apaga
			//APAGAR ROBOT
			 // Serial.println("  apagado:  ");
			   //Serial.println(porcentaje);
		}

		   if (encendido==false){		   // SI NO HAY SUFICIENTE VOLTAJE EN LA BATERÍA, EL PROGRAMA  NO SE INICIARÁ
			   digitalWrite(Mosfet, LOW);
			   while(1);// se queda en bucle, hay que desconectar la bateria, CARGARLA,  y volverla a conectar
		   }else digitalWrite(Mosfet, HIGH);// SI LA HAY SE ACTIVA EL MOSFET Y SE INICIA EL ROBOT

}



void set_color (int red,int green, int blue){

	analogWrite(red_Pin,red);
	analogWrite(green_Pin,green);
	analogWrite(blue_Pin,blue);

}



void pi_pi_(){
// ESTO ES UN PII PII ---------->   |~~|__|~~|______

pi_();
delay(50);
pi_();
//Y YA HABRIA QUE DEJARLO APAGADO SIEMPRE HASTA LA SIGUIENTE LLAMADA
}

void pi_(){
	// ESTO ES UN PII ---------->   |~~|______

tone(zumbador, 2000, 1000);//encendido
delay(50);
tone(zumbador, 0, 1000);//apagado

//Y YA HABRIA QUE DEJARLO APAGADO SIEMPRE HASTA LA SIGUIENTE LLAMADA
}

float mapf(float val, float in_min, float in_max, float out_min, float out_max) {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
