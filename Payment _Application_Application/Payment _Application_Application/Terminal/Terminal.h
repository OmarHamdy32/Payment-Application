#ifndef TERMINAL_H
#define TERMINAL_H
#include "../Card/Card.h"

int Meow;
int Meow_1;
typedef unsigned char       uint8_t;
typedef signed char         int8_t;
typedef unsigned short      uint16_t;
typedef signed short	    int16_t;
typedef unsigned int		uint32_t;
typedef signed int			int32_t;
typedef unsigned long long  uint64_t;
typedef signed long long    int64_t;


typedef struct ST_terminalData_t
{
	float Transaction_Amount;
	float Max_Transaction_Amount;
	uint8_t Transaction_Date[11];
}ST_terminalData_t;

typedef enum EN_terminalError_t
{
	TERMINAL_OK,			//0
	WRONG_DATE,		    	//1
	EXPIRED_CARD,			//2
	INVALID_CARD,			//3
	INVALID_AMOUNT,			//4
	EXCEED_MAX_AMOUNT,		//5
	INVALID_MAX_AMOUNT		//6
}EN_terminalError_t;

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData);
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData);
EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData);
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData);
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData);
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData);


#endif // !TERMINAL_Hs