#ifndef SERVER_H
#define SERVER_H


typedef unsigned char       uint8_t;
typedef signed char         int8_t;
typedef unsigned short      uint16_t;
typedef signed short	    int16_t;
typedef unsigned int		uint32_t;
typedef signed int			int32_t;
typedef unsigned long long  uint64_t;
typedef signed long long    int64_t;

#include "../Card/Card.h"
#include "../Terminal/Terminal.h"

typedef enum EN_transState_t
{
	APPROVED,					//0
	DECLINED_INSUFFECIENT_FUND, //1
	DECLINED_STOLEN_CARD,		//2
	INTERNAL_SERVER_ERROR		//3
}EN_transState_t;

typedef struct ST_transaction_t
{
	ST_cardData_t Card_Holder_Data;
	ST_terminalData_t Terminal_Data;
	EN_transState_t Trans_State;
	uint32_t Transaction_Sequence_Number;
}ST_transaction_t;

typedef enum EN_serverError_t
{
	SERVER_OK,					//0
	SAVING_FAILED,	     		//1
	TRANSACTION_NOT_FOUND,		//2
	ACCOUNT_NOT_FOUND,			//3
	LOW_BALANCE,				//4
	BLOCKED_ACCOUNT,            //5
	RUNNING_ACCOUNT             //6
}EN_serverError_t;

typedef enum EN_accountState_t
{
	RUNNING,
	BLOCKED
}EN_accountState_t;

typedef struct ST_accountsDB_t
{
	float Balance;
	EN_accountState_t state;
	uint8_t Primary_Account_Number[20];
}ST_accountsDB_t;

EN_transState_t recieveTransactionData(ST_transaction_t* transData);
EN_serverError_t isValidAccount(ST_cardData_t* cardData);
EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence);
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData);
EN_serverError_t saveTransaction(ST_transaction_t* transData);
void listSavedTransactions(void);


extern ST_accountsDB_t Accounts[255];
extern ST_transaction_t Transactions[255];



#endif // !SERVER_H
