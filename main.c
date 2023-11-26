
//#include "getPassward.h"

//int main ()
//{
	//getPassward_Init ();
	//LCD_Init();
	//
	//static u8 passward[20] = {0};
	//u8 passwardSize = sizeof(passward)/sizeof(passward[0]);
	//while (1)
	//{
		//getPassward_Run (passward, passwardSize);
		//
		//LCD_GoTo(1, 0);
		//LCD_WriteString(passward);
		//LCD_WriteString("				");
	//}
//}

//#include "changePassward.h"
//
//int main ()
//{
	//DIO_Init();
	//LCD_Init();
	//
	//static u8 newPassward[20] = {NULL};
	//static u8 newPasswardSize = sizeof (newPassward)/sizeof (newPassward[0]);
//
	//changePassward_Init(newPassward, newPasswardSize);
	//
	//
	//while (1)
	//{
		//changePassward_Run(newPassward, newPasswardSize);
	//}
//}

#include "Fire_System.h"

int main ()
{
	Fire_System_Init ();

	while (1)
	{
		Fire_System_Run ();
	}
}