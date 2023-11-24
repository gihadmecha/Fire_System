

#ifndef GETPASSWARD_H_
#define GETPASSWARD_H_

#include "getPassward_Lcfg.h"
#include "StdTypes.h"

#define		GET_PASSWARD_WAIT		1
#define		GET_PASSWARD_DONE		2

extern void getPassward_Init ();
extern int getPassward_Run (u8 passward[], u8 passwardSize);


#endif /* GETPASSWARD_H_ */