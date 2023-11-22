


#ifndef FIRE_SYSTEM_PRIVATE_H_
#define FIRE_SYSTEM_PRIVATE_H_

#include "LCD.h"
#include "LM35.h"

#include "LED.h"
#include "KEYPAD.h"
#include "STEPPER_MOTOR.h"
#include "SERVO.h"
#include "MQ_2.h"



#define  NORMAL				0
#define	 CHANGE_PASSWARD	1


static void Fire_System_State_Fine ();
static void Fire_System_State_Heat ();
static void Fire_System_State_Fire ();
static void Fire_System_GetPassward ();
static u8 Fire_System_CompareString (char* string1, char* string2);
static void Fire_System_State_WrongPassword ();
static void Fire_System_CheckPeriodically ();
static void Fire_System_delay_ms(u32 delay);
static void Fire_System_changePassward ();



#endif /* FIRE_SYSTEM_PRIVATE_H_ */