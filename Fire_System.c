
#include "Fire_System.h"
#include "Fire_System_Private.h"

static u8 passward[20] = {0};
u8 passwardSize = sizeof(passward)/sizeof(passward[0]);

static u8 newPassward[20] = {NULL};
static u8 newPasswardSize = sizeof (newPassward)/sizeof (newPassward[0]);

static u8 key = NULL;

static u8 screen = Fire_System_NORMAL_SCREEN;

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
			}
			else if (key == '2')
			{
				screen = Fire_System_CHANGE_PASSWARD_SCREEN;
			}
		}
	}
}

static void Fire_System_getPassward ()
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
			screen = Fire_System_NORMAL_SCREEN;
			LCD_Clear();
		}
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