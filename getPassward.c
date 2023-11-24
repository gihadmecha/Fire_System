
#include "getPassward_Private.h"
#include "getPassward.h"

static char key;

extern void getPassward_Init ()
{
	DIO_Init();
}

static int getPassward_trigger ()
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

extern int getPassward_Run (u8 passward[], u8 passwardSize)
{
	LCD_GoTo(0, 0);
	LCD_WriteString("Enter Passward:");
	
	static u8 state = GET_PASSWARD_WAIT;
	
	static u8 passwordIndex = 0;
	if (passward != NULL && passwardSize > 0 && passwordIndex < passwardSize)
	{
		if (getPassward_trigger ())
		{
			if (key != NULL)
			{
				passward[passwordIndex] = key;
				
				if (passward[passwordIndex] == '=' || passwordIndex == passwardSize - 1)
				{
					passward[passwordIndex] = NULL;
					passwordIndex = 0;
					
					return GET_PASSWARD_DONE;
				}
				else
				{
					passwordIndex++;
					passward[passwordIndex] = NULL;
					
					return GET_PASSWARD_WAIT;
				}
			}
		}
	}
	
	return state;
}
