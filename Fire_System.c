
#include "Fire_System.h"
#include "Fire_System_Private.h"

static double angle = -90;

static u8 passward[20] = {0};
u8 passwardSize = sizeof(passward)/sizeof(passward[0]);

static u8 newPassward[20] = {NULL};
static u8 newPasswardSize = sizeof (newPassward)/sizeof (newPassward[0]);

static u8 key = NULL;

static u8 screen = Fire_System_NORMAL_SCREEN;

static u8 changePasswardMode = 0;

static u8 getPasswardMode = 0;

extern void Fire_System_Init ()
{
	getPassward_Init ();
	DIO_Init();
	LCD_Init();
	
	changePassward_Init(newPassward, newPasswardSize);
}

extern void Fire_System_Run ()
{
	Fire_System_getKey ();
	
	Fire_System_getPassward ();
	
	Fire_System_changePassward ();
	
	Fire_System_State_WrongPassword ();
}

static void Fire_System_servo ()
{
	static u8 servoDirection = 0;
	
	if (servoDirection == 0)
	{
		angle += 1;
		if (angle == 90)
		{
			servoDirection = 1;
		}
	}
	else if (servoDirection == 1)
	{
		angle -= 1;
		if (angle == -90)
		{
			servoDirection = 0;
		}
	}
	
	SERVO_setAngle (angle);
}

static void Fire_System_getKey ()
{
	if (screen == Fire_System_NORMAL_SCREEN)
	{
		if (Fire_System_trigger ())
		{
			if (key == '1')
			{
				passward[0] = NULL;
				LCD_GoTo(0, 0);
				LCD_WriteString("Enter Passward:");
				screen = Fire_System_GET_PASSWARD_SCREEN;
				getPasswardMode = 1;
			}
			else if (key == '2')
			{
				passward[0] = NULL;
				screen = Fire_System_GET_PASSWARD_SCREEN;
				changePasswardMode = 1;
			}
		}
	}
}

static void Fire_System_getPassward ()
{
	if (getPasswardMode == 1)
	{
		if (screen == Fire_System_GET_PASSWARD_SCREEN)
		{
			if (getPassward_Run (passward, passwardSize) == GET_PASSWARD_WAIT)
			{
				LCD_GoTo(1, 0);
				LCD_WriteString(passward);
				LCD_WriteString("				");
			}
			else
			{
				if (Fire_System_CompareString (newPassward, passward))
				{
					screen = Fire_System_NORMAL_SCREEN;
				}
				else
				{
					screen = Fire_System_Error_SCREEN;
				}
				
				getPasswardMode = 0;
				LCD_Clear();
			}
		}
	}
}

static void Fire_System_changePassward ()
{
	if (changePasswardMode == 1)
	{
		if (screen == Fire_System_GET_PASSWARD_SCREEN)
		{
			if (getPassward_Run (passward, passwardSize) == GET_PASSWARD_WAIT)
			{
				LCD_GoTo(1, 0);
				LCD_WriteString(passward);
				LCD_WriteString("				");
			}
			else
			{
				screen = Fire_System_CHANGE_PASSWARD_SCREEN;
				LCD_Clear();
			}
		}
		
		if (screen == Fire_System_CHANGE_PASSWARD_SCREEN)
		{
			if (Fire_System_CompareString (newPassward, passward))
			{
				if (changePassward_Run(newPassward, newPasswardSize) == CHANGE_PASSWARD_DONE)
				{
					screen = Fire_System_NORMAL_SCREEN;
					changePasswardMode = 0;
					LCD_Clear();
				}
			}
			else
			{
				screen = Fire_System_Error_SCREEN;
				changePasswardMode = 0;
				LCD_Clear();
			}
		}
	}
}

static void Fire_System_State_WrongPassword ()
{
	if (screen == Fire_System_Error_SCREEN)
	{
		LCD_GoTo(0, 0);
		LCD_WriteString("Wrong Password!!");
		Fire_System_delay_ms( 3, Fire_System_periodically);
		LCD_Clear();
		screen = Fire_System_NORMAL_SCREEN;
	}
}

static void Fire_System_periodically ()
{
	Fire_System_servo ();
}

static void Fire_System_delay_ms(u32 delay, void (*Func)(void))
{
	for (u32 index = 0; index < delay*1000; index++)
	{
		Func ();
		
		_delay_us(1);
		
		//if (fireMode == 0)
		//{
			//break;
		//}
	}
}

static u8 Fire_System_trigger ()
{
	static u8 lastKey = NULL;
	
	lastKey = key;
	
	key = KEYPAD_GetKey ();
	
	if (lastKey != key)
	{
		return 1;
	}
	
	return 0;
}

static u8 Fire_System_CompareString (char* string1, char* string2)
{
	for (u32 index = 0; string1[index]; index++)
	{
		if (string1[index] != string2[index])
		{
			return 0;
		}
	}
	
	return 1;
}