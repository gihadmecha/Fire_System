
#include "Fire_System_Private.h"
#include "Fire_System.h"

static u16 temperature;
static u16 smoke;

static u8 fireMode = 0;
static char key;
static char passward[100];
static unsigned int passwordIndex = 0;

static u8 mode = NORMAL;

static double angle = -90;

extern void Fire_System_Init ()
{
	DIO_Init();
	ADC_Init(ADC_VCC, ADC_PRESCALER_64);
	TIMER1_ICR1_WRITE(TIMER1_TOP_VALUE);
	SERVO_setAngle(angle);
	TIMER1_Init(TIMER1_PRESCALER_8, TIMER1_FAST_PWM_ICR1_TOP, TIMER1_OC1A_CLEAR_ON_COMPARE_MATCH, TIMER1_OC1B_DISCONNECTED);
	LCD_Init();
	
	ADC_Enable();
	
	Fire_System_State_Fine ();
	
	//STEPPER1_Stop ();
	
	changePassward_Init ();
}

extern void Fire_System_Run ()
{
	temperature = LM35();
	
	if (fireMode == 0)
	{
		if (temperature <= 400)
		{
			Fire_System_State_Fine();
		}
		else if (temperature > 450)
		{
			smoke = MQ_2();
			
			if (smoke <= 50)
			{
				Fire_System_State_Heat ();
			}
			else if (smoke > 50)
			{
				Fire_System_State_Fire();
			}
		}
	}
	else if (fireMode == 1)
	{
		smoke = MQ_2();
		
		Fire_System_State_Fire();
	}
	
	Fire_System_changePassward ();
}

static void Fire_System_changePassward ()
{
	if (mode == NORMAL)
	{
		key = KEYPAD_GetKey ();
		if (key == 'C')
		{
			mode = CHANGE_PASSWARD;
			LCD_Clear();
		}
	}
	
	if (mode == CHANGE_PASSWARD)
	{
		if (changePassward_Run () == 0)
		{
			mode = NORMAL;
			LCD_Clear();
		}
	}
}

static void Fire_System_State_Fine ()
{
	smoke = 0;

	LED1_Off();
	LED2_Off();
	
	if (mode == NORMAL)
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
	
	if (mode == NORMAL)
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

static void Fire_System_State_Fire ()
{
	static u8 servoDirection = 0;
	
	fireMode = 1;
	
	LED1_On();
	LED2_On();
	
	//STEPPER1_Forward (Fire_System_GetPassward);
	
	if (servoDirection == 0)
	{
		angle += 1;
		if (angle == 90)
		{
			servoDirection = 1;
		}
		SERVO_setAngle (angle);
	}
	else if (servoDirection == 1)
	{
		angle -= 1;
		if (angle == -90)
		{
			servoDirection = 0;
		}
		SERVO_setAngle (angle);
	}
	
	if (mode == NORMAL)
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

static void Fire_System_CheckPeriodically ()
{
	key = KEYPAD_GetKey();
	
	if (key != NULL)
	{
		if (key != '=')
		{
			LCD_WriteChar(key);
		}
		passwordIndex++;
		passward[passwordIndex] = key;
	}
}

static void Fire_System_GetPassward ()
{
	if (mode == NORMAL)
	{
		key = KEYPAD_GetKey();
		if (key != NULL && key != 'C')
		{
			passwordIndex = 0;
			
			LCD_Clear();
			LCD_GoTo(0, 0);
			LCD_WriteChar(key);
			
			passward[passwordIndex] = key;
			
			while (passward[passwordIndex] != '=' && passwordIndex < 99)
			{
				STEPPER1_Forward (Fire_System_CheckPeriodically);
			}
			passwordIndex++;
			passward[passwordIndex] = NULL;
			
			if (Fire_System_CompareString ("954", passward))
			{
				fireMode = 0;
			}
			else
			{
				Fire_System_State_WrongPassword ();
			}
		}
	}
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

static void Fire_System_State_WrongPassword ()
{
	LCD_Clear();
	LCD_GoTo(0, 0);
	LCD_WriteString("Wrong Password!!");
	Fire_System_delay_ms(3000);
	LCD_Clear();
}

static void Fire_System_delay_ms(u32 delay)
{
	for (u32 index = 0; index < (delay/ (DELAY * 4)); index++)
	{
		STEPPER1_Forward(Fire_System_GetPassward);
		
		if (fireMode == 0)
		{
			break;
		}
	}
}

