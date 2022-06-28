
#include "stm32f1xx_hal.h"




struct USBSTR{
uint32_t   TotalSize;
uint32_t   FreeSize;
uint8_t 	 Operation;
uint8_t  	 curser;
uint8_t	   ReadPage;
uint8_t  	 PageLock;
uint8_t  	 LoadPreset;
char 	     Buffer[41000];
};

struct FoundSTR{
uint16_t  Address;
uint8_t   Feeder_Page;
uint8_t 	Feeder_Lock;
uint16_t 	Feeder_Conter;
uint8_t 	FeederValue;
uint8_t	  Feeder;	
uint8_t   Mode;
};

struct GroupSTR{
uint8_t   Mode;
uint8_t 	NumberOFuptodate[37];
uint8_t 	Counter;
};
 
struct SettingsSTR{
uint8_t   Mode;
uint8_t 	Operation;
uint8_t 	Counter;
};

struct DissolveSTR{
uint8_t   Mode;
uint8_t  	Cut;
uint8_t   FeederValue;
uint8_t 	Value;
uint8_t 	Select;
uint8_t   First_Group;
uint8_t   Second_Group;
uint8_t 	Counter;
};

struct PresetSTR{
uint8_t   Mode;
uint8_t 	Empte;
uint8_t 	EraseAll;
uint8_t 	Page;
uint8_t 	Sector;
uint8_t   Name[20];
uint8_t   Operation;
uint8_t 	PreviewPage;
uint8_t   NameCounter;
uint8_t   Buffer[4096];
};

struct MasterSTR{
uint8_t   Value;
uint8_t 	Lock;
uint8_t 	Counter;
};

struct JoystickSTR{
uint8_t status;
uint8_t Address;
uint8_t Command1;
uint8_t Command2;
uint8_t Data1;
uint8_t Data2;
uint8_t Speed;
uint8_t Packt[25];
uint32_t	BaudRate;
uint8_t	Standard;
};



