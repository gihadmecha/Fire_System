


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
#define  PASSWARD			2

#define		Fire_System_NORMAL_SCREEN				0
#define		Fire_System_GET_PASSWARD_SCREEN			1
#define		Fire_System_CHANGE_PASSWARD_SCREEN		2
#define		Fire_System_Error_SCREEN				3

static void Fire_System_servo ();
static void Fire_System_getKey ();
static void Fire_System_getPassward ();
static void Fire_System_changePassward ();
static void Fire_System_State_WrongPassword ();
static void Fire_System_periodically ();
static void Fire_System_delay_ms(u32 delay, void (*Func)(void));
static u8 Fire_System_trigger ();
static u8 Fire_System_CompareString (char* string1, char* string2);



#endif /* FIRE_SYSTEM_PRIVATE_H_ */