/* Encoder Library - Basic Example
   http://www.pjrc.com/teensy/td_libs_Encoder.html

   This example code is in the public domain.
*/

#include "motor.h"
#include "controller.h"
#include "encoder.h"
#include "comm.h"

#include <DueTimer.h>


/***************************************** 
  PROBLEMS WITH LIBRARY IMPORT ON ARDUINO.
  THEY ARE PLACED HERE FOR SIMPLICITY 
  ****************************************/

/** CONTROLLER.h **/
#ifdef __cplusplus
extern "C" {
#endif
void CONTROLLER_Start      ( long l_sampling_time );
#ifdef __cplusplus
} // extern "C"
#endif
/** END CONTROLLER.h **/

/** COMMUNICATION.h **/
#ifdef __cplusplus
extern "C" {
#endif
void COMMUNICATION_Start      ( long l_sampling_time );
#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
extern "C" {
#endif
void COMMUNICATION_Stop      ( void );
#ifdef __cplusplus
} // extern "C"
#endif
/** END COMMUNICATION.h **/
/***************************************** 
  END PROBLEMS WITH LIBRARY IMPORT ON ARDUINO.
  THEY ARE PLACED HERE FOR SIMPLICITY 
  ****************************************/


/* FUNCTIONS definition */
void COMMUNICATION_ROB ( void );
void CONTROLLER_ROB ( void );

/* VARs definition */
#define CONTROLLER_SAMPLING_PERIOD    5   // in mSec
#define COMMUNICATION_SAMPLING_PERIOD 10  // in mSec


/* USER vars definition */
float     fPos;
float     fSpeed;
uint32_t  uControllerCounter;
uint32_t  uCommCounter;
float r;
float pos_real;
float speed_real;
float torque;
float m;
float g;
float lc;
float nu;
float Kd;
float V;
float u;
float x;
float Rm; 
float Km;
/* END USER vars definition */

void setup() 
{

  /* INIT USER vars */
  r=1/9.68; //Reduction ratio
  fPos=0.0;
  pos_real=0.0;
  fSpeed = 0.0;
  speed_real=0.0;
  uControllerCounter  = 0;
  uCommCounter  = 0;
  m=0.037; //kg
  g=9.8;
  lc=0.08; //m
  nu=1;
  Rm=16; 
  Km=0.0235;
  u=0;
  /* End INIT USER vars */

  /* INIT ENCODER */
  /* Pos */
  ENCODER_Init();
  /* Speedeegi */
  ENCODER_StartSpeedCalculation(ENCODER_SPEED_CALCULATION_SAMPLING_PERIOD);

  /* INIT MOTOR  */
  MOTOR_Init();

  /* INIT COMMUNICATION  */
  COMMUNICATION_Init(115200);
  
  CONTROLLER_Start(CONTROLLER_SAMPLING_PERIOD);
  COMMUNICATION_Start(COMMUNICATION_SAMPLING_PERIOD);
}

/******************************
*******************************/

void CONTROLLER_ROB ( void )
{
  /* START CODE TO BE IMPLEMENTED BY THE STUDENTS */
fPos = (float) ENCODER_GetCount();
  pos_real = (fPos*7.5)*r; 
  fSpeed  = (float) ENCODER_GetCountInSpeedInterval();
  speed_real= (fSpeed*7.5)*r;

  torque= m*g*lc*sin(2*M_PI/360*pos_real);
  u=r*torque;
  V=Rm/Km*u;
  
 /* if ( uControllerCounter < 200 )
    MOTOR_SetVoltage(4);
      
  else if ( uControllerCounter < 400 )
    MOTOR_SetVoltage(-4);*/
  
    MOTOR_SetVoltage(-V);

  uControllerCounter++;
  /* END CODE TO BE IMPLEMENTED BY THE STUDENTS */
}

/******************************
*******************************/

void COMMUNICATION_ROB ( void )
{
  /* START CODE TO BE IMPLEMENTED BY THE STUDENTS */
  /* Print Comm time */
  Serial.print("Count: ");
  Serial.print(uCommCounter);
  /* Print pos in pulses */
  Serial.print(" Pos: ");
  Serial.print(pos_real,5);
  /* Print vel in pulses */
  Serial.print(" Vel: ");
  Serial.println(speed_real,5);
  Serial.print(" V: ");
  Serial.println(V);
 /* if (uControllerCounter > 450)
   COMMUNICATION_Stop();
  */
  uCommCounter++;
  /* END CODE TO BE IMPLEMENTED BY THE STUDENTS */
}

/******************************
*******************************/

void loop()
{
}


/******************************
*******************************/


/***************************************** 
  PROBLEMS WITH LIBRARY IMPORT ON ARDUINO.
  THEY ARE PLACED HERE FOR SIMPLICITY 
  ****************************************/

/** CONTROLLER.c **/
/* Input in mSec */
void CONTROLLER_Start ( long l_sampling_time )
{
  Timer3.attachInterrupt(CONTROLLER_ROB);
  Timer3.start(l_sampling_time * 1000); // Timer defined in uSec

}
/** END CONTROLLER.c **/

/** COMMUNICATION **/
void COMMUNICATION_Start ( long l_sampling_time )
{
  Timer2.attachInterrupt(COMMUNICATION_ROB);
  Timer2.start(l_sampling_time * 1000);
}

void COMMUNICATION_Stop ( )
{
  Timer2.stop();
}
/** END COMMUNICATION **/

/** ENCODER **/
/* Input in mSec */
void ENCODER_StartSpeedCalculation ( long l_sampling_time )
{
  Timer1.attachInterrupt(ENCODER_CalcSpeed);
  Timer1.start(l_sampling_time * 1000); // Timer defined in uSec
}
/** END ENCODER **/
/***************************************** 
  END PROBLEMS WITH LIBRARY IMPORT ON ARDUINO.
  THEY ARE PLACED HERE FOR SIMPLICITY 
  ****************************************/
