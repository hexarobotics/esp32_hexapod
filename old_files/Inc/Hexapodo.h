// Gaits.h

#ifndef Hexapodo_h
#define Hexapodo_h

#include "stdint.h"
//#include "PS3BT.h"
//#include "usbhub.h"
//#include "SPI.h"
//#include "Wire.h"
#include "Adafruit_PWMServoDriver.h"

/* 	    ---				####			GLOBAL VARIABLES		####			------
*	    ---------------------------------------------------------------------	   *\

// 		### 	PWM CONTROL OBJET  		### 		//

// Creamos los objetos para el control del PWM de manera GLOBAL para que puedan ser utilizados por las librerias
/Servos del 1 al 9  controlador de direcci�n: 0x42     Servos del 10 al 18 controlador de direcci�n: 0x44*/
extern	Adafruit_PWMServoDriver pwm; //= Adafruit_PWMServoDriver(0x42);
extern	Adafruit_PWMServoDriver pwm2;// = Adafruit_PWMServoDriver(0x44);

//	 ****************************************		//


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
/*
#define RIGHT_FRONT    3
#define RIGHT_REAR     5
#define LEFT_FRONT    0
#define LEFT_REAR      2
#define RIGHT_MIDDLE   4
#define LEFT_MIDDLE    1
*/

// antiguo
#define RIGHT_FRONT    3
#define RIGHT_REAR     5
#define LEFT_FRONT     0
#define LEFT_REAR      2
#define RIGHT_MIDDLE   4
#define LEFT_MIDDLE    1

/* Servo Legs*/
#define RF_COXA 1
#define RF_FEMUR 2
#define RF_TIBIA 3
//
#define RR_COXA 4
#define RR_FEMUR 5
#define RR_TIBIA 6
//
#define LF_COXA 7
#define LF_FEMUR 8
#define LF_TIBIA 9
//
#define LR_COXA 10
#define LR_FEMUR 11
#define LR_TIBIA 12
//
#define RM_COXA 13
#define RM_FEMUR 14
#define RM_TIBIA 15
//
#define LM_COXA 16
#define LM_FEMUR 17
#define LM_TIBIA 18

typedef struct 
{
	int min;
	int max;
	uint8_t Cero;
}MMC;//minimo maximo offset

typedef struct
{
	MMC servo_coxa;
	MMC servo_femur;
	MMC servo_tibia;
} LEG;

typedef struct
{
	int coxa;
	int femur;
	int tibia;
} ik_sol_t;

typedef struct
{
	int x;
	int y;
	int z;
	float r;
} ik_req_t;

typedef struct
{
	int8_t x;
	int8_t y;
} vect_xy;

extern		LEG PATAS  [6]; 	// Variable muy importante: Vector de structs (cada pata), que a su vez tiene 3 structs, uno  para cada servomotor, y para cada uno de ellos un struct de 3 variables que indican el minimo, m�ximo y el delta_90
extern		ik_req_t endpoints  [6];	// vector de structs que guarda la posici�n x,y,z de cada pata
extern		vect_xy  Posicion_coxa[6];

extern		bool servo_fail;

#ifdef DEBUG
	char* String_Fails[]={"	RF_COXA FAIL: ", "	RF_FEMUR FAIL: ", "		RF_TIBIA FAIL: ","		RR_COXA FAIL: ", "		RR_FEMUR FAIL: ","		RR_TIBIA FAIL: ", "		LF_COXA FAIL: ", "	LF_FEMUR FAIL: ", "		LF_TIBIA FAIL: ","		LR_COXA FAIL: ", "		LR_FEMUR FAIL: ","		LR_TIBIA FAIL: ", "		RM_COXA FAIL: ","		RM_FEMUR FAIL: ", "		RM_TIBIA FAIL: ","		LM_COXA FAIL: ", "		LM_FEMUR FAIL: ","		LM_TIBIA FAIL: "};
#endif


		//***BODY_KINEMATICS***
extern		uint8_t bodyPosX ;               // body offset (mm)
extern		uint8_t bodyPosY ;               // body offset (mm)
extern		int TraslacionX;
extern		int TraslacionY;
extern		int TraslacionZ;
extern		float bodyRotX ;             // body roll (rad)
extern		float bodyRotY ;             // body pitch (rad)
extern		float bodyRotZ ;             // body rotation (rad)

// ###****NUEVA****###
//CONSTANTE PWM/GRADOS
const uint8_t  Constante_PWM=195;

//########################  DECLARACION FUNCIONES CINEMATICA ########################//

int radToServoV2(float rads);
int ServoToDEG(int servo_pwm);
ik_sol_t  Real_angle(uint8_t leg,ik_sol_t angulo);
/* find the translation of the coxa point (x,y) in 3-space, given our rotations */
ik_req_t bodyIK(uint8_t leg );
/* given our leg offset (x,y,z) from the coxa point, calculate servo values */
ik_sol_t legIK(int X, int Y, int Z, int leg);
/* ties all of the above together */
void doIK();
/* setup the starting positions of the legs. */
void setupIK();


//	 ****************************************		//


//	###			GAIT MANIPULATION VARIABLES		###		//
//	 ****************************************		//

extern		 int Xspeed;                     // forward speed (mm/s)
extern		 int Yspeed;                     // sideward speed (mm/s)
extern		 float Rspeed;                   // rotation speed (rad/s)
extern		 bool parado;
extern		 float  multiplicador;

enum gait
{
	ripple_6,
	ripple_12,
	ripple_24,
	wave_12,
	wave_24,
	tripod_6,
	tripod_12,
	tripod_24
};

extern enum gait Current_Gait;
extern		 uint8_t desfase;
extern		 uint8_t pushSteps;                  // how much of the cycle we are on the ground
extern		 uint8_t stepsInCycle;               // how many steps in this cycle
extern		 uint8_t step;                       // current step
extern		 int8_t leg_step;				//step de la pata actual
extern	 	uint16_t tranTime;
extern		 uint8_t liftHeight;
extern		 float cycleTime;                // cycle time in seconds (adjustment from speed to step-size)
extern		 uint8_t gaitLegNo[6];
extern		  ik_req_t gaits  [6];
extern		float Veloc_X_max;
extern		float Veloc_Y_max;
extern		float Veloc_Rot_max;

#define MOVING   ((Xspeed > 15 || Xspeed < -15) || (Yspeed > 15 || Yspeed < -15) || (Rspeed > 0.05 || Rspeed < -0.05))
/* Standard Transition time should be of the form (k*BIOLOID_FRAME_LENGTH)-1
*  for maximal accuracy. BIOLOID_FRAME_LENGTH = 33ms, so good options include:
*   32, 65, 98, etc...
*/
//#define STD_TRANSITION          98   //98 for ax-12 hexapod, 32 for ax-18f
//#define STD_TRANSITION          65
#define STD_TRANSITION          32
//ik_req_t *gaitGen;

//######################## 	 DECLARACION FUNCIONES GAIT 	 ########################//

void Gait_generator (uint8_t leg);
void gaitSelect(gait GaitType);
void Gait_body_config( );
void calculo_recorrido ();
//	*****************************************		//



//	###			SERVO CONTROL VARIABLES		###		//
//	 ****************************************		//

#define BIOLOID_FRAME_LENGTH      20  // 33 HA SIDO SIEMPRE
/* we need some extra resolution, use 13 bits, rather than 10, during interpolation */
extern   uint8_t poseSize;
extern int agachados_  [];//= {300,206,165,300,224,176,283,194,186,276,201,160,300,185,132,270,192,101};
extern int reposo_  [];// REPOSO NO SE USA
extern int Servos_grupo1_up [];
extern int Servos_grupo2_up [];
extern int Servos_inicio [];

extern unsigned char interpolating;                // are we in an interpolation? 0=No, 1=Yes
extern unsigned long tiempo_sig ;
extern unsigned long tiempo_ant ;
extern unsigned long tiempo_pasado ;
 // Adafruit_PWMServoDriver pwm;// = Adafruit_PWMServoDriver(0x42);
 // Adafruit_PWMServoDriver pwm2;// = Adafruit_PWMServoDriver(0x44);
extern unsigned int  pose_[];                       // the current pose, updated by Step(), set out by Sync()
extern unsigned int  nextpose_[];                   // the destination pose, where we put on load
extern uint8_t  id_[];                        // servo id for this index
extern unsigned long lastframe_;                   // time last frame was sent out
extern int speed_[];

//######################## 	 DECLARACION FUNCIONES CONTROLADOR SERVOS 	 ########################//

void Setup_Servo();
void Escribir_posicion();                           // write a pose out to the servos
void Guardar_posicion(int id, int pos);          // set a servo value in the next pose
void Interpolate_Setup(int time);            // calculate speeds for smooth transition
void Interpolate_Step();                     // move forward one step in current interpolation
void Inicializacion_servos ();
 	 	 	 	 	 	 	 	 	 //	*****************************************		//


		//	###			CONTROL INPUT  VARIABLES		###		//
		//    *************************************		//
		/*
	*USB Usb; //USBHub Hub1(&Usb); // Some dongles have a hub inside
	*BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
	* You can create the instance of the class in two ways
	*PS3BT PS3(&Btd); // This will just create the instance
	*PS3BT PS3(&Btd, 0x00, 0x15, 0x83, 0xEA, 0x4D, 0xDF); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch
	*/
//	bool printTemperature, printAngle;

//	uint8_t  multiplicador=0;
//	int Xspeed;                     // forward speed (mm/s)
//	int Yspeed;                     // sideward speed (mm/s)
//	float Rspeed;                   // rotation speed (rad/s)
extern	float nivel; //2^7 niveles de comapraci�n		 	//60�--> pi/3		//  Variable que indica el minimo valor en radianes que se puede girar

extern	bool modo_control;	// 0 movimiento cuerpo; 1 movimiento robot

	//	 ****************************************		//


#endif

