
#include "Fire_System_Private.h"
#include "Fire_System.h"

static u16 temperature;
static u16 smoke;

static u8 lastMode = Fire_System_NORMAL;

static u8 fireMode = 0;

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
	ADC_Init(ADC_VCC, ADC_PRESCALER_64);
	TIMER1_ICR1_WRITE(TIMER1_TOP_VALUE);
	SERVO_setAngle(angle);
	TIMER1_Init(TIMER1_PRESCALER_8, TIMER1_FAST_PWM_ICR1_TOP, TIMER1_OC1A_CLEAR_ON_COMPARE_MATCH, TIMER1_OC1B_DISCONNECTED);
	LCD_Init();
	
	ADC_Enable();
	
	
	//Fire_System_State_Fine ();
	
	changePassward_Init(newPassward, newPasswardSize);
}

extern void Fire_System_Run ()
{
	temperature = LM35();
	
	if (fireMode == 0)
	{
		if (temperature <= Fire_System_FINE_DEGREE)
		{
			Fire_System_State_Fine();
			
			lastMode = Fire_System_NORMAL;
		}
		else if (temperature > Fire_System_HEAT_DEGREE)
		{
			smoke = MQ_2();
			if (smoke <= 50)
			{
				Fire_System_State_Heat ();
				
				lastMode = Fire_System_HEAT;
			}
			else if (smoke > 50)
			{
				Fire_System_State_Fire();
			}
		}
		else
		{
			if (lastMode == Fire_System_NORMAL)
			{
				Fire_System_State_Fine();
			}
			else if (lastMode == Fire_System_HEAT)
			{
				Fire_System_State_Heat ();
			}
		}
	}
	else if (fireMode == 1)
	{
		smoke = MQ_2();
	
		Fire_System_State_Fire();
	}
	
	Fire_System_getKey ();
	
	Fire_System_getKey ();
	
	Fire_System_getPassward ();
	
	Fire_System_changePassward ();
	
	Fire_System_State_WrongPassword ();
}

static void Fire_System_State_Fine ()
{
	smoke = 0;

	LED1_Off();
	LED2_Off();
		
	if (screen == Fire_System_NORMAL_SCREEN)
	{
		LCD_GoTo(0, 0);
		LCD_WriteString("Fine");
		LCD_GoTo(0, 6);
		LCD_WriteString("Temp:");
		LCD_WriteNumber_4Digit(temperature);
		LCD_GoTo(1, 6);
		LCD_WriteString("Smok:");
		LCD_WriteNumber_4Digit(smoke);
	}
}

static void Fire_System_State_Heat ()
{
	LED1_On();
	LED2_Off();
		
	if (screen == Fire_System_NORMAL_SCREEN)
	{
		LCD_GoTo(0, 0);
		LCD_WriteString("Heat");
		LCD_GoTo(0, 6);
		LCD_WriteString("Temp:");
		LCD_WriteNumber_4Digit(temperature);
		LCD_GoTo(1, 6);
		LCD_WriteString("Smok:");
		LCD_WriteNumber_4Digit(smoke);
	}
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

static void Fire_System_State_Fire ()
{
	fireMode = 1;
		
	LED1_On();
	LED2_On();
		
	Fire_System_servo();
		
	if (screen == Fire_System_NORMAL_SCREEN)
	{
		LCD_GoTo(0, 0);
		LCD_WriteString("Fire");
		LCD_GoTo(0, 6);
		LCD_WriteString("Temp:");
		LCD_WriteNumber_4Digit(temperature);
		LCD_GoTo(1, 6);
		LCD_WriteString("Smok:");
		LCD_WriteNumber_4Digit(smoke);
	}
}

static void Fire_System_getKey ()
{
	if (screen == Fire_System_NORMAL_SCREEN)
	{
		if (Fire_System_trigger ())
		{
			if (key == '1')
			{
				LCD_Clear();
				passward[0] = NULL;
				LCD_GoTo(0, 0);
				LCD_WriteString("Enter Passward:");
				screen = Fire_System_GET_PASSWARD_SCREEN;
				getPasswardMode = 1;
			}
			else if (key == '2')
			{
				LCD_Clear();
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
					fireMode = 0;
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
					passward[0] = NULL;
					LCD_Clear();
				}
			}
			else
			{
				passward[0] = NULL;
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
		
		if (fireMode == 0)
		{
			break;
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