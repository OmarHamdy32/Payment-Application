#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS

#include <stdio.h>
#include "../Card/Card.h"
#include "../Server/Server.h"
#include "Application.h"
#include "../Terminal/Terminal.h"


void appStart(void)
{
	// Card Data Struct
	ST_cardData_t cardHolderData;
	ST_cardData_t* cardData = &cardHolderData;

	ST_accountsDB_t account_Ref;
	ST_accountsDB_t* Account_Refrenc = &account_Ref;

	// Card Module : Get Card Info Name, Expiry Date, PAN
	if (getCardHolderName(cardData) == WRONG_NAME)
	{
		printf("Sorry, that's Wrong Name !\n");
		return 0;
	}

	if (getCardExpiryDate(cardData) == WRONG_EXP_DATE)
	{
		printf("Wrong Expiry Date !\n");
		return 0;
	}

	if (getCardPAN(cardData) == WRONG_PAN)
	{
		printf("Sorry, that's Wrong PAN !\n");
		return 0;
	}

	//--------------------------------------------//
	// Terminal Data Struct
	ST_terminalData_t terminalData;
	ST_terminalData_t* termData = &terminalData;

	// Terminal Module: Get Card Data, Transaction Date, Check if card is expired, Get transaction amount and check if it exceeds the max amount
	if (isValidCardPAN(cardData) == INVALID_CARD)
	{
		printf("Sorry, that's Invalid card");
		return 0;
	}

	if (isValidAccount(cardData) == SERVER_OK) {  // check if card running or blocked
		printf("The Account in database \n");
	}
	else {
		goto loop;
	}


	if (isBlockedAccount(Account_Refrenc) == RUNNING_ACCOUNT) {  // check if card running or blocked
		printf("The Card is Running \n");
	}
	else {
		printf("The Card is Blocked \n");
		return 0;
	}

	if (getTransactionDate(termData) == WRONG_DATE)
	{
		printf("Sorry, that's Wrong Date !");
		return 0;
	}

	if (isCardExpired(cardHolderData, terminalData) == EXPIRED_CARD)
	{
		printf("Your Card is Expired !, Please Refer To The Bank To Get Your a New One");
		return 0;
	}

	if (setMaxAmount(termData) == INVALID_MAX_AMOUNT)
	{
		printf("Invalid Max Amount");
		return 0;
	}
	
	if (getTransactionAmount(termData) == INVALID_AMOUNT)
	{
		printf("Invalid Amount !, Please Enter A Valid Amount");
		return 0;
	}

	if (isBelowMaxAmount(termData) == EXCEED_MAX_AMOUNT)
	{
		printf("Exceeded Max Amount !, You Can't Withdraw More Than The Max Amount Mentioned Above");
		return 0;
	}

	//--------------------------------------------//
	// Transaction Error Flag
	EN_transState_t Recieve_Transaction_f;

	// Transaction Data Struct
	ST_transaction_t transactonData;
	ST_transaction_t* transData = &transactonData;



	// Server Module: Get Card Data, and Terminal Data
	transData->Card_Holder_Data = cardHolderData;
	transData->Terminal_Data = terminalData;

	Recieve_Transaction_f = recieveTransactionData(transData);


	switch (Recieve_Transaction_f)
	{
	case APPROVED:
		printf("\nTransaction was successfull, Thank You !! \n");
		listSavedTransactions();
		break;

	case DECLINED_INSUFFECIENT_FUND:
		saveTransaction(transData);
		printf("\nTransaction Declined Due To Insuffecient Fund\n");
		listSavedTransactions();
		break;

	case DECLINED_STOLEN_CARD:
		saveTransaction(transData);
	loop: printf("\nTransaction Declined This Card Is Stolen, Please Return It To The Nearest Branch\n");
		listSavedTransactions();

		break;

	case INTERNAL_SERVER_ERROR:
		saveTransaction(transData);
		printf("\nTransaction Declined Due To Server Error, Please Try Again Later ..\n ");
		listSavedTransactions();
		break;

	default:
		break;
	}
}

