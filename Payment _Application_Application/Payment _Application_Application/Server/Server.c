#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS

#include <stdio.h>
#include "Server.h"
#include <string.h>
#include <time.h>
#include "../Card/Card.h"
#include "../Terminal/Terminal.h"

ST_accountsDB_t Accounts[255] = { { 2000.00 , RUNNING , "5399353562786336"} , {500.00 , RUNNING , "5399353562786344"} , {50.00 , BLOCKED , "5399353562786443"} };
ST_transaction_t Transactions[255] = { 0 };
ST_transaction_t* transaction_reference = 0; // the pointer used to access transactionsDB array

uint32_t Transaction_Number = 0;

uint16_t Acc_Number = 0;


EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	EN_serverError_t Vaild_Account_f;
	EN_serverError_t Amount_Available_f;
	EN_serverError_t Saving_Transaction_f;


	if (Vaild_Account_f = isValidAccount(&transData->Card_Holder_Data) == ACCOUNT_NOT_FOUND)
	{
		return DECLINED_STOLEN_CARD;
	}

	if (Amount_Available_f = isAmountAvailable(&transData->Terminal_Data) == LOW_BALANCE)
	{
		return DECLINED_INSUFFECIENT_FUND;
	}

	if (Saving_Transaction_f = saveTransaction(transData) == SAVING_FAILED)
	{
		return INTERNAL_SERVER_ERROR;
	}

	printf("\nYour Balance before The Transaction : %.2f", Accounts[Acc_Number].Balance);

	Accounts[Acc_Number].Balance = (Accounts[Acc_Number].Balance - transData->Terminal_Data.Transaction_Amount);

	printf("\nTou Balance After The Transaction : %.2f", Accounts[Acc_Number].Balance);

	return APPROVED;
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{
	for (Acc_Number = (0 + Acc_Number); Acc_Number < 255; Acc_Number++)
	{
		if (strcmp(cardData->Primary_Account_Number, Accounts[Acc_Number].Primary_Account_Number) == 0)
		{
			return SERVER_OK;
		}
	}
	return ACCOUNT_NOT_FOUND;
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
	for (Acc_Number = (0 + Acc_Number); Acc_Number < 255; Acc_Number++)
	{
		if (Accounts[Acc_Number].state == RUNNING)
		{
			return RUNNING_ACCOUNT;
		}
		else
		{
			return BLOCKED_ACCOUNT;
		}
	}
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{
	if (termData->Transaction_Amount < Accounts[Acc_Number].Balance)
	{
		return SERVER_OK;
	}
	return LOW_BALANCE;
}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	transData->Transaction_Sequence_Number = 0 + Transaction_Number;
	if (transData->Transaction_Sequence_Number < 255)
	{
		Transactions[transData->Transaction_Sequence_Number].Card_Holder_Data = transData->Card_Holder_Data;
		Transactions[transData->Transaction_Sequence_Number].Terminal_Data = transData->Terminal_Data;
		Transactions[transData->Transaction_Sequence_Number].Trans_State = transData->Trans_State;
		Transactions[transData->Transaction_Sequence_Number].Transaction_Sequence_Number = transData->Transaction_Sequence_Number + 1;

		Transaction_Number++;
		return SERVER_OK;
	}
	return SAVING_FAILED;
}

void listSavedTransactions(void)
{
	ST_transaction_t transactionData;
	ST_transaction_t* transaction_reference = &transactionData;
	
	printf("#########################");
	printf("\n");
	printf("Transaction Sequence Number: %d", Transaction_Number);  
	printf("\n");
	time_t TIME = time(NULL);
	struct tm t_m = *localtime(&TIME);
	printf("Transaction date: %d/%d/%d\n", t_m.tm_mday, t_m.tm_mon + 1, t_m.tm_year + 1900);
	printf("Transaction Amount: %d", Meow);
	printf("\n");


	switch (transaction_reference->Trans_State)
	{
	case APPROVED:
		printf("Transaction State: Approved");
		break;

	case DECLINED_INSUFFECIENT_FUND:
		printf("Transaction State: Declined -> Insufficient fund");
		break;

	case DECLINED_STOLEN_CARD:
		printf("Transaction State: Declined -> Blocked Account");
		break;

	
	case INTERNAL_SERVER_ERROR:
		printf("Transaction State: Declined -> Internal Server Error");
		break;
	}

	printf("\n");
	printf("Terminal Max Amount: %d", Meow_1);
	printf("\n");
	printf("Cardholder Name: %s", Meow_2.Card_Holder_Name);
	printf("\n");
	printf("PAN: %s", Meow_2.Primary_Account_Number);
	printf("\n");
	printf("Card Expiration Date: %s", Meow_2.Card_Expiration_Date);
	printf("\n");
	printf("#########################");
	printf("\n");
}