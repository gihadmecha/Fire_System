
#include "Fire_System_Private.h"
#include "Fire_System.h"

static u16 temperature;
static u16 smoke;

static u8 fireMode = 0;

extern void Fire_System_Init ()
{
	DIO_Init();
	ADC_Init(ADC_VCC, ADC_PRESCALER_64);
	LCD_Init();
	
	ADC_Enable();
	
	Fire_System_State_Fine ();
}

extern void Fire_System_Run ()
{
	temperature = LM35();
	
	if (temperature <= 400)
	{
		Fire_System_State_Fine();
	}
	else if (temperature > 450)
	{
		Fire_System_State_Heat ();
		
		smoke = MQ_2();
		
		if (smoke > 50)
		{
			do 
			{
				Fire_System_State_Fire();
				Fire_System_GetPassward ();
			} while (fireMode == 1);
		}
	}
}

static void Fire_System_State_Fine ()
{
	smoke = 0;

	LED1_Off();
	LED2_Off();
	
	STEPPER1_Stop ();
	
	//LCD_Clear();
	
	LCD_GoTo(0, 0);
	LCD_WriteString("Fine");
	LCD_GoTo(0, 6);
	LCD_WriteString("Temp:");
	LCD_WriteNumber_4Digit(temperature);
	LCD_GoTo(1, 6);
	LCD_WriteString("Smok:");
	LCD_WriteNumber_4Digit(smoke);
}

static void Fire_System_State_Heat ()
{
	LED1_On();
	LED2_Off();
	
	STEPPER1_Stop ();
	
	//LCD_Clear();
	
	LCD_GoTo(0, 0);
	LCD_WriteString("Heat");
	LCD_GoTo(0, 6);
	LCD_WriteString("Temp:");
	LCD_WriteNumber_4Digit(temperature);
	LCD_GoTo(1, 6);
	LCD_WriteString("Smok:");
	LCD_WriteNumber_4Digit(smoke);
}

static void Fire_System_State_Fire ()
{
	fireMode = 1;
	
	LED1_On();
	LED2_On();
	
	STEPPER1_Forward ();
	
	//LCD_Clear();
	
	LCD_GoTo(0, 0);
	LCD_WriteString("Fire");
	LCD_GoTo(0, 6);
	LCD_WriteString("Temp:");
	LCD_WriteNumber_4Digit(temperature);
	LCD_GoTo(1, 6);
	LCD_WriteString("Smok:");
	LCD_WriteNumber_4Digit(smoke);
}

static void Fire_System_GetPassward ()
{
	
}

