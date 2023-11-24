


#ifndef CHANGEPASSWARD_PRIVATE_H_
#define CHANGEPASSWARD_PRIVATE_H_



#define		CHANGE_PASSWARD_WAIT			1
#define		CHANGE_PASSWARD_DONE			2

#define		CHANGE_PASSWARD_EnterPasswardScreen		0
#define		CHANGE_PASSWARD_ChangePasswardScreen	1

static void EEPROM_passwardRetrieval (u8 passward[], u16 passwardSize, u16 address);
static void EEPROM_KeyPad_passwardSave (u8 passward[], u16 passwardSize, u16 address);
static u8 EEPROM_KeyPad_getPassward (u8 passward[], u16 passwardSize, u16 address);
static int EEPROM_KeyPad_trigger ();




#endif /* CHANGEPASSWARD_PRIVATE_H_ */