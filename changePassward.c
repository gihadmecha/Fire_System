
#include "changePassward.h"
#include "changePassward_Private.h"

static u8 firstUse = 0;

static u8 passward[20] = {NULL};
static u8 passwardSize = sizeof (passward)/sizeof (passward[0]);

static u16 addressAddress = 0;
static u16 counterAddress = 2;
static u16 eepromWriteCounter = 0;

static u16 dataAddress;

static u16 passwardAddress = 3;
static u8* dataAddressPointer = &dataAddress;

static u8 key = NULL;

static u8 state = CHANGE_PASSWARD_WAIT;

static u8 retrievalDone = 0;

extern void changePassward_Init (u8 newPassward[], u8 newPasswardSize)
{
	if (newPassward != NULL)
	{
		//DIO_Init();
		//LCD_Init();
		
		if (firstUse == 0)
		{
			addressAddress = 0;
			dataAddress = 2;
			
			counterAddress = dataAddress;
			eepromWriteCounter = 0;

			passwardAddress = 3;
			
			EEPROM_write (passwardAddress,     '0');
			EEPROM_write (passwardAddress + 1, '0');
			EEPROM_write (passwardAddress + 2, '0');
			EEPROM_write (passwardAddress + 3, '0');
			EEPROM_write (passwardAddress + 4, NULL);
			
			firstUse = 1;
		}
		
		*(dataAddressPointer + 0) = EEPROM_read(addressAddress);
		*(dataAddressPointer + 1) = EEPROM_read(addressAddress + 1);
		
		if ( dataAddress > 1023 - passwardSize - 1)
		{
			dataAddress = 2;
			EEPROM_write(addressAddress, *(dataAddressPointer + 0));
			EEPROM_write(addressAddress - 1, *(dataAddressPointer + 1) );
		}
		
		counterAddress = dataAddress;
		eepromWriteCounter = EEPROM_read(counterAddress);
		
		if (eepromWriteCounter >= 3)
		{
			EEPROM_write (counterAddress, 0);
			eepromWriteCounter = 0;
		}
		
		passwardAddress = dataAddress + 1;
		EEPROM_passwardRetrieval ( newPassward, newPasswardSize, passwardAddress);
	}
}

extern u8 changePassward_Run (u8 newpassward[], u8 newpasswardSize)
{
	EEPROM_KeyPad_getPassward ( passward, passwardSize, passwardAddress);
	
	if (state == CHANGE_PASSWARD_DONE && retrievalDone == 1)
	{
		EEPROM_passwardRetrieval ( newpassward, newpasswardSize, passwardAddress);
		
		retrievalDone = 0;
	}
		
	if (eepromWriteCounter == 3)
	{
		dataAddress = dataAddress + passwardSize + 1;
		
		if ( (dataAddress > 1023 - passwardSize - 1) || (dataAddress < 2) )
		{
			dataAddress = 2;
		}
		
		EEPROM_write(addressAddress, *(dataAddressPointer + 0));
		EEPROM_write(addressAddress + 1, *(dataAddressPointer + 1) );
		
		counterAddress = dataAddress;
		EEPROM_write(counterAddress, 0);
		eepromWriteCounter = 0;
		
		u8 index;
		u8 eeprom_passwardReadChar = EEPROM_read(passwardAddress + 0);
		for (index = 0; eeprom_passwardReadChar;  )
		{
			EEPROM_write(dataAddress + 1 + index, eeprom_passwardReadChar);
			index++;
			eeprom_passwardReadChar = EEPROM_read(passwardAddress + index);
		}
		EEPROM_write(dataAddress + 1 + index, NULL);
		passwardAddress = dataAddress + 1;
	}
		
	LCD_GoTo(0, 0);
	LCD_WriteNumber (dataAddress);
	LCD_GoTo(1, 0);
	LCD_WriteNumber_4Digit(addressAddress);
	
	LCD_GoTo(0, 6);
	LCD_WriteNumber_4Digit(eepromWriteCounter);
	LCD_GoTo(1, 6);
	LCD_WriteNumber_4Digit(counterAddress);
	
	LCD_GoTo(0, 11);
	if (state == CHANGE_PASSWARD_DONE)
	{
		LCD_WriteString(newpassward);
	}
	else if (state == CHANGE_PASSWARD_WAIT)
	{
		LCD_WriteString(passward);
	}
	LCD_WriteString("    ");
	
	LCD_GoTo(1, 11);
	LCD_WriteNumber_4Digit(passwardAddress);
	
	return state;
}

static void EEPROM_passwardRetrieval (u8 passward[], u16 passwardSize, u16 address)
{
	if (passwardSize > 0)
	{
		passward[0] = EEPROM_read(address);
		
		u16 index;
		for (index = 1; index < (passwardSize - 1) && passward[index - 1] != NULL; index++)
		{
			passward[index] = EEPROM_read(address + index);
		}
		passward[index] = NULL;
	}
}

static int EEPROM_KeyPad_trigger ()
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

static void EEPROM_KeyPad_passwardSave (u8 passward[], u16 passwardSize, u16 address)
{
	u8 index;
	for (index = 0; passward[index]; index++)
	{
		EEPROM_write (address + index, passward[index]);
	}
	EEPROM_write (address + index, NULL);
	
	EEPROM_write (counterAddress, ++eepromWriteCounter);
	
	passward[0] = NULL;
}

static void EEPROM_KeyPad_getPassward (u8 passward[], u16 passwardSize, u16 address)
{
	static u16 index = 0;
	
	if (passward != NULL && index < passwardSize-1)
	{
		state = CHANGE_PASSWARD_WAIT;
		
		if (EEPROM_KeyPad_trigger ())
		{
			if (key != NULL)
			{
				if (key != '=' && key != 'C')
				{
					passward[index] = key;
					index++;
					passward[index] = NULL;
					state = CHANGE_PASSWARD_WAIT;
				}
				
				else if (key == '=')
				{
					EEPROM_KeyPad_passwardSave (passward, passwardSize, address);
					
					index = 0;
					retrievalDone = 1;
					state = CHANGE_PASSWARD_DONE;
				}
				
				else if (key == 'C')
				{
					index = 0;
					passward[0] = NULL;
					state = CHANGE_PASSWARD_DONE;
				}
			}
		}
	}
}