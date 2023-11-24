


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

#define		Fire_System_NORMAL_SCREEN			0
#define		Fire_System_GET_PASSWARD_SCREEN			1
#define		Fire_System_CHANGE_PASSWARD_SCREEN		2

static u8 Fire_System_trigger ();
static void Fire_System_getKey ();
static void Fire_System_getPassward ();



#endif /* FIRE_SYSTEM_PRIVATE_H_ */